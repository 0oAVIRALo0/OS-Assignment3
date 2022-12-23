#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>

#define TOTAL_STRINGS 50
#define BUFFER_SIZE 5
#define GROUP_SIZE 5

int main(){
    char strings[TOTAL_STRINGS][BUFFER_SIZE + 1];
    char alphabets[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int i;
    for (i = 0; i < TOTAL_STRINGS; i++) {
        int j;
        for (j = 0; j < BUFFER_SIZE; j++) {
            strings[i][j] = alphabets[rand() % (int) (sizeof(alphabets) - 1)];
        }
        strings[i][BUFFER_SIZE] = '\0';
    }   

    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error while creating socket.\n");
        exit(1);
    } else {
        printf("Socket created successfully.\n");
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "mysocket", sizeof(addr.sun_path) - 1);

    int ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));
    if (ret < 0) {
        perror("Error connecting socket.\n");
        exit(1);
    }

    struct timespec start,end;
    clock_gettime(CLOCK_REALTIME, &start);

    char buffer[(BUFFER_SIZE + 4) * GROUP_SIZE + 1];
    for (i = 0; i < TOTAL_STRINGS; i+=5) {
        memset(buffer, 0, sizeof(buffer));

        for (int j = i; j < i + GROUP_SIZE; j++) {
            char index_str[8];
            sprintf(index_str, " %d_", j+1);
            strcat(buffer, index_str); 
            strcat(buffer, strings[j]);
        }
        
        int data = write(sockfd, &buffer, sizeof(buffer));
        if (data < 0) {
            perror("Error writing data.\n");
            exit(1);
        }

        int max_index;
        int x = read(sockfd, &max_index, sizeof(max_index));
        if (x < 0) {
            perror("Error reading data.\n");
            exit(1);
        }
    }

    clock_gettime(CLOCK_REALTIME, &end);
    double time = (end.tv_sec - start.tv_sec) +(end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Time taken: %f: \n",time);

    close(sockfd);

    return 0;
}
