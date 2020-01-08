#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

void server_main() {
    //create signal set with SIGUSR!
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);

    //wait for SIGUSR1, indicates server will start
    int caught_signal;
    sigwait(&sigset, &caught_signal);

    //start listening to connections
    //TODO
}