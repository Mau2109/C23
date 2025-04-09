#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main() {
	const char *queue_name = "/my_queue";
	mqd_t mq = mq_open(queue_name, O_CREAT | O_RDWR, 0666, NULL);
	if (mq == (mqd_t)-1) {
		perror("Error creating message queue");
		return 1;
	}

	pid_t pid = fork();
	if (pid == -1) {
		perror("Error forking process");
		return 1;
	}

	if (pid == 0) {
		// Hijo: recibir mensaje
		char buffer[9048];
		if (mq_receive(mq, buffer, sizeof(buffer), NULL) == -1) {
			perror("Error receiving message");
			return 1;
		}
		printf("Hijo recibió: %s\n", buffer);
	} else {
		// Padre: enviar mensaje
		const char *message = "Hola desde el padre";
		sleep(1); // Dar tiempo al hijo para estar listo para recibir
		if (mq_send(mq, message, strlen(message) + 1, 0) == -1) {
			perror("Error sending message");
			return 1;
		}
		wait(NULL); // Esperar a que el hijo termine
		mq_unlink(queue_name); // Eliminar la cola de mensajes
	}

	mq_close(mq);
	return 0;
}
