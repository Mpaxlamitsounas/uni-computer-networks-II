// Winsock2.h is windows only
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

// Socket programming on Windows uses different libraries than on 
// UNIX systems. This will run on UNIX, but no compile

FILE *file;
int socketfd;
// Windows only
WSADATA wsa;

void close_connections();

int main() {

    atexit(close_connections);

    // Windows only
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        printf("\nError: Windows socket subsytsem could not be initialized. Error Code: %d. Exiting..\n", WSAGetLastError());
        exit(1);
    }

    // Creates an identifier for the socket
    // fd short for file descriptor
    socketfd = socket(  AF_INET, // IPv4
                        SOCK_STREAM, // TCP
                        0); // IP protocol number
    if (socketfd < 0) {
        puts("Error creating socket");
        exit(1);
    }

    // provides server info
    struct sockaddr_in serv_addr;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080); // server on port 8080

    // binds the server program to the port
    int result = bind(  socketfd,
                        (struct sockaddr *) &serv_addr, // server details
                        sizeof(serv_addr)); // length of server details
    if (result < 0) {
        puts("Failed to bind to port");
        exit(1);
    }

    // makes server listen for requests
    result = listen(socketfd, 3);
    if (result < 0) {
        puts("Server could not listen to socket");
        exit(1);
    }
    puts("Server started");

    int size = sizeof(serv_addr);
    unsigned int counter = 1;
    while (TRUE) {

        // creates new socket just for file transfer
        int transfer_socket = accept(   socketfd, 
                                        (struct sockaddr*) &serv_addr, 
                                        (int *) &size);
        if (transfer_socket < 0) {
            puts("Failed to accept connection");
            continue; // ignores the failure and instead waits for a new connection
        }

        char filepath[32];
        // each file received has a different name
        sprintf(filepath,  "./receive%u.jpg", counter++);
        // opens file to be written to
        file = fopen(filepath, "wb");
        if (file == NULL) {
            puts("Failed to open file");
            exit(1);
        }

        char buffer[2048];
        int bytes_count = 0, bytes_received = 0;
        do {
            // receives up to 2048 bytes at a time from client
            bytes_received = recv(transfer_socket, buffer, 2048, 0);
            bytes_count += bytes_received;
            // writes those bytes to a file
            fwrite(buffer, 1, bytes_received, file);
        } while (bytes_received == 2048);
        // if less than 2048 bytes, there aren't enough bytes
        // for another packet, must be checked because server
        // crashes if it receives 0 bytes

        // sends how many bytes server read to client
        send(transfer_socket, itoa(bytes_count, buffer, 10), 4+1, 0);
        printf("Successfully received %d bytes, saved to file %s\n", bytes_count, filepath);

        closesocket(transfer_socket);
        fclose(file);
    }
    
    exit(0);
}

void close_connections() {

    if (file != NULL)
        fclose(file);
    // Windows only
    closesocket(socketfd);
    WSACleanup();
}