#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORTNO 10215

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); exit(1); }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("172.16.48.148"); // Change to server IP
    servaddr.sin_port = htons(PORTNO);

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(1);
    }

    char buf[256], recvbuf[300];

    printf("Enter sentence: ");
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';

    write(sockfd, buf, strlen(buf));

    int n = read(sockfd, recvbuf, sizeof(recvbuf) - 1);
    if (n > 0) {
        recvbuf[n] = '\0';
        printf("Server:\n%s", recvbuf);
    }

    close(sockfd);
    return 0;
}

