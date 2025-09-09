// daytime_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORTNO 10200

int main() {
    int sockfd, n;
    struct sockaddr_in server_addr;
    char recv_buf[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("172.16.48.148"); 
    server_addr.sin_port = htons(PORTNO);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    n = read(sockfd, recv_buf, sizeof(recv_buf) - 1);
    if (n > 0) {
        recv_buf[n] = '\0';
        printf("Server Response:\n%s\n", recv_buf);
    }

    close(sockfd);
    return 0;
}

