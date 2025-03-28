#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
int main() {
	const char *msg = "Hello, World!\n";
	syscall(SYS_write, STDOUT_FILENO, msg, 13); // Directly invoke the
	return 0;
}