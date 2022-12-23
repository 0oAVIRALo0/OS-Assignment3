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

    struct timespec start,end;
    clock_gettime(CLOCK_REALTIME, &start);

    char buffer[(BUFFER_SIZE + 4) * GROUP_SIZE + 1];
    for (i = 0; i < TOTAL_STRINGS; i+=5) {
        memset(buffer, 0, sizeof(buffer));

        int fd = open("myfifo", O_WRONLY);

        for (int j = i; j < i + GROUP_SIZE; j++) {
            char index_str[8];
            sprintf(index_str, " %d_", j+1);
            strcat(buffer, index_str); 
            strcat(buffer, strings[j]);
        }
        
        int data = write(fd, &buffer, sizeof(buffer));

        close(fd);
    
        fd = open("myfifo", O_RDONLY);
        int max_index;
        int x = read(fd, &max_index, sizeof(max_index));

        close(fd);
    }

    clock_gettime(CLOCK_REALTIME, &end);
    double time = (end.tv_sec - start.tv_sec) +(end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Time taken: %f: \n",time);

    return 0;    
}
