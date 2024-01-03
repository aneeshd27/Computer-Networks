#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FRAME_SIZE 1024
#define MAX_WINDOW_SIZE 10

#pragma comment(lib, "ws2_32.lib")

void die(char *message) {
    perror(message);
    exit(1);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        die("WSAStartup failed");
    }

    SOCKET receiverSocket;
    receiverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (receiverSocket == INVALID_SOCKET) {
        die("Socket creation failed");
    }

    struct sockaddr_in receiverAddr;
    memset(&receiverAddr, 0, sizeof(receiverAddr));
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    receiverAddr.sin_port = htons(12345);

    if (bind(receiverSocket, (struct sockaddr *)&receiverAddr, sizeof(receiverAddr)) == SOCKET_ERROR) {
        die("Binding failed");
    }

    int expectedSeqNum = 0;
    char ack[MAX_FRAME_SIZE];
    struct sockaddr_in senderAddr;
    int senderAddrSize = sizeof(senderAddr);
    int done = 1;
    while (1) {
        char frame[MAX_FRAME_SIZE];
        int recvSize = recvfrom(receiverSocket, frame, sizeof(frame), 0, (struct sockaddr *)&senderAddr, &senderAddrSize);

        if (recvSize == SOCKET_ERROR) {
            die("Receive error");
        }

        frame[recvSize] = '\0';
        int frameNum = atoi(frame + 6); // Assuming "Frame " prefix in the frame data

        // if (frameNum == expectedSeqNum) {
            printf("Received Frame %d\n", frameNum);

            if (frameNum != 6 || done == 0) {
                // Simulate ACK transmission for all frames except frame number 6
                sprintf(ack, "%d", frameNum);
                sendto(receiverSocket, ack, strlen(ack), 0, (struct sockaddr *)&senderAddr, sizeof(senderAddr));
                expectedSeqNum++; 
            } else {
                done = 0;
            }

        // }
    }

    closesocket(receiverSocket);
    WSACleanup();
    return 0;
}