// http_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 80
#define BUFFER_SIZE 4096

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char buffer[BUFFER_SIZE];
    char request[1024];
    char hostname[256];
    char resource[256] = "/";
    
    // Get hostname from user
    printf("Enter hostname (e.g., example.com): ");
    scanf("%255s", hostname);
    
    // Optional: get resource path
    printf("Enter resource path (default '/'): ");
    getchar(); // consume newline
    fgets(resource, sizeof(resource), stdin);
    resource[strcspn(resource, "\n")] = '\0';
    if (strlen(resource) == 0) strcpy(resource, "/");

    // Resolve hostname to IP
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr, "ERROR: No such host\n");
        exit(1);
    }

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Prepare server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    server_addr.sin_port = htons(PORT);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("ERROR connecting");
        close(sockfd);
        exit(1);
    }

    // Craft HTTP GET request manually
    snprintf(request, sizeof(request),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "User-Agent: CustomClient/1.0\r\n"
             "Accept: */*\r\n"
             "Connection: close\r\n"
             "\r\n",
             resource, hostname);

    // Send request
    if (write(sockfd, request, strlen(request)) < 0) {
        perror("ERROR writing to socket");
        close(sockfd);
        exit(1);
    }

    // Read and display full HTTP response
    printf("\n--- HTTP Response ---\n");
    int bytes_read;
    while ((bytes_read = read(sockfd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }

    if (bytes_read < 0) {
        perror("ERROR reading from socket");
    }

    close(sockfd);
    return 0;
}

