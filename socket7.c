#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>388
#include <unistd.h>
#include <arpa/inet.h>
int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("Error creating socket");
		return 1;
	}
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
	if (connect(sockfd, (struct sockaddr *)&server_addr,sizeof(server_addr)) == -1) {
		perror("Error connecting to server");
		close(sockfd);
		return 1;
	}
	printf("Connected to server\n");
	const char *message = "Hello from client";
	ssize_t bytes_sent = send(sockfd, message, strlen(message), 0);
	if (bytes_sent == -1) {
		perror("Error sending data");
		close(sockfd);
		return 1;
	}
	printf("Message sent\n");
	char buffer[1024];
	ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
	if (bytes_received == -1) {
		perror("Error receiving data");
		close(sockfd);
		return 1;
	}
	buffer[bytes_received] = '\0';
	printf("Received: %s\n", buffer);
	close(sockfd);
	return 0;
}
