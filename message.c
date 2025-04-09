#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
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
	// Child process: send a message
		const char *message = "Hello from child";
		if (mq_send(mq, message, strlen(message) + 1, 0) == -1) {
		perror("Error sending message");
		return 1;
		}
	} else {
	// Parent process: receive a message
		char buffer[9048];
		if (mq_receive(mq, buffer, sizeof(buffer), NULL) == -1) {
			perror("Error receiving message");
		return 1;

		}
	printf("Received: %s\n", buffer);
	}
	mq_close(mq);
	mq_unlink(queue_name); // Remove the message queue
	return 0;
}
