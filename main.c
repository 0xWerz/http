// THIS IS BARELY HTTP SERVER IMPLEMENTATION

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "server.h"

volatile sig_atomic_t stop = 0;

void signal_handler(int signum) {
    if (signum == SIGINT) {
        printf("\nBye!\n");
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char *argv[]) {
    // Install a signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, signal_handler);

    // check if the corrent number of arguments are passed:
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_address> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse the server address and port from the command-line arguments.
    const char *server_address = argv[1];
    int port = atoi(argv[2]);

    // Start the server.
    start_server(server_address, port);

    return 0;
}