#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FRAME_SIZE 1024
#define MAX_WINDOW_SIZE 10
#define TIMEOUT 5000 // 5 seconds

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

    SOCKET senderSocket;
    senderSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (senderSocket == INVALID_SOCKET) {
        die("Socket creation failed");
    }

    struct sockaddr_in receiverAddr;
    memset(&receiverAddr, 0, sizeof(receiverAddr));
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    receiverAddr.sin_port = htons(12345);

    int n, k;
    printf("Enter the number of frames to send (n): ");
    scanf("%d", &n);
    printf("Enter the window size (k): ");
    scanf("%d", &k);

    char frame[MAX_FRAME_SIZE];
    srand(time(NULL));

    int base = 0;
    int nextSeqNum = 0;

    while (base < n) {
        for (int i = base; i < base + k && i < n; i++) {
            // Simulate sending the frame
            sprintf(frame, "Frame %d", i);
            sendto(senderSocket, frame, strlen(frame), 0, (struct sockaddr *)&receiverAddr, sizeof(receiverAddr));
            printf("Sent Frame %d\n", i);
        }

        int acked = base;
        int maxAck = 0;
        while (acked < base + k && acked < n) {
            struct timeval timeout;
            timeout.tv_sec = TIMEOUT / 1000;
            timeout.tv_usec = (TIMEOUT % 1000) * 1000;

            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(senderSocket, &read_fds);

            int ready = select(senderSocket + 1, &read_fds, NULL, NULL, &timeout);

            if (ready == SOCKET_ERROR) {
                die("Select error");
            } else if (ready == 0) {
                // Timeout occurred, resend frames from base
                printf("Timeout occurred. Resending frame %d\n", base);
                sprintf(frame, "Frame %d", base);
                sendto(senderSocket, frame, strlen(frame), 0, (struct sockaddr *)&receiverAddr, sizeof(receiverAddr));
                printf("Sent Frame %d\n", base);
                nextSeqNum = base;
            } else {
                char ack[MAX_FRAME_SIZE];
                recvfrom(senderSocket, ack, sizeof(ack), 0, NULL, NULL);
                int ackNum = atoi(ack);
                printf("Received acknowledgment for Frame %d\n", ackNum);
                if(ackNum >= maxAck) {
                    maxAck = ackNum;
                } else {
                    acked = maxAck;
                    nextSeqNum = maxAck + 1;
                    break;
                }

                if (ackNum == nextSeqNum) {
                    nextSeqNum++;
                    acked++;
                }
            }
        }

        base = nextSeqNum;
    }

    closesocket(senderSocket);
    WSACleanup();
    return 0;
}