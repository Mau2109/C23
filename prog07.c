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
		execlp("ls", "ls", "-l", NULL); // Replace the child process with 'ls -l'
		perror("Error executing ls"); // This line is only reached if execlp fails
		return 1;
	} else {
		// Parent process
		int status;
		wait(&status); // Wait for the child process to terminate
		if (WIFEXITED(status)) {
			printf("Child process exited with status %d\n",WEXITSTATUS(status));
		} else {
			printf("Child process terminated abnormally.\n");
		}
	}
	return 0;
}