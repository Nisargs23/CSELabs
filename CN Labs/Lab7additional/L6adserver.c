// daytime_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTNO 10200

int main() {
    int sockfd, newsockfd, clilen;
    struct sockaddr_in seraddr, cliaddr;


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }


    memset(&seraddr, 0, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("172.16.48.148"); 
    seraddr.sin_port = htons(PORTNO);


    if (bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) < 0) {
        perror("Listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Daytime server running on %s:%d\n", inet_ntoa(seraddr.sin_addr), PORTNO);

    while (1) {
        clilen = sizeof(cliaddr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, (socklen_t *)&clilen);
        if (newsockfd < 0) {
            perror("Accept failed");
            continue;
        }


        if (fork() == 0) {
            close(sockfd); 

            // Get current time
            time_t now = time(NULL);
            char *time_str = ctime(&now);
            time_str[strcspn(time_str, "\n")] = '\0'; // Remove newline

   
            pid_t pid = getpid();

         
            char send_buf[256];
            snprintf(send_buf, sizeof(send_buf),
                     "Server PID: %d\nCurrent Date/Time: %s\n", pid, time_str);

  
            write(newsockfd, send_buf, strlen(send_buf));

            printf("Sent to client %s:%d -> %s\n",
                   inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), send_buf);

            close(newsockfd);
            exit(0); 
        } else {
            close(newsockfd); 
        }
    }

    close(sockfd);
    return 0;
}

