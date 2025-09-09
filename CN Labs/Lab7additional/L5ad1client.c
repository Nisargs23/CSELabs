// client.c
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "172.16.48.148"
#define SERVER_PORT 10215

int main() {
    int sockfd, len, result;
    struct sockaddr_in address;
    char ch[256], buf[256];


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }


    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(SERVER_IP);
    address.sin_port = htons(SERVER_PORT);
    len = sizeof(address);

 
    result = connect(sockfd, (struct sockaddr *)&address, len);
    if (result == -1) {
        perror("CLIENT ERROR");
        exit(1);
    }


    printf("ENTER STRING: ");
    if (fgets(ch, sizeof(ch), stdin) == NULL) {
        perror("Input error");
        close(sockfd);
        exit(1);
    }
    ch[strcspn(ch, "\n")] = '\0'; // Remove newline

  
    for (int i = 0; ch[i] != '\0'; i++) {
        ch[i] = ch[i] + 4;
    }


    write(sockfd, ch, strlen(ch));


    memset(buf, 0, sizeof(buf));
    int n = read(sockfd, buf, sizeof(buf) - 1);
    if (n > 0) {
        printf("STRING SENT BACK FROM SERVER IS: %s\n", buf);
    }

    close(sockfd);
    return 0;
}

