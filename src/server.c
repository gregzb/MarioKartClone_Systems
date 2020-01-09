#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#include "networking.h"

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

void server_main()
{
    //create signal set with SIGUSR!
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);

    //wait for SIGUSR1, indicates server will start
    int caught_signal;
    sigwait(&sigset, &caught_signal);

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
        if (new_connection)
        {
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
                    countdown_start = current_time;
                    break;
                }
                else if (size < 0)
                {
                    //if there is an error rather than a lack of data
                    if (errno != EWOULDBLOCK && errno != EAGAIN)
                    {
                        printf("Error reading from client socket: %s\n", strerror(errno));
                        remove_client(clients, &num_clients, i--);
                        countdown_start = current_time;
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
        if (num_clients == MAX_CLIENTS) break;

        //if the 30 second countdown has elapsed and there are at least two players start
        else if (current_time.tv_sec - countdown_start.tv_sec >= 30 && num_clients > 1) break;

        //TODO: send wait message to all clients

        //TODO: spin until 500ms consumed
    }

    //TODO: handle game start code

    //send game start packet(s) here

    //game loop here
}

void remove_client(struct client *clients, size_t *length, size_t index)
{
    printf("remove_client: todo\n");
    exit(1);
}

// CODE BELOW IS DIRECTLY "FORKED" FROM MR. DW

void error_check(int i, char *s)
{
    if (i < 0)
    {
        fprintf(stderr, "[%s] error %d: %s\n", s, errno, strerror(errno));
        exit(1);
    }
}

/*=========================
  server_setup
  args:
  creates, binds a server side socket
  and sets it to the listening state
  returns the socket descriptor
  =========================*/
int server_setup()
{
    int sd, i;

    //create the socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    error_check(sd, "server socket");
    printf("[server] socket created\n");

    error_check(fcntl(sd, F_SETFL, fcntl(sd, F_GETFL, 0) | O_NONBLOCK), "set server socket to nonblock");

    //setup structs for getaddrinfo
    struct addrinfo *hints, *results;
    hints = (struct addrinfo *)calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_INET;               //IPv4 address
    hints->ai_socktype = SOCK_STREAM;         //TCP socket
    hints->ai_flags = AI_PASSIVE;             //Use all valid addresses
    getaddrinfo(NULL, PORT, hints, &results); //NULL means use local address

    //bind the socket to address and port
    i = bind(sd, results->ai_addr, results->ai_addrlen);
    error_check(i, "server bind");
    printf("[server] socket bound\n");

    //set socket to listen state
    i = listen(sd, 10);
    error_check(i, "server listen");
    printf("[server] socket in listen state\n");

    //free the structs used by getaddrinfo
    free(hints);
    freeaddrinfo(results);
    return sd;
}

/*=========================
  try_listen_for_client
  args: int sd
  sd should refer to a socket in the listening state
  run the accept call
  returns the socket descriptor for the new socket connected
  to the client, or -1 if no accept available
  =========================*/
int try_listen_for_client(int sd)
{
    int client_socket;
    socklen_t sock_size;
    struct sockaddr_storage client_address;

    sock_size = sizeof(client_address);
    client_socket = accept4(sd, (struct sockaddr *)&client_address, &sock_size, O_NONBLOCK);
    if (client_socket < 0)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            error_check(client_socket, "accept4 client socket");
        }
    }

    return client_socket;
}