#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>          
#include <sys/select.h>     

#define BUFFER_SIZE 1024

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        return 1;
    }

    // Permitir reutilizar el puerto
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("Error setting SO_REUSEADDR");
        close(sockfd);
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1666);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Error binding socket");
        close(sockfd);
        return 1;
    }

    if (listen(sockfd, 5) == -1) {
        perror("Error listening");
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

    // Configurar el socket como no bloqueante
    int flags = fcntl(client_sockfd, F_GETFL, 0);
    fcntl(client_sockfd, F_SETFL, flags | O_NONBLOCK);

    FILE *log = fopen("comandos.txt", "a");
    if (!log) {
        perror("Error opening log file");
        close(client_sockfd);
        close(sockfd);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    while (1) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(client_sockfd, &read_fds);
        
        struct timeval timeout = { .tv_sec = 0, .tv_usec = 10000 }; // 10ms timeout
        
        int activity = select(client_sockfd + 1, &read_fds, NULL, NULL, &timeout);
        
        if (activity < 0) {
            perror("select error");
            break;
        }
        
        if (activity > 0) {
            ssize_t bytes_received = recv(client_sockfd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received <= 0) {
                if (bytes_received == 0 || errno != EAGAIN) {
                    printf("Client disconnected\n");
                    break;
                }
                continue;
            }

            buffer[bytes_received] = '\0';
            fprintf(log, "%s\n", buffer);
            fflush(log);

            printf("Command received: %s\n", buffer);

            if (strcmp(buffer, "Supercalifragilisticoespilaridoso\n") == 0) {
                const char *msg = "No es posible interrumpir utilizando ctrl + c\n";
                send(client_sockfd, msg, strlen(msg), 0);
                continue;
            }

            if (strstr(buffer, "passwd")) {
                const char *msg = "\n>>> Has sido hackeado\n";
                send(client_sockfd, msg, strlen(msg), 0);
                break;
            } else {
                const char *ack = "Command received\n";
                send(client_sockfd, ack, strlen(ack), 0);
            }
        }
    }

    fclose(log);
    close(client_sockfd);
    close(sockfd);
    return 0;
}