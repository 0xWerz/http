#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "router.h"

/**
 * The arpa/inet.h header file provides functions for manipulating Internet addresses.
 * It includes functions for converting between network byte order and host byte order,
 * as well as functions for manipulating IP addresses and port numbers.
 *
 * The functions in this header file are commonly used in network programming to handle
 * IP addresses and network communication.

 * The <netinet/in.h> header file contains definitions for the Internet Protocol (IP) family of protocols, including structures for IP addresses and socket addresses.
 * It provides the necessary definitions and functions for working with network sockets and IP addresses.
 * This header file is commonly used in network programming to handle network communication.

 * The <stdbool.h> header file is a standard C library header file that defines the bool data type and the true and false values.

 * The <stdio.h> header file defines several macros that specify how input and output is to be performed within a C program.

 * The <stdlib.h> header file provides functions for performing general functions in C programs, such as dynamic memory allocation, random number generation, communication with the environment, integer arithmetics, searching, sorting and converting.

 * The <string.h> header file provides functions for manipulating arrays of characters or strings in C.

 * The <sys/socket.h> header file is a standard C library header file that contains definitions of data structures needed for the socket interface.

 * The <unistd.h> header file is a standard C library header file that provides access to the POSIX operating system API.
 */

#define BUFFER_SIZE 1024

void handle_client(int client_sockfd) {
    // Read data from the client using the read() system call.
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(client_sockfd, buffer, BUFFER_SIZE);
    if (bytes_read < 0) {
        perror("Failed to read data from client");
        return;
    }

    // Parse the request path from the HTTP request.
    char *path = strtok(buffer, " ");
    if (path == NULL) {
        perror("Failed to parse request path");
        return;
    }

    // Generate an HTTP response based on the request path.
    char response[BUFFER_SIZE];
    handle_request(buffer, response, sizeof(response));

    // Send the HTTP response to the client.
    ssize_t bytes_written = write(client_sockfd, response, strlen(response));

    if (bytes_written < 0) {
        perror("Failed to write data to client");
        return;
    }

    printf("Response sent to client:\n%s\n", response);

    // Close the client socket.
    close(client_sockfd);
}

void start_server(const char *server_address, int port) {
    // Create a socket using the socket() system call.
    printf("Creating socket...\n");
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Define the server address and port.
    printf("Defining server address and port...\n");
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_address);
    server_addr.sin_port = htons(port);

    // Bind the socket to the server address using the bind() system call.
    printf("Binding socket to server address...\n");
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections using the listen() system call.
    printf("Listening for incoming connections on port %s:%d...\n", server_address, port);
    if (listen(sockfd, 5) < 0) {
        perror("Failed to listen for incoming connections");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connections using the accept() system call.
    while (true) {
        printf("Waiting for incoming connection...\n");
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sockfd < 0) {
            perror("Failed to accept incoming connection");
            exit(EXIT_FAILURE);
        }

        // Handle the client request.
        handle_client(client_sockfd);

        // Close the client socket.
        close(client_sockfd);
    }

    // Close the server socket.
    close(sockfd);
}
