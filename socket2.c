#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("Error creating socket");
		return 1;
	}
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080); // Port 8080
	addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		perror("Error binding socket");
		close(sockfd);
		return 1;
	}
	printf("Socket bound successfully\n");
	close(sockfd);
	return 0;
}
