#include "networking.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

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
    client_socket = accept(sd, (struct sockaddr *)&client_address, &sock_size);

    if (client_socket < 0)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            error_check(client_socket, "accept client socket");
        }
    }

    if (client_socket > -1)
    {
        error_check(fcntl(client_socket, F_SETFL, fcntl(client_socket, F_GETFL, 0) | O_NONBLOCK), "set O_NONBLOCK");
    }

    return client_socket;
}
//return bound socket if connction accepted, or -1 otherwise
int connect_to_server(char *ip)
{
    int sd;

    //create the socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    error_check(sd, "client socket");

    //run getaddrinfo
    /* hints->ai_flags not needed because the client
        specifies the desired address. */
    struct addrinfo *hints, *results;
    hints = (struct addrinfo *)calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_INET;       //IPv4
    hints->ai_socktype = SOCK_STREAM; //TCP socket
    getaddrinfo(ip, PORT, hints, &results);

    //connect to the server
    //connect will bind the socket for us
    int i = connect(sd, results->ai_addr, results->ai_addrlen);

    free(hints);
    freeaddrinfo(results);

    if (i < 0)
    {
        close(sd);
        return -1;
    }

    error_check(fcntl(sd, F_SETFL, fcntl(sd, F_GETFL, 0) | O_NONBLOCK), "set client to server socket to nonblock");

    return sd;
}