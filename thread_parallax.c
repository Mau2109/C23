#include <threads.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>

mtx_t mutex;
int semid;
int shared_data = 0;
int i = 0;
#define TIMES 10
#define ERROR -1	 

void wait_sem(int sem_num){
	struct sembuf op =
	{
		sem_num,
		-1,
		0
	};
	semop(semid, &op, 1);
}

void signal_sem(int sem_num){
	struct sembuf op =
	{
		sem_num,
		1,
		0
	};
	semop(semid, &op, 1);
}

int thread_function(void *arg);

int main() {
    thrd_t thread1;
    key_t llave;
    shared_data = 12;

    llave = ftok("/bin/ls",1974);
    if (llave == ERROR){
    	perror("ftok() call failed!");
    	exit(1);
    }

    semid = semget(llave, 2, IPC_CREAT | 0666);
    if (semid == ERROR){
    	perror("semget() call failed!");
    	exit(1);
    }

    semctl(semid, 0, SETVAL, 1);
    semctl(semid, 1, SETVAL, 0);

    mtx_init(&mutex, mtx_plain);

    thrd_create(&thread1, thread_function, (void *)1);

    for (i = 0; i < TIMES; i++) {
        // Imprimir en el main
        wait_sem(0);
        printf("Main Running... Proceso Main: %d (Variable global = %d)\n", i, shared_data);
        signal_sem(1);
        
       
    }
    thrd_join(thread1, NULL);

    printf("Main ended with value: %d\n", shared_data);

    mtx_destroy(&mutex);
    return 0;
}

int thread_function(void *arg) {
    // Imprimir en el hilo
    for (int j = 0; j < TIMES; j++) {
        wait_sem(1);
        printf("Thread %ld Running... (Iteration %d) shared_data = %d\n", (long)arg, j, shared_data);
        signal_sem(0);
    }

    thrd_exit(0);
}