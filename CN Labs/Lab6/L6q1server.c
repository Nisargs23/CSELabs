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
    int sockfd, newsockfd, clilen, n;
    char buf[256];
    struct sockaddr_in seraddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("172.16.48.148"); // Change to your server IP
    seraddr.sin_port = htons(PORTNO);

    bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr));
    listen(sockfd, 5);

    while (1) {
        clilen = sizeof(cliaddr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);

        if (fork() == 0) { // Child process
            close(sockfd);
            n = read(newsockfd, buf, sizeof(buf));
            buf[n] = '\0';

            int num1, num2;
            char op;
            sscanf(buf, "%d %c %d", &num1, &op, &num2);

            double result;
            int valid = 1;

            switch (op) {
                case '+': result = num1 + num2; break;
                case '-': result = num1 - num2; break;
                case '*': result = num1 * num2; break;
                case '/':
                    if (num2 != 0) result = (double)num1 / num2;
                    else valid = 0;
                    break;
                default: valid = 0;
            }

            char res_str[256];
            if (valid)
                sprintf(res_str, "Result: %.2f", result);
            else
                sprintf(res_str, "Error: Invalid operation");

            // ✅ Added line to acknowledge what is being sent
            printf("Sending answer: %s\n", res_str);

            write(newsockfd, res_str, strlen(res_str));
            close(newsockfd);
            exit(0);
        } else {
            close(newsockfd); // Parent closes client socket
        }
    }
}

