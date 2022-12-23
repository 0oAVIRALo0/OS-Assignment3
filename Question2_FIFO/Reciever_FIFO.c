#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define TOTAL_STRINGS 50
#define BUFFER_SIZE 5
#define GROUP_SIZE 5

int main(){
    int pt=0;
    while (pt<10){
        char buffer[50];
        int fd = open("myfifo", O_RDONLY);
        if (read(fd, &buffer, sizeof(buffer)) < 0) {
            perror("Error while reading from FIFO.\n");
            exit(1);
        }
        close(fd);

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

        fd = open("myfifo", O_WRONLY);

        int x=write(fd, &max_index, sizeof(max_index));
        pt++;
        close(fd);
    }

    return 0;
}
