#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define NUM_Philosophers 5
#define NUM_Forks 5
#define NUM_SAUCE_BOWLS 2

int forks[NUM_Forks] = {0};
int sauceBowls[NUM_SAUCE_BOWLS] = {0};

void philosopherIsThinking(int num);
void philosopherIsEating(int num, int s);

void *philosopher(void *arg) {
    int id = *((int *)arg);

    int leftFork, rightFork;
    leftFork = id;
    rightFork = (id + 1) % NUM_Philosophers;
    int idSauceBowl = id % NUM_SAUCE_BOWLS;

    while (1) {
        if (id % 2 == 0) {
            if (forks[rightFork] == 0) {
                forks[rightFork] = 1;
            }

            if (forks[leftFork] == 0) {
                forks[leftFork] = 1;
            }

            if (sauceBowls[idSauceBowl] == 0){
                sauceBowls[idSauceBowl] = 1;
            }
            
        } else {
            if (forks[leftFork] == 0) {
                forks[leftFork] = 1;
            }

            if (forks[rightFork] == 0) {
                forks[rightFork] = 1;
            }

            if (sauceBowls[idSauceBowl] == 0){
                sauceBowls[idSauceBowl] = 1;
            }
        }

        philosopherIsEating(id, idSauceBowl);

        forks[leftFork] = 0;
        forks[rightFork] = 0;
        sauceBowls[idSauceBowl] = 0;

        philosopherIsThinking(id);

        sleep(0.2);
    }

    return 0;
    
}

void philosopherIsThinking(int num){
    printf("Philosopher %d is thinking\n", num+1);
}

void philosopherIsEating(int num, int s){
    printf("Philosopher %d is eating from sauce bowl %d.\n", num+1, s+1);
}

int main() {
    for(int i = 0; i < NUM_Forks; i++) {
        forks[i] = 1;
    }

    for (int i = 0; i < NUM_SAUCE_BOWLS; i++){
        sauceBowls[i] = 1;
    }

    pthread_t threads[NUM_Philosophers];
    for (int i = 0; i < NUM_Philosophers; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, philosopher, (void *) id);
    }

    for (int i = 0; i < NUM_Philosophers; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}
