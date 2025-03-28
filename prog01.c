#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
int main() {
	int fd = open("example.txt", O_RDONLY);
	if (fd == -1) {
		perror("Error opening file");
		return 1;
	}
	char buffer[100];
	ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	if (bytes_read == -1) {
		perror("Error reading file");
		close(fd);
		return 1;
	}
	buffer[bytes_read] = '\0'; // Null-terminate the string
	printf("Read: %s\n", buffer);
	close(fd);
	return 0;
}