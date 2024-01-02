#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")//compiler specific directive for additional info 

#define W 5

char a[10];
char b[10];

void alpha9(int);

int main() {
    int s, f, wl, c = 1, i = 0, p = 0, e = 0;
    struct sockaddr_in ser;//Information about server address,information of IP address and port 
    WSADATA wsa;//store information about window sockets 

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {//Requesting 2.2 version of winstock
        printf("Failed to initialize Winsock\n");
        return 1;
    }

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        printf("Socket creation error\n");
        WSACleanup();
        return 1;
    }

    ser.sin_family = AF_INET;
    ser.sin_port = htons(6500);
    ser.sin_addr.s_addr = inet_addr("127.0.0.1");//converts IP address into 32 bit representation

    if (connect(s, (struct sockaddr*)&ser, sizeof(ser)) == SOCKET_ERROR) {
        printf("Connection error\n");
        closesocket(s);
        WSACleanup();
        return 1;
    }

    printf("TCP Connection Established.\n");
    printf("Enter the number of Frames: ");
    scanf("%d", &f);
    alpha9(f);
    send(s, a, sizeof(a), 0);
    strcpy(b, "Time Out ");

    while (1) {
        for (i = 0; i < W; i++) {//Send a batch of frames to the server
            alpha9(c);//Convert frame number into a string 
            send(s, a, sizeof(a), 0);
            if (c <= f) {
                printf("\nFrame %d Sent", c);
                c++;
            }
        }

        i = 0;
        wl = W;

        while (i < W) {//Receive acknowledgements from receiver
            recv(s, a, sizeof(a), 0);
            p = atoi(a);//Received string is converted into integer
            if (strcmp(a, b) == 0) {//Checks the timeout condition
                e = c - wl;//Calc range of frames that need to be sent again
                if (e < f) {
                    printf("\nTime Out, Resent Frame %d onwards", e);
                }
                break;
            } else {
                if (p <= f) {//p-Acknowledgement number
                    printf("\nFrame %s Acknowledged", a);
                    wl--;//Window size has been decremented
                } else {
                    break;
                }
            }

            if (p > f) {
                break;
            }
            i++;
        }

        if (wl == 0 && c > f) {
            send(s, b, sizeof(b), 0);
            break;
        } else {
            c = c - wl;
            wl = W;
        }
    }

    closesocket(s);//cleans up Winsock resources 
    WSACleanup();
    return 0;
}

void alpha9(int z) {// Converting integer z into char array a
    int k, i = 0, j, g;
    k = z;
    while (k > 0) {
        i++;
        k = k / 10;
    }
    g = i;//No of digits
    i--;
    while (z > 0) {
        k = z % 10;
        a[i] = k + 48;//Convert it to the ASCII character
        i--;
        z = z / 10;
    }
    a[g] = '\0';
}