#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define NUM_Philosophers 5
#define NUM_Forks 5

int forks[NUM_Forks] = {0};

void philosopherIsThinking(int num);
void philosopherIsEating(int num);

void *philosophers(void *arg) {
    int id = *((int *)arg);

    int leftFork, rightFork;
    leftFork = id;
    rightFork = (id + 1) % NUM_Philosophers;

    while (1) {
        if (id % 2 == 0) {
            if (forks[rightFork] == 0) {
                forks[rightFork] = 1;
            }

            if (forks[leftFork] == 0) {
                forks[leftFork] = 1;
            }
        } else {
            if (forks[leftFork] == 0) {
                forks[leftFork] = 1;
            }

            if (forks[rightFork] == 0) {
                forks[rightFork] = 1;
            }
        }

        philosopherIsEating(id);

        forks[leftFork] = 0;
        forks[rightFork] = 0;

        philosopherIsThinking(id);

        sleep(0.2);

    }

    return 0;
}

void philosopherIsThinking(int num){
    printf("Philosopher %d is thinking\n", num+1);
}

void philosopherIsEating(int num){
    printf("Philosopher %d is eating\n", num+1);
}

int main() {
    for(int i = 0; i < NUM_Forks; i++) {
        forks[i] = 1;
    }

    pthread_t threads[NUM_Philosophers];
    for (int i = 0; i < NUM_Philosophers; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, philosophers, (void *) id);
    }

    for (int i = 0; i < NUM_Philosophers; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}
