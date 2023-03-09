#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define PHIL 5
#define RIGHT i
#define LEFT ((i + 1) % PHIL)

sem_t forks[PHIL]; // Forks

void *dining_philosopher(void *args)
{
    int i = *(int *)args;
    while (1)
    {
        printf("Philosopher [%d] thinks...\n", i + 1);
        sleep(3);
        printf("Philosopher [%d] is hungry...\n", i + 1);
        sleep(3);

        sem_wait(&forks[RIGHT]);
        sem_wait(&forks[LEFT]);

        printf("Philosopher [%d] took forks {%d, %d}\n", i + 1, RIGHT + 1, LEFT + 1);
        printf("Philosopher [%d] is eating...\n", i + 1);
        sleep(2);
        printf("Philosopher [%d] left forks {%d, %d}\n", i + 1, RIGHT + 1, LEFT + 1);

        sem_post(&forks[RIGHT]);
        sem_post(&forks[LEFT]);
    }
}

int main(int argc, char *argv[])
{
    int i;
    pthread_t philosophers[PHIL];

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

    return 0;
}