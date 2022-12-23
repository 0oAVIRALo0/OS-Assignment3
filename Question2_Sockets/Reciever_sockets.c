#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define TOTAL_STRINGS 50
#define BUFFER_SIZE 5
#define GROUP_SIZE 5

int main(){
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error while creating socket.\n");
        exit(1);
    } else {
        printf("Socket created successfully.\n");
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "mysocket", sizeof(addr.sun_path) - 1);

    int ret = bind(sockfd, (const struct sockaddr *) &addr, sizeof(addr));
    if (ret < 0) {
        perror("Error binding socket.\n");
        exit(1);
    }

    if (listen(sockfd, 50) < 0) {
        perror("Error listening on socket.\n");
        exit(1);
    }

    int client_sockfd;
    client_sockfd = accept(sockfd, NULL, NULL);
    if (client_sockfd < 0) {
        perror("Error accepting connection.\n");
        exit(1);
    }

    int pt = 0;
    while (pt<10){
        char buffer[50];
        int x = read(client_sockfd, &buffer, sizeof(buffer));
        if (x < 0) {
            perror("Error receiving data.\n");
            exit(1);
        }

        char *token;
        int max_index = 0;
        puts(buffer);
        token = strtok(buffer, " ");
        while (token != NULL) {
            int index = atoi(token);
            if (index > max_index) {
                max_index = index;
            }
            token = strtok(NULL, " ");
        }
        printf("Maximum index: %d\n", max_index);

        int y = write(client_sockfd, &max_index, sizeof(max_index));
        if (y < 0) {
            perror("Error sending data.\n");
            exit(1);
        }
        pt++;
    }
    
    close(client_sockfd);

    close(sockfd);
    
    return 0;
}
