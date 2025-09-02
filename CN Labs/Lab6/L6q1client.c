#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int len, result, sockfd, n;
    struct sockaddr_in address;
    char send_buf[256], recv_buf[256];
    int num1, num2;
    char op;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("172.16.48.148"); // Change to server IP
    address.sin_port = htons(10200);
    len = sizeof(address);

    result = connect(sockfd, (struct sockaddr *)&address, len);
    if (result == -1) {
        perror("CLIENT ERROR");
        exit(1);
    }

    printf("Enter first integer: ");
    scanf("%d", &num1);
    printf("Enter operator (+, -, *, /): ");
    scanf(" %c", &op);
    printf("Enter second integer: ");
    scanf("%d", &num2);

    sprintf(send_buf, "%d %c %d", num1, op, num2);
    write(sockfd, send_buf, strlen(send_buf));

    n = read(sockfd, recv_buf, sizeof(recv_buf));
    recv_buf[n] = '\0';
    printf("Server Response: %s\n", recv_buf);

    close(sockfd);
    return 0;
}

