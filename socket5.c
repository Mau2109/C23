#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
	perror("Error creating socket");
	return 1;
	}
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		perror("Error binding socket");
		close(sockfd);
		return 1;
	}
	if (listen(sockfd, 5) == -1) {
		perror("Error listening on socket");
		close(sockfd);
		return 1;
	}
	printf("Listening for incoming connections...\n");
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
	if (client_sockfd == -1) {
		perror("Error accepting connection");
		close(sockfd);
		return 1;
	}
	printf("Connection accepted\n");
	char buffer[1024];
	ssize_t bytes_received = recv(client_sockfd, buffer, sizeof(buffer),0);
	if (bytes_received == -1) {
		perror("Error receiving data");
		close(client_sockfd);
		close(sockfd);
		return 1;
	}
	buffer[bytes_received] = '\0';
	printf("Received: %s\n", buffer);
	const char *response = "Hello from server";
	ssize_t bytes_sent = send(client_sockfd, response, strlen(response),0);
	if (bytes_sent == -1) {
		perror("Error sending data");
		close(client_sockfd);
		close(sockfd);
		return 1;
	}
	printf("Response sent\n");
	close(client_sockfd);
	close(sockfd);
	return 0;
}
