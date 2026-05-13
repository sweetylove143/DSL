// gcc Client.c -lws2_32 -o client.exe
// ./client.exe
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
#define CHUNK_SIZE 4096
#define FILENAME_MAX_LEN 256

static int send_all(
#ifdef _WIN32
    SOCKET sockfd,
#else
    int sockfd,
#endif
    const char *buf, int len) {
    int total = 0;
    while (total < len) {
        int sent = send(sockfd, buf + total, len - total, 0);
        if (sent <= 0) {
            return -1;
        }
        total += sent;
    }
    return 0;
}

static int recv_all(
#ifdef _WIN32
    SOCKET sockfd,
#else
    int sockfd,
#endif
    char *buf, int len) {
    int total = 0;
    while (total < len) {
        int recvd = recv(sockfd, buf + total, len - total, 0);
        if (recvd <= 0) {
            return -1;
        }
        total += recvd;
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
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

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

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
        printf("Connection to server failed!\n");
        CLOSESOCKET(sockfd);
#ifdef _WIN32
        WSACleanup();
#endif
        return 0;
    }

    char filename[FILENAME_MAX_LEN];
    printf("Enter file name to request: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Invalid input.\n");
        CLOSESOCKET(sockfd);
#ifdef _WIN32
        WSACleanup();
#endif
        return 0;
    }

    size_t name_len = strcspn(filename, "\r\n");
    filename[name_len] = '\0';

    if (name_len == 0) {
        printf("File name cannot be empty.\n");
        CLOSESOCKET(sockfd);
#ifdef _WIN32
        WSACleanup();
#endif
        return 0;
    }

    uint32_t net_len = htonl((uint32_t)name_len);
    if (send_all(sockfd, (const char *)&net_len, (int)sizeof(net_len)) != 0
        || send_all(sockfd, filename, (int)name_len) != 0) {
        printf("Failed to send request.\n");
        CLOSESOCKET(sockfd);
#ifdef _WIN32
        WSACleanup();
#endif
        return 0;
    }

    uint32_t net_status = 0;
    if (recv_all(sockfd, (char *)&net_status, (int)sizeof(net_status)) != 0) {
        printf("Failed to read server response.\n");
        CLOSESOCKET(sockfd);
#ifdef _WIN32
        WSACleanup();
#endif
        return 0;
    }

    int status = (int)ntohl(net_status);
    if (status != 0) {
        printf("Server response: file not found.\n");
        CLOSESOCKET(sockfd);
#ifdef _WIN32
        WSACleanup();
#endif
        return 0;
    }

    uint32_t net_size = 0;
    if (recv_all(sockfd, (char *)&net_size, (int)sizeof(net_size)) != 0) {
        printf("Failed to read file size.\n");
        CLOSESOCKET(sockfd);
#ifdef _WIN32
        WSACleanup();
#endif
        return 0;
    }

    uint32_t file_size = ntohl(net_size);
    printf("Receiving file (%u bytes)...\n", file_size);

    FILE *fp = fopen("received.txt", "wb");
    if (fp == NULL) {
        printf("Failed to open output file.\n");
        CLOSESOCKET(sockfd);
#ifdef _WIN32
        WSACleanup();
#endif
        return 0;
    }

    uint32_t remaining = file_size;
    char buffer[CHUNK_SIZE];
    while (remaining > 0) {
        int to_read = (remaining > CHUNK_SIZE) ? CHUNK_SIZE : (int)remaining;
        int recvd = recv(sockfd, buffer, to_read, 0);
        if (recvd <= 0) {
            printf("Connection lost while receiving file.\n");
            break;
        }
        fwrite(buffer, 1, recvd, fp);
        remaining -= (uint32_t)recvd;
    }

    fclose(fp);
    printf("File saved to received.txt\n");

    CLOSESOCKET(sockfd);

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

/*
Steps to run:
1) gcc Server.c -lws2_32 -o server.exe
2) gcc Client.c -lws2_32 -o client.exe
3) ./server.exe (keep this running), then in another terminal run: ./client.exe

Input examples:
1) Request: Test.c -> output file received.txt created
2) Request: missing.txt -> output "Server response: file not found."

Expected output:
1) Receiving file (N bytes)... File saved to received.txt
2) Server response: file not found.
*/