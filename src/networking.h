//The networking files define and implement shared networking structures and functions.

#ifndef NETWORKING_HEADER
#define NETWORKING_HEADER

#include <stdint.h>
#include <stdbool.h>

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
    CONNECTION_REQUEST_RESPONSE
};

struct connection_response
{
    //true if connection accepted, false otherwise
    bool accepted;
    //the following fields are only valid if accepted is true
    //an id unique to the client in the server instance.
    uint8_t id;
};

struct server_packet
{
    enum server_packet_type type;
    union {
        struct connection_response connection_request_response;
    } data;
};

#endif