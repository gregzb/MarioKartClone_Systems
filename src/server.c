#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "networking.h"

#define CONNECT_COUNTDOWN 30

struct client
{
    int socket_descriptor;
    struct timespec last_received;
    int id;
};

int server_setup();

int try_listen_for_client(int sd);

//removes clients[index] from clients, shifting
void remove_client(struct client *clients, size_t *length, size_t index);

void server_main(int read_pipe)
{
    printf("Waiting for pipe write from main process.\n");

    //TODO: use data type conveying meaning

    bool buffer;

    //block until we are told to start network code
    read(read_pipe, &buffer, sizeof buffer);

    printf("Starting network code.\n");

    int socket = server_setup();

    struct client clients[MAX_CLIENTS];
    size_t num_clients = 0;

    //id which will be given to next connecting client
    int next_id = 0;

    //time at which countdown begins
    struct timespec countdown_start;
    clock_gettime(CLOCK_MONOTONIC, &countdown_start);

    while (true)
    {
        struct timespec current_time;
        clock_gettime(CLOCK_MONOTONIC, &current_time);

        //handle new conenctions
        int new_connection = try_listen_for_client(socket);
        if (new_connection != -1)
        {
<<<<<<< HEAD
            printf("Server: got new connection.");
=======
            //wot is this type
>>>>>>> b884dbeb89d70a91cf856df78e793bd732e75443
            struct server_packet packet = {.type = CONNECTION_REQUEST_RESPONSE};
            struct connection_response response = {0};

            //rejected if MAX_CLIENTS reached
            response.accepted = !(num_clients == MAX_CLIENTS);
            response.id = next_id;

            packet.data.connection_request_response = response;

            write(new_connection, &packet, sizeof(packet));

            if (response.accepted)
            {
                struct client new_client =
                    {.socket_descriptor = new_connection,
                     .last_received = current_time,
                     .id = next_id++};
                clients[num_clients++] = new_client;
<<<<<<< HEAD
                printf("(testing server) new client connected; %zd clients\n", num_clients);
                countdown_start = current_time;
=======
                printf("(testing server) new client connected; %ld clients\n", num_clients);
>>>>>>> b884dbeb89d70a91cf856df78e793bd732e75443
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

        //start game if all possible client connected
        if (num_clients == MAX_CLIENTS)
            break;

        //if the countdown has elapsed and there are at least two players start
        else if (current_time.tv_sec - countdown_start.tv_sec >= CONNECT_COUNTDOWN && num_clients > 1)
            break;

        struct server_packet wait_packet = {.type = WAIT_STATUS};
        struct wait_status wait_status = {.seconds_left = CONNECT_COUNTDOWN - (current_time.tv_sec - countdown_start.tv_sec),
                                          .client_ids_length = num_clients};
        //fill client_ids
        for (int i = 0; i < num_clients; i++)
        {
            wait_status.client_ids[i] = clients[i].id;
        }
        wait_packet.data.wait_status = wait_status;

        //send wait status to all clients
        for (int i = 0; i < num_clients; i++)
        {
            write(clients[i].socket_descriptor, &wait_packet, sizeof wait_packet);
        }

        //spin until 1s elapsed
        struct timespec spin_time;
        do
        {
            clock_gettime(CLOCK_MONOTONIC, &spin_time);
        } while (spin_time.tv_sec < current_time.tv_sec + 1);
    }

    printf("(testing) game loop started!");

    //TODO: handle game start code

    //send game start packet(s) here

    //game loop here
}

void remove_client(struct client *clients, size_t *length, size_t index)
{
    close(clients[index].socket_descriptor);

    for (size_t i = index; i < *length - 1; i++)
    {
        clients[i] = clients[i + 1];
    }

    (*length)--;
}
