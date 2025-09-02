#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdlib.h>

#define PORTNO 10215

// Function to remove duplicates and count them
int dedup(char *line) {
    char *words[100];
    int count = 0;
    int duplicates = 0;

    char temp[256];
    strcpy(temp, line); // Work on a copy so we don't lose data

    char *tok = strtok(temp, " \t\n");
    while (tok) {
        int seen = 0;
        for (int i = 0; i < count; i++) {
            if (strcasecmp(words[i], tok) == 0) {
                seen = 1;
                duplicates++;
                break;
            }
        }
        if (!seen) {
            words[count++] = tok;
        }
        tok = strtok(NULL, " \t\n");
    }

    // Rebuild the sentence from the unique words
    line[0] = '\0';
    for (int i = 0; i < count; i++) {
        strcat(line, words[i]);
        if (i < count - 1) strcat(line, " ");
    }

    return duplicates;
}


int main() {
    int sockfd, newsockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); exit(1); }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORTNO);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(1);
    }

    if (listen(sockfd, 5) < 0) {
        perror("listen");
        close(sockfd);
        exit(1);
    }

    printf("Server listening on port %d...\n", PORTNO);

    while (1) {
        clilen = sizeof(cliaddr);
        newsockfd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen);
        if (newsockfd < 0) {
            perror("accept");
            continue;
        }

        if (fork() == 0) { // Child process
            close(sockfd); // Child doesn't need listening socket
            char buf[256];
            int n = read(newsockfd, buf, sizeof(buf) - 1);
            if (n <= 0) {
                close(newsockfd);
                exit(0);
            }
            buf[n] = '\0';

            int dup_count = dedup(buf);

            char sendbuf[300];
            snprintf(sendbuf, sizeof(sendbuf),
                     "Result: %s\nDuplicates removed: %d\n",
                     buf, dup_count);

            write(newsockfd, sendbuf, strlen(sendbuf));

            close(newsockfd);
            exit(0);
        }
        close(newsockfd); // Parent closes connected socket
    }

    close(sockfd);
    return 0;
}

