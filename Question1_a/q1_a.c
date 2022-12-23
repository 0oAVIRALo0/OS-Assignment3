#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define NUM_Philosophers 5
#define NUM_Forks 5
#define NUM_SAUCE_BOWLS 2

void philosopherIsThinking(int num);
void philosopherIsEating(int num, int s);

sem_t forks[NUM_Forks];
sem_t sauceBowls[NUM_SAUCE_BOWLS];

void *philosopher(void *arg) {
    int id = *((int *) arg);

    int leftFork;
    int rightFork;

    while (1) {
        leftFork = id; 
        rightFork = (id + 1) % NUM_Forks;

        philosopherIsThinking(id);

        sem_wait(&forks[leftFork]);
        sem_wait(&forks[rightFork]);

        int idSauceBowl = id % NUM_SAUCE_BOWLS;
        sem_wait(&sauceBowls[idSauceBowl]);

        philosopherIsEating(id, idSauceBowl);
        // sleep(1);

        sem_post(&forks[leftFork]);
        sem_post(&forks[rightFork]);
        sem_wait(&sauceBowls[idSauceBowl]);
    }
    return 0;
}

void philosopherIsThinking(int num){
    printf("Philosopher %d is thinking\n", num+1);
}

void philosopherIsEating(int num, int s){
    printf("Philosopher %d is eating from sauce bowl %d\n", num+1, s+1);
}

int main() {
    for (int i = 0; i < NUM_Forks; i++) {
        sem_init(&forks[i], 0, 1);
    }

    for (int i = 0; i < NUM_SAUCE_BOWLS; i++) {
        sem_init(&sauceBowls[i], 0, 1);
    }

    pthread_t threads[NUM_Philosophers];
    for (int i = 0; i < NUM_Philosophers; i++){
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, philosopher, (void *) id);
    }

    for (int i = 0; i < NUM_Philosophers; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}
