//The networking files define and implement shared networking structures and functions.

#ifndef NETWORKING_HEADER
#define NETWORKING_HEADER

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define PORT "40934"
#define MAX_CLIENTS 4

enum client_packet_type
{
    CONNECTION_REQUEST,
    //sent so server does not disconnect client
    KEEP_ALIVE
};

struct connection_request {};
struct keep_alive {};

//a tagged union representing all possible client messages
struct client_packet 
{
    enum client_packet_type type;
    union {
        struct connection_request connection_request;
        struct keep_alive keep_alive;
    } data;
};

enum server_packet_type
{
    CONNECTION_REQUEST_RESPONSE,
    WAIT_STATUS
};

struct connection_response
{
    //true if connection accepted, false otherwise
    bool accepted;
    //the following fields are only valid if accepted is true
    //an id unique to the client in the server instance.
    uint8_t id;
};

//sent to clients while they are waiting for game start
struct wait_status
{
    int seconds_left;
    int client_ids[MAX_CLIENTS];
    size_t client_ids_length;
};

struct server_packet
{
    enum server_packet_type type;
    union {
        struct connection_response connection_request_response;
        struct wait_status wait_status;
    } data;
};

int connect_to_server(char *ip);
void error_check(int i, char *s);
int server_setup();
int try_listen_for_client(int sd);
int connect_to_server(char *ip);

#endif