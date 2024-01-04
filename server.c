#include <stdio.h>
#include <winsock2.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

void sendFile(FILE *fp, SOCKET sockfd);

int main() {
    WSADATA wsaData;
    SOCKET listenSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    FILE *fp;
    char *filename = "Textfile1.txt";

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    listenSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    bind(listenSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(listenSocket, 5);

    int clientSize = sizeof(clientAddr);
    clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &clientSize);

    fp = fopen(filename, "rb");
    if (!fp) {
        perror("Failed to open the file");
        exit(1);
    }

    sendFile(fp, clientSocket);
    fflush(stdout);  // Flush the output buffer
    printf("File sent successfully.\n");

    fclose(fp);
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}

void sendFile(FILE *fp, SOCKET sockfd) {
    int n;
    char buffer[BUFFER_SIZE];

    while ((n = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
        send(sockfd, buffer, n, 0);
    }
}