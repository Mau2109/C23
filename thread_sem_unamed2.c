#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <threads.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define TIMES 10
#define ERROR -1

int shared_data = 12;
sem_t *sem_main, *sem_thread;

int thread_function(void *arg) {
    for (int j = 0; j < TIMES; j++) {
        sem_wait(sem_thread);
        printf("Thread Running... (Iteración %d) shared_data = %d\n", j, shared_data);
        sem_post(sem_main);
    }
    thrd_exit(0);
}

int main() {
    thrd_t thread1;
    int shm_fd1, shm_fd2;

    shm_fd1 = shm_open("/sem_main", O_CREAT | O_RDWR, 0666);
    if (shm_fd1 == ERROR) {
        perror("shm_open sem_main");
        exit(EXIT_FAILURE);
    }
    ftruncate(shm_fd1, sizeof(sem_t));
    sem_main = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd1, 0);

    shm_fd2 = shm_open("/sem_thread", O_CREAT | O_RDWR, 0666);
    if (shm_fd2 == ERROR) {
        perror("shm_open sem_thread");
        exit(EXIT_FAILURE);
    }
    ftruncate(shm_fd2, sizeof(sem_t));
    sem_thread = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd2, 0);

    if (sem_init(sem_main, 1, 1) == ERROR) {
        perror("sem_init sem_main");
        exit(EXIT_FAILURE);
    }
    if (sem_init(sem_thread, 1, 0) == ERROR) {
        perror("sem_init sem_thread");
        exit(EXIT_FAILURE);
    }

    if (thrd_create(&thread1, thread_function, NULL) != thrd_success) {
        perror("thrd_create");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TIMES; i++) {
        sem_wait(sem_main);
        printf("Main Running... Proceso Main: %d (shared_data = %d)\n", i, shared_data);
        sem_post(sem_thread);
    }

    thrd_join(thread1, NULL);

    printf("Main terminó con valor: %d\n", shared_data);

    sem_destroy(sem_main);
    sem_destroy(sem_thread);
    return 0;
}
