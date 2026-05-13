// gcc Server.c -lws2_32 -o server.exe
// ./server.exe
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef int socklen_t;
#define CLOSESOCKET closesocket
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define CLOSESOCKET close
#endif

#define IP_PROTOCOL 0
#define PORT_NO 15050
#define NET_BUF_SIZE 32
#define cipherKey 'S'
#define sendrecvflag 0
#define nofile "File Not Found!"

void clearBuf(char *b) {
    for (int i = 0; i < NET_BUF_SIZE; i++)
        b[i] = '\0';
}

char Cipher(char ch) {
    return ch ^ cipherKey;
}

int sendFile(FILE *fp, char *buf, int s) {
    if (fp == NULL) {
        strcpy(buf, nofile);
        int len = strlen(nofile);
        buf[len] = EOF;
        for (int i = 0; i <= len; i++)
            buf[i] = Cipher(buf[i]);
        return 1;
    }

    for (int i = 0; i < s; i++) {
        char ch = fgetc(fp);
        buf[i] = Cipher(ch);

        if (ch == EOF)
            return 1;
    }
    return 0;
}

int main() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("\nWSAStartup failed!\n");
        return 0;
    }
#endif

#ifdef _WIN32
    SOCKET sockfd;
#else
    int sockfd;
#endif
    int nBytes;
    struct sockaddr_in addr_con;
    socklen_t addrlen = sizeof(addr_con);

    addr_con.sin_family = AF_INET;
    addr_con.sin_port = htons(PORT_NO);
    addr_con.sin_addr.s_addr = INADDR_ANY;

    char net_buf[NET_BUF_SIZE];
    FILE *fp;

    sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);

#ifdef _WIN32
    if (sockfd == INVALID_SOCKET)
        printf("\nFile descriptor not received!\n");
    else
        printf("\nFile descriptor %llu received\n", (unsigned long long)sockfd);
#else
    if (sockfd < 0)
        printf("\nFile descriptor not received!\n");
    else
        printf("\nFile descriptor %d received\n", sockfd);
#endif

    if (bind(sockfd, (struct sockaddr *)&addr_con, sizeof(addr_con)) == 0)
        printf("\nSuccessfully binded!\n");
    else
        printf("\nBinding Failed!\n");

    while (1) {
        printf("\nWaiting for file name...\n");

        clearBuf(net_buf);

        nBytes = recvfrom(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag,
                          (struct sockaddr *)&addr_con, &addrlen);

        printf("\nFile Name Received: %s\n", net_buf);

        fp = fopen(net_buf, "r");

        if (fp == NULL)
            printf("\nFile open failed!\n");
        else
            printf("\nFile Successfully opened!\n");

        while (1) {
            if (sendFile(fp, net_buf, NET_BUF_SIZE)) {
                sendto(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag,
                       (struct sockaddr *)&addr_con, addrlen);
                break;
            }

            sendto(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag,
                   (struct sockaddr *)&addr_con, addrlen);

            clearBuf(net_buf);
        }

        if (fp != NULL)
            fclose(fp);
    }

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}