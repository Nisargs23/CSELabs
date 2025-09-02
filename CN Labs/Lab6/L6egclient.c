#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTNO 10200

int main() {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    char buf[256];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Server address setup
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORTNO);
    serv_addr.sin_addr.s_addr = inet_addr("172.16.59.10"); // Server IP

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(1);
    }

    // Get message from user
    printf("Enter message: ");
    fgets(buf, sizeof(buf), stdin);

    // Send to server
    n = write(sockfd, buf, sizeof(buf));
    if (n < 0) {
        perror("Error writing to socket");
        close(sockfd);
        exit(1);
    }

    // Read reply from server
    n = read(sockfd, buf, sizeof(buf));
    if (n < 0) {
        perror("Error reading from socket");
        close(sockfd);
        exit(1);
    }

    printf("Message from server: %s\n", buf);

    close(sockfd);
    return 0;
}

