#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("Error creating socket");
		return 1;
	}
	printf("Socket created successfully\n");
	close(sockfd);
	return 0;
}
