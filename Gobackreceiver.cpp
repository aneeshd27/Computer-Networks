#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#define W 5//Frame size 
#define P1 50//Probability of packet loss
#define P2 10

char a[10];
char b[10];

void alpha9(int);

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {//It initializes the startup with WSA if does not start failed and end 
        printf("Failed to initialize Winsock\n");
        return 1;
    }

    struct sockaddr_in ser, cli;
    int s, n, sock, i, j, c = 1, f;
    unsigned int s1;
    s = socket(AF_INET, SOCK_STREAM, 0);
    ser.sin_family = AF_INET;//IPV4 family
    ser.sin_port = htons(6500);//Convert port number to byte type order
    ser.sin_addr.s_addr = inet_addr("127.0.0.1");//loopback address -localhost
    bind(s, (struct sockaddr*)&ser, sizeof(ser));//binds socket s to server address
    listen(s, 1);//listens and looks at the acknowledgements
    n = sizeof(cli);
    sock = accept(s, (struct sockaddr*)&cli, &n);//accepts new communication and creates new socket
    printf("\nTCP Connection Established.\n");
    s1 = (unsigned int)time(NULL);
    srand(s1);
    strcpy(b, "Time Out ");
    recv(sock, a, sizeof(a), 0);
    f = atoi(a);

    while (1) {
        for (i = 0; i < W; i++) {
            recv(sock, a, sizeof(a), 0);
            if (strcmp(a, b) == 0) {//checks if message is time out else exit
                break;
            }
        }
        i = 0;
        while (i < W) {
            j = rand() % P1;
            if (j < P2) {//simulates packet loss
                send(sock, b, sizeof(b), 0);
                break;
            } else {
                alpha9(c);
                if (c <= f) {
                    printf("\nFrame %s was succesfully Received ", a);
                    send(sock, a, sizeof(a), 0);
                } else {
                    break;
                }
                c++;
            }
            if (c > f) {
                break;
            }
            i++;
        }
    }

    closesocket(sock);
    closesocket(s);
    WSACleanup();
    return 0;
}

void alpha9(int z) {
    int k, i = 0, j, g;
    k = z;
    while (k > 0) {
        i++;
        k = k / 10;
    }
    g = i;
    i--;
    while (z > 0) {
        k = z % 10;
        a[i] = k + 48;
        i--;
        z = z / 10;
    }
    a[g] = '\0';
}