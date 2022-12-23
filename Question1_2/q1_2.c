#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define NUM_Philosophers 5
#define NUM_Forks 5

sem_t forks[NUM_Forks];

void philosopherIsThinking(int num);
void philosopherIsEating(int num);

int arrEating[NUM_Philosophers] = {0};

void *philosophers(void *num){
    int id = *((int *) num);

    int leftFork;
    int rightFork;

    if (id == 4){
        leftFork = (id + 1) % NUM_Forks;
        rightFork = id;
    } else {
        leftFork = id; 
        rightFork = (id + 1) % NUM_Forks;
    }
    
    while(1){
        philosopherIsThinking(id);
        sem_wait(&forks[leftFork]);
        sem_wait(&forks[rightFork]);
        philosopherIsEating(id);

        arrEating[id] += 1;
        for(int i = 0; i < NUM_Philosophers; i++) {
            printf("Philosopher %d has eaten %d number of times.\n", i+1, arrEating[i]);
        }

        sem_post(&forks[leftFork]);
        sem_post(&forks[rightFork]);
    } 

    return NULL;
}

void philosopherIsThinking(int num){
    printf("Philosopher %d is thinking\n", num+1);
}

void philosopherIsEating(int num){
    printf("Philosopher %d is eating\n", num+1);
}

int main(){
    for (int i = 0; i < NUM_Forks; i++){
        sem_init(&forks[i], 0, 1);
    }

    pthread_t threads[NUM_Philosophers];
    for (int i = 0; i < NUM_Philosophers; i++){
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, philosophers, (void *) id);
    }

    for (int i = 0; i < NUM_Philosophers; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}
