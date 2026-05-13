// gcc Server.c -lws2_32 -o server.exe
// ./server.exe
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
#include <sys/types.h>
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
    SOCKET server_sock;
#else
    int server_sock;
#endif
    struct sockaddr_in addr_con;

    addr_con.sin_family = AF_INET;
    addr_con.sin_port = htons(PORT_NO);
    addr_con.sin_addr.s_addr = INADDR_ANY;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

#ifdef _WIN32
    if (server_sock == INVALID_SOCKET)
        printf("\nSocket creation failed!\n");
    else
        printf("\nSocket created: %llu\n", (unsigned long long)server_sock);
#else
    if (server_sock < 0)
        printf("\nSocket creation failed!\n");
    else
        printf("\nSocket created: %d\n", server_sock);
#endif

    if (bind(server_sock, (struct sockaddr *)&addr_con, sizeof(addr_con)) == 0)
        printf("Server listening on port %d\n", PORT_NO);
    else
        printf("Binding failed!\n");

    if (listen(server_sock, 5) != 0) {
        printf("Listen failed!\n");
        CLOSESOCKET(server_sock);
#ifdef _WIN32
        WSACleanup();
#endif
        return 0;
    }

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

#ifdef _WIN32
        SOCKET client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock == INVALID_SOCKET) {
            printf("Accept failed!\n");
            continue;
        }
#else
        int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock < 0) {
            printf("Accept failed!\n");
            continue;
        }
#endif

        uint32_t net_len = 0;
        if (recv_all(client_sock, (char *)&net_len, (int)sizeof(net_len)) != 0) {
            CLOSESOCKET(client_sock);
            continue;
        }

        uint32_t name_len = ntohl(net_len);
        if (name_len == 0 || name_len > FILENAME_MAX_LEN) {
            uint32_t status = htonl(1);
            send_all(client_sock, (const char *)&status, (int)sizeof(status));
            CLOSESOCKET(client_sock);
            continue;
        }

        char filename[FILENAME_MAX_LEN + 1];
        if (recv_all(client_sock, filename, (int)name_len) != 0) {
            CLOSESOCKET(client_sock);
            continue;
        }
        filename[name_len] = '\0';

        printf("Requested file: %s\n", filename);

        FILE *fp = fopen(filename, "rb");
        if (fp == NULL) {
            uint32_t status = htonl(1);
            send_all(client_sock, (const char *)&status, (int)sizeof(status));
            printf("File not found.\n");
            CLOSESOCKET(client_sock);
            continue;
        }

        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        if (file_size < 0) {
            uint32_t status = htonl(1);
            send_all(client_sock, (const char *)&status, (int)sizeof(status));
            fclose(fp);
            CLOSESOCKET(client_sock);
            continue;
        }

        uint32_t status = htonl(0);
        uint32_t net_size = htonl((uint32_t)file_size);
        if (send_all(client_sock, (const char *)&status, (int)sizeof(status)) != 0
            || send_all(client_sock, (const char *)&net_size, (int)sizeof(net_size)) != 0) {
            fclose(fp);
            CLOSESOCKET(client_sock);
            continue;
        }

        char buffer[CHUNK_SIZE];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, CHUNK_SIZE, fp)) > 0) {
            if (send_all(client_sock, buffer, (int)bytes_read) != 0) {
                break;
            }
        }

        fclose(fp);
        printf("File sent (%ld bytes).\n", file_size);
        CLOSESOCKET(client_sock);
    }

    CLOSESOCKET(server_sock);
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

Input examples (from client):
1) Test.c
2) missing.txt

Expected output (server console):
1) Requested file: Test.c, File sent (N bytes).
2) Requested file: missing.txt, File not found.
*/