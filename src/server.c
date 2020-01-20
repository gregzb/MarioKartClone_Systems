#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "networking.h"
#include "kart.h"
#include "level.h"
#include "time_util.h"

#define CONNECT_COUNTDOWN 30

struct client_connection
{
    int socket_descriptor;
    struct timespec last_received;
    struct client client;
    SDL_Point wasd;
    //TODO: sunan-- add fields for client
    //kart (use kart stuff: has positions, acceleration, etc)
    //currently pressed keys
    //other info you need
};

enum game_state
{
    WAITING_FOR_CLIENTS,
    BEGINNING_RACE,
    IN_RACE
};

//removes clients[index] from clients, shifting
void remove_client(struct client_connection *clients, size_t *length, size_t index);

//gets dt time depending on game state
double min_loop_time(enum game_state);

void server_instance();

void handle_sigquit(int signum);

bool restart;

void server_main(int read_pipe)
{
    struct sigaction action = {0};
    action.sa_handler = &handle_sigquit;
    sigaction(SIGQUIT, &action, NULL);

    while (true)
    {
        //TODO: use data type conveying meaning

        bool buffer;

        //block until we are told to start network code
        read(read_pipe, &buffer, sizeof buffer);

        restart = false;

        server_instance();
    }
}

void server_instance()
{
    printf("Starting network code.\n");

    srand(time(NULL));

    int socket = server_setup();

    struct client_connection clients[MAX_CLIENTS];
    size_t num_clients = 0;

    //id which will be given to next connecting client
    int next_id = 1;

    //array of levels
    struct level levels[8];
    int num_levels = 0;

    for (int i = 0; level_names[i] != NULL; i++)
    {
        levels[i] = level_init(NULL, level_names[i]);
        num_levels++;
    }

    int current_level = 0;

    //time at which countdown begins
    struct timespec countdown_start;
    clock_gettime(CLOCK_MONOTONIC, &countdown_start);

    enum game_state game_state = WAITING_FOR_CLIENTS;
    enum game_state next_game_state = game_state;

    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    while (true)
    {
        double dt = get_delta_time(current_time);

        if (dt < min_loop_time(game_state))
        {
            continue;
        }

        //exit if parent no longer exists
        if (getppid() == 1)
            exit(0);

        // UNCOMMENT
        // if (num_clients < MIN_CLIENTS) {
        //     clock_gettime(CLOCK_MONOTONIC, &countdown_start);
        // }

        clock_gettime(CLOCK_MONOTONIC, &current_time);

        //handle new conenctions
        int new_connection = try_listen_for_client(socket);
        if (new_connection != -1)
        {
            printf("Server: got new connection.\n");
            struct server_packet packet = {.type = CONNECTION_REQUEST_RESPONSE};
            struct connection_response response = {0};

            //rejected if MAX_CLIENTS reached or we're not waiting for clients
            response.accepted = !(num_clients >= MAX_CLIENTS) && game_state == WAITING_FOR_CLIENTS;
            response.id = next_id;

            packet.data.connection_request_response = response;

            write(new_connection, &packet, sizeof(packet));

            if (response.accepted)
            {
                struct client client = {.kart = kart_init(), .id = next_id++};
                struct client_connection new_client =
                    {.socket_descriptor = new_connection,
                     .last_received = current_time,
                     .client = client,
                     .wasd = {0}};
                clients[num_clients++] = new_client;
                printf("(testing server) new client connected; %zd clients\n", num_clients);
                countdown_start = current_time;
            }
            else
            {
                close(new_connection);
            }
        }

        //process messages
        for (size_t i = 0; i < num_clients; i++)
        {
            struct client_packet packet;

            //get all queued messages
            while (true)
            {
                ssize_t size = read(clients[i].socket_descriptor, &packet, sizeof packet);
                if (size == 0)
                {
                    remove_client(clients, &num_clients, i--);
                    break;
                }
                else if (size < 0)
                {
                    //if there is an error rather than a lack of data
                    if (errno != EWOULDBLOCK && errno != EAGAIN)
                    {
                        printf("Error reading from client socket: %s\n", strerror(errno));
                        remove_client(clients, &num_clients, i--);
                    }
                    break;
                }

                //handle messages
                switch (packet.type)
                {
                case CONNECTION_REQUEST:
                    //todo
                    break;
                case KEEP_ALIVE:
                    //dummy, does nothing
                    break;
                case CURRENT_INPUTS:
                    for (int i = 0; i < num_clients; i++)
                    {
                        if (clients[i].client.id == packet.data.current_inputs.id)
                        {
                            clients[i].wasd = packet.data.current_inputs.wasd;
                            // for (int j = 0; j < current_level.num_boxes; j++)
                            // {
                            //     SDL_Rect rect = current_level.collision_boxes[j];
                            //     kart_handle_collision(&clients[j].client.kart, &rect, dt);
                            // }
                            kart_move(&clients[i].client.kart, clients[i].wasd.y, clients[i].wasd.x, dt);

                            //handle collision with level
                            for (int j = 0; j < levels[current_level].num_boxes; j++)
                            {
                                SDL_Rect rect = levels[current_level].collision_boxes[j];
                                kart_handle_collision(&clients[i].client.kart, &rect, dt);
                            }
                            break;
                        }
                    }
                }

                clients[i].last_received = current_time;
            }
        }

        //remove expired connections
        for (size_t i = 0; i < num_clients; i++)
        {
            //disconnect if the client has not communicated in 5 seconds
            if (current_time.tv_sec - clients[i].last_received.tv_sec > 5)
            {
                remove_client(clients, &num_clients, i--);
            }
        }

        if (game_state == WAITING_FOR_CLIENTS)
        {
            //start game if all possible client connected
            if (num_clients == MAX_CLIENTS)
            {
                next_game_state = BEGINNING_RACE;
            }

            //if the countdown has elapsed and there are at least two players start
            //else if (current_time.tv_sec - countdown_start.tv_sec >= CONNECT_COUNTDOWN && num_clients >= MIN_CLIENTS)
            else if (current_time.tv_sec - countdown_start.tv_sec >= CONNECT_COUNTDOWN && num_clients > 0)
            {
                next_game_state = BEGINNING_RACE;
            }
        }

        struct server_packet packet = {0};

        if (game_state == WAITING_FOR_CLIENTS)
        {
            packet.type = WAIT_STATUS;
            struct wait_status wait_status = {.seconds_left = CONNECT_COUNTDOWN - (current_time.tv_sec - countdown_start.tv_sec),
                                              .client_ids_length = num_clients};
            //fill client_ids
            for (int i = 0; i < num_clients; i++)
            {
                wait_status.client_ids[i] = clients[i].client.id;
            }
            packet.data.wait_status = wait_status;
        }
        else if (game_state == BEGINNING_RACE)
        {
            packet.type = START_RACE;
            struct start_race start = {.level = current_level};
            packet.data.start_race = start;

            for (int i = 0; i < num_clients; i++)
            {
                clients[i].client.kart.position.x = levels[current_level].spawn_points[i].x;
                clients[i].client.kart.position.y = levels[current_level].spawn_points[i].y;
            }

            next_game_state = IN_RACE;
        }
        else if (game_state == IN_RACE)
        {
            packet.type = CLIENT_POSITIONS;

            // for (int i = 0; i < num_clients; i++)
            // {
            //     kart_move(&clients[i].client.kart, clients[i].wasd.y, clients[i].wasd.x, dt);
            // }

            for (int i = 0; i < num_clients; i++)
            {
                packet.data.client_positions.clients[i] = clients[i].client;
            }
            packet.data.client_positions.num_clients = num_clients;
        }

        //printf("Sending message type %d\n", (int)packet.type);

        //send packet to all clients
        for (int i = 0; i < num_clients; i++)
        {
            write(clients[i].socket_descriptor, &packet, sizeof packet);
        }

        game_state = next_game_state;

        if (restart)
        {
            close(socket);

            for (int i = 0; i < num_clients; i++)
            {
                close(clients[i].socket_descriptor);
            }
            return;
        }
    }
}

void remove_client(struct client_connection *clients, size_t *length, size_t index)
{
    close(clients[index].socket_descriptor);

    for (size_t i = index; i < *length - 1; i++)
    {
        clients[i] = clients[i + 1];
    }

    (*length)--;
}

double min_loop_time(enum game_state state)
{
    switch (state)
    {
    case WAITING_FOR_CLIENTS:
        return 1.0;
    default:
        return 1.0 / 60.0;
    }
}

void handle_sigquit(int signum)
{
    printf("Caught SIGQUIT.\n");
    restart = true;
}
