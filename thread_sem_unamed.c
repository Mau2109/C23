#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <threads.h>

#define TIMES 10

sem_t sem_main, sem_thread;
int shared_data = 12;

int thread_function(void *arg) {
    for (int j = 0; j < TIMES; j++) {
        sem_wait(&sem_thread);  
        printf("Thread Running... (Iteración %d) shared_data = %d\n", j, shared_data);
        sem_post(&sem_main);   
    }
    thrd_exit(0);
}

int main() {
    thrd_t thread1;

   
    if (sem_init(&sem_main, 0, 1) == -1) {
        perror("sem_init sem_main");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&sem_thread, 0, 0) == -1) {
        perror("sem_init sem_thread");
        exit(EXIT_FAILURE);
    }

    if (thrd_create(&thread1, thread_function, NULL) != thrd_success) {
        perror("thrd_create");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TIMES; i++) {
        sem_wait(&sem_main);  
        printf("Main Running... Proceso Main: %d (Variable global = %d)\n", i, shared_data);
        sem_post(&sem_thread);  
    }

    thrd_join(thread1, NULL);

    printf("Main terminó con valor: %d\n", shared_data);

    
    sem_destroy(&sem_main);
    sem_destroy(&sem_thread);

    return 0;
}
