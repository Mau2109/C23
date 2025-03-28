#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main() {
	pid_t pid = fork(); // Create a new process
	if (pid == -1) {
		perror("Error forking process");
		return 1;
	} else if (pid == 0) {
		// Child process
		printf("Child process: PID = %d\n", getpid());
		sleep(2); // Simulate some work
		printf("Child process exiting.\n");
		return 42; // Exit with status 42
	} else {
		// Parent process
		printf("Parent process: PID = %d, Child PID = %d\n", getpid(),pid);
		int status;
		pid_t child_pid = wait(&status); // Wait for the child process toterminate
		if (child_pid == -1) {
			perror("Error waiting for child process");
			return 1;
		}
		if (WIFEXITED(status)) {
			printf("Child process exited with status %d\n",WEXITSTATUS(status));
		} else {
			printf("Child process terminated abnormally.\n");
		}
	}
	return 0;
}