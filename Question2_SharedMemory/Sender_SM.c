#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define TOTAL_STRINGS 50
#define BUFFER_SIZE 5
#define GROUP_SIZE 5

int main() {
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

    key_t uniqueKey = ftok(".", 'a');
    int shmid = shmget(uniqueKey, (BUFFER_SIZE + 4) * GROUP_SIZE + 1, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("Error creating shared memory segment.\n");
        exit(1);
    }

    struct timespec start,end;
    clock_gettime(CLOCK_REALTIME, &start);

    for (i = 0; i < TOTAL_STRINGS; i+=5) {
        char* buffer = (char*) shmat(shmid, NULL, 0);
        if (buffer == (char*) -1) {
            perror("Error attaching shared memory segment.\n");
            exit(1);
        }

        memset(buffer, 0, (BUFFER_SIZE + 4) * GROUP_SIZE + 1);

        for (int j = i; j < i + GROUP_SIZE; j++) {
            char index_str[8];
            sprintf(index_str, " %d_", j+1);
            strcat(buffer, index_str); 
            strcat(buffer, strings[j]);
        }

        shmdt(buffer);
    }

    clock_gettime(CLOCK_REALTIME, &end);
    double time = (end.tv_sec - start.tv_sec) +(end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Time taken: %f: \n",time);

    return 0;
}
