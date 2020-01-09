#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
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
    int socket_id;
};

int server_setup();

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
    int num_clients = 0;


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
  listen_for_client
  args: int sd
  sd should refer to a socket in the listening state
  run the accept call
  returns the socket descriptor for the new socket connected
  to the client.
  =========================*/
int listen_for_client(int sd)
{
    int client_socket;
    socklen_t sock_size;
    struct sockaddr_storage client_address;

    sock_size = sizeof(client_address);
    client_socket = accept4(sd, (struct sockaddr *)&client_address, &sock_size, O_NONBLOCK);
    error_check(client_socket, "server accept");

    return client_socket;
}