#include <stdio.h>
#include <winsock2.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

void receiveFile(FILE *fp, SOCKET sockfd);

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    FILE *fp;
    char *filename = "receivedFile.txt";

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT);

    connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    fp = fopen(filename, "wb");
    if (!fp) {
        perror("Failed to open the file");
        exit(1);
    }

    receiveFile(fp, clientSocket);
    printf("File received successfully.\n");

    fclose(fp);
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}

void receiveFile(FILE *fp, SOCKET sockfd) {
    int n;
    char buffer[BUFFER_SIZE];

    while ((n = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, sizeof(char), n, fp);
    }
}