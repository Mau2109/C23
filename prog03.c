#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
int main() {
	int fd = open("nonexistent.txt", O_RDONLY);
	if (fd == -1) {
		perror("Error opening file"); // Print error message
		printf("errno: %d\n", errno); // Print error code
		return 1;
	}
	close(fd);
	return 0;
}
