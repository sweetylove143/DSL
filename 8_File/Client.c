// gcc Client.c -lws2_32 -o client.exe
// ./client.exe
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
#include <unistd.h>
#define CLOSESOCKET close
#endif

#define PORT_NO 15050
#define NET_BUF_SIZE 32
#define cipherKey 'S'
#define sendrecvflag 0

void clearBuf(char *b) {
    for (int i = 0; i < NET_BUF_SIZE; i++)
        b[i] = '\0';
}

char Cipher(char ch) {
    return ch ^ cipherKey;
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
    struct sockaddr_in server_addr;
    socklen_t addrlen = sizeof(server_addr);

    char net_buf[NET_BUF_SIZE];
    FILE *fp;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

#ifdef _WIN32
    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed!\n");
        WSACleanup();
        return 0;
    }
#else
    if (sockfd < 0) {
        printf("Socket creation failed!\n");
        return 0;
    }
#endif

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NO);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter file name to request: ");
    scanf("%s", net_buf);

    sendto(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag,
           (struct sockaddr *)&server_addr, addrlen);

    printf("Receiving file...\n");

    fp = fopen("received.txt", "w");

    while (1) {
        clearBuf(net_buf);

        nBytes = recvfrom(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag,
                          (struct sockaddr *)&server_addr, &addrlen);

        for (int i = 0; i < NET_BUF_SIZE; i++)
            net_buf[i] = Cipher(net_buf[i]);

        if (net_buf[0] == EOF)
            break;

        fprintf(fp, "%s", net_buf);
    }

    printf("File received successfully.\n");

    fclose(fp);
    CLOSESOCKET(sockfd);

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}