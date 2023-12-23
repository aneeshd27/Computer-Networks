#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFSIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        error("Error initializing Winsock");
    }

    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFSIZE];

    // Create socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET)
        error("Error opening socket");

    // Initialize server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(PORT);

    printf("Enter messages to send (type 'exit' to quit):\n");

    while (1) {
        // Read user input
        printf("You: ");
        fgets(buffer, BUFSIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove trailing newline

        // Send the message to the server
        if (sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
            error("Error sending data to server");

        if (strcmp(buffer, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }

        // Receive the echoed message from the server
        int bytesRead = recvfrom(clientSocket, buffer, BUFSIZE, 0, NULL, NULL);
        if (bytesRead == SOCKET_ERROR)
            error("Error receiving data from server");

        buffer[bytesRead] = '\0'; // Null-terminate the received data

        printf("Server: %s\n", buffer);
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}

