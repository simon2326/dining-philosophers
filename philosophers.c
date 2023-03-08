#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define PHIL 5
#define RIGHT i
#define LEFT ((i + 1) % PHIL)

sem_t forks[PHIL]; // Forks
sem_t state;

void *dining_philosopher(void *args)
{
    int i = *(int *)args;

    while (1)
    {
        sem_wait(&state);
        printf("Philosopher [%d] thinks...\n", i);
        sem_post(&state);
        sleep(3);

        sem_wait(&state);
        printf("Philosopher [%d] is hungry...\n", i);
        sem_post(&state);
        sleep(3);

        sem_wait(&forks[RIGHT]);
        sem_wait(&forks[LEFT]);

        sem_wait(&state);
        printf("Philosopher [%d] took forks {%d, %d}\n", i, RIGHT, LEFT);
        printf("Philosopher [%d] is eating...\n", i);
        sem_post(&state);

        sem_post(&forks[RIGHT]);
        sem_post(&forks[LEFT]);

        printf("Philosopher [%d] left forks {%d, %d}\n", i, RIGHT, LEFT);
    }
}

int main(int argc, char *argv[])
{

    int i;
    pthread_t philosophers[PHIL];
    sem_init(&state, 0, 1);

    for (i = 0; i < PHIL; i++)
    {
        sem_init(&forks[i], 0, 1); // Initializing semaphores with 1 source (forks)
    }

    for (i = 0; i < PHIL; i++)
    {
        pthread_create(&philosophers[i], NULL, &dining_philosopher, &i); // Creating philosophers
    }

    for (i = 0; i < PHIL; i++)
    {
        pthread_join(&philosophers[i], NULL); // Wait each philosopher to eat
    }

    for (i = 0; i < PHIL; i++)
    {
        sem_destroy(&forks[i]); // Destroy every semaphore (fork)
    }
}