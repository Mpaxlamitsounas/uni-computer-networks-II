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
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 127.0.0.1 is loopback address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080); // server on port 8080

    // reads location of the image file the user wants to send
    char filepath[512+1];
    puts("Enter image file to be sent:");
    fgets(filepath, 512, stdin);
    filepath[strlen(filepath)-1] = '\0'; // removes newline from end of string

    // opens file to be sent before attempting connection
    file = fopen(filepath, "rb");
    if (file == NULL) {
        puts("Failed to open file");
        exit(1);
    }

    // connects to the server
    int result = connect(   socketfd,
                            (struct sockaddr *) &serv_addr, // server details
                            sizeof(serv_addr)); // length of server details
    if (result < 0) {
        puts("Connection failed");
        exit(1);
    }

    char buffer[2048];
    int bytes_count = 0, bytes_read = 0;

    // checks if file is empty before sending
    // server crashes if it receives 0 bytes packet
    bytes_read = fread(buffer, 1, sizeof(buffer), file);
    if (bytes_read == 0) {
        puts("Cannot send empty file");
        exit(1);
    }
    bytes_count += bytes_read;
    do {
       
        // sends the bytes from fread to server
        result = send(socketfd, buffer, bytes_read, 0);
        if (result < 0) {
            puts("Failed to send data");
            exit(1);
        }

        // reads up to 2048 bytes from file to buffer at time
        bytes_read = fread(buffer, 1, sizeof(buffer), file);
        bytes_count += bytes_read;
        
    } while (bytes_read != 0);

    // server responds with number of bytes read
    result = recv(socketfd, buffer, 2048, 0);
    if (result < 0) {
        puts("Failed to get reply from server");
        exit(1);
    }

    printf("Sent %d bytes\n", bytes_count);
    printf("Server received %d bytes\n", atoi(buffer));
    
    exit(0);

}

void close_connections() {

    if (file != NULL)
        fclose(file);
    // Windows only
    closesocket(socketfd);
    WSACleanup();
}