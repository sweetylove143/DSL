#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define IP_PROTOCOL 0
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
    int sockfd, nBytes;
    struct sockaddr_in addr_con;
    int addrlen = sizeof(addr_con);

    addr_con.sin_family = AF_INET;
    addr_con.sin_port = htons(PORT_NO);
    addr_con.sin_addr.s_addr = INADDR_ANY;

    char net_buf[NET_BUF_SIZE];
    FILE *fp;

    sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);

    if (sockfd < 0) {
        printf("\nFile descriptor not received!\n");
        exit(0);
    } else {
        printf("\nFile descriptor %d received\n", sockfd);
    }

    printf("\nEnter file name to request: ");
    scanf("%s", net_buf);

    sendto(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag,
           (struct sockaddr *)&addr_con, addrlen);

    printf("\nReceiving file...\n");

    fp = fopen("output.txt", "w");

    while (1) {
        clearBuf(net_buf);

        nBytes = recvfrom(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag,
                          (struct sockaddr *)&addr_con, &addrlen);

        for (int i = 0; i < NET_BUF_SIZE; i++)
            net_buf[i] = Cipher(net_buf[i]);

        if (net_buf[0] == EOF)
            break;

        fprintf(fp, "%s", net_buf);
    }

    printf("\nFile received successfully.\n");

    fclose(fp);
    close(sockfd);

    return 0;
}