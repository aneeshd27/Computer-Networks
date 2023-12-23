#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

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

    SOCKET serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    char buffer[BUFSIZE];

    // Create socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET)
        error("Error opening socket");

    // Initialize server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        error("Error binding socket");

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        int bytesRead = recvfrom(serverSocket, buffer, BUFSIZE, 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (bytesRead == SOCKET_ERROR)
            error("Error receiving data");

        buffer[bytesRead] = '\0'; // Null-terminate the received data

        printf("Client (%s:%d): %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buffer);

        // Echo the message back to the client
        if (sendto(serverSocket, buffer, bytesRead, 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR)
            error("Error sending data back to client");
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
