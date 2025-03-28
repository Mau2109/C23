#include <stdio.h>
#include <unistd.h>
int main() {
	pid_t pid = fork(); // Create a new process
	if (pid == -1) {
		perror("Error forking process");
		return 1;
	} else if (pid == 0) {
		// Child process
		printf("Child process: PID = %d, Parent PID = %d\n", getpid(),getppid());
	} else {
	// Parent process
		printf("Parent process: PID = %d, Child PID = %d\n", getpid(),,→ pid);
	}
	return 0;
}