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
    int pt=0;
    while (pt<10){
        key_t uniqueKey = ftok(".", 'a');
        int shmid = shmget(uniqueKey, (BUFFER_SIZE + 4) * GROUP_SIZE + 1, 0666);
        if (shmid < 0) {
            perror("Error getting shared memory segment.\n");
            exit(1);
        }

        char* buffer = (char*) shmat(shmid, NULL, 0);
        if (buffer == (char*) -1) {
            perror("Error attaching shared memory segment.\n");
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

        shmdt(buffer);

        pt++;
    }

    return 0;
}
