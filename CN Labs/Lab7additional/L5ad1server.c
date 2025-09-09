// server.c
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define PORTNO 10215
#define SERVER_IP "172.16.48.148"

int main() {
    int sockfd, newsockfd, clilen;
    struct sockaddr_in seraddr, cliaddr;
    char buf[256], decrypted[256];


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }


    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    seraddr.sin_port = htons(PORTNO);

    if (bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

  
    listen(sockfd, 5);
    printf("Server waiting for connection...\n");

    clilen = sizeof(cliaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, (socklen_t *)&clilen);
    if (newsockfd < 0) {
        perror("Accept failed");
        close(sockfd);
        exit(1);
    }


    memset(buf, 0, sizeof(buf));
    int n = read(newsockfd, buf, sizeof(buf) - 1);
    if (n > 0) {
        printf("Encrypted message from Client: %s\n", buf);

 
        strcpy(decrypted, buf);
        for (int i = 0; decrypted[i] != '\0'; i++) {
            decrypted[i] = decrypted[i] - 4;
        }

        printf("Decrypted message from Client: %s\n", decrypted);

        write(newsockfd, decrypted, strlen(decrypted));
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}

