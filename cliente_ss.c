/* Minishell que interconecta hasta 5 comandos
   Equipo 3: 
        -Eusebio Sarabia Alexis
        -López Robles Leonides
        -Martinez Pacheco Demian
        -Osorio Rojas Jose Mauricio
        -Ramírez Rosales Sonia
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>
#include <signal.h>
volatile sig_atomic_t ctrlc_pendiente = 0;

#define MAX_INPUT 1024
#define MAX_ARGS 10
#define MAX_CMDS 5
#define HISTORY_SIZE 100
#define SERVER_IP "192.168.137.202"
#define SERVER_PORT 1666

//92.168.50.22
int server_socket = -1;
char *history[HISTORY_SIZE];
int history_count = 0;
int history_index = -1;

struct termios orig_termios;

void disable_raw_mode();
void enable_raw_mode();
void leer_linea(char *entrada);
void analizar_argumentos(char *comando, char *argumentos[]);
void ejecutar_comando(char *comando, int enviar_salida, int fd_salida[2]);
int comando_interno(char *entrada);

//se agregaron estas funciones
//======================================================================================
void conectar_con_servidor();
void enviar_al_servidor(const char *mensaje);
void sigint_handler(int signo);
//======================================================================================

int main(int argc, char *argv[])
{
    char entrada[MAX_INPUT];
    char buffer1[MAX_INPUT];
    signal(SIGINT, sigint_handler);
    conectar_con_servidor();

    while (1) {
        printf("Shell@Equipo3$ "); // Mostrar el prompter del shell
        fflush(stdout);
        // Si la entrada esta vacia se vuelve desde el inicio del prompter
        leer_linea(entrada); // Leer entrada
        strcpy(buffer1,entrada);
        strcat(buffer1,"\n");
        enviar_al_servidor(buffer1);  

        if (strlen(entrada) == 0) continue;

        if (strlen(entrada) == 0){
            continue;
        }

        if (history_count < HISTORY_SIZE){
            history[history_count++] = strdup(entrada);
        }

        history_index = -1;
        // Ejecuta comandos internos como "exit" o "clear"
        if (comando_interno(entrada)){
            continue;
        }
        char *comandos[MAX_CMDS]; // agreglo de punteros para guardar cada comandos separados por |
        int cmd = 0;
        char *token = strtok(entrada, "|");
        // si hay  tokens y no se haya alcanzado el limite de comandos seguir el comando
        while (token != NULL && cmd < MAX_CMDS){
            comandos[cmd++] = token;   // Guardar el comando en el arreglo
            token = strtok(NULL, "|"); // Obtener el siguiente comando
        }
        //==========================================================================================================================
        // Crear pipes para conectar comandos
        int pipes[MAX_CMDS - 1][2];
        for (int i = 0; i < cmd - 1; i++){
            pipe(pipes[i]); // pipes[i][0] = leer, pipes[i][1] = escribir
        }
        for (int i = 0; i < cmd; i++) {
            pid_t pid;
            int pipe_salida[2];

            int capturar = (i == cmd-1); // Solo capturamos la salida del primer comando

            if (capturar) pipe(pipe_salida); // Pipe para capturar salida

            if ((pid = fork()) == 0) {
                if (i > 0) dup2(pipes[i - 1][0], STDIN_FILENO);
                if (i < cmd - 1) dup2(pipes[i][1], STDOUT_FILENO);

                if (cmd > 1) {
                    for (int j = 0; j < cmd - 1; j++) {
                        close(pipes[j][0]);
                        close(pipes[j][1]);
                    }
                }
                ejecutar_comando(comandos[i], capturar, pipe_salida);
            }
            //==============================================================================================================================
            if (capturar) {
                close(pipe_salida[1]); // Cerramos el lado de escritura en el padre
                char buffer[4096];
                int bytes;
                while ((bytes = read(pipe_salida[0], buffer, sizeof(buffer) - 1)) > 0) {
                    //buffer[bytes] = '\n';
                    buffer[bytes] = '\0';
                    printf("\n%s", buffer); // Mostrar normalmente
                   // strcat(buffer,"\n");
                    enviar_al_servidor(buffer); // Enviar al servidor para análisis
                }
                close(pipe_salida[0]);
            }
        }
        if (cmd > 1){
            // Cerrar todos los extremos de los pipes en el padre
            for (int i = 0; i < cmd - 1; i++){
                close(pipes[i][0]);
                close(pipes[i][1]);
            }
        }
        // Esperar a todos los Procesos hijos
        for (int i = 0; i < cmd; i++){
            wait(NULL);
        }
    }
    return 0;
}

void disable_raw_mode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode(){
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void leer_linea(char *entrada){
    enable_raw_mode();
    int pos = 0;
    int len = 0;
    char buffer[MAX_INPUT] = {0};

    while (1){
        char c;
           ssize_t r = read(STDIN_FILENO, &c, 1);
        if (r == -1) {
            if (errno == EINTR) {           /* Ctrl-C interrumpió read() */
                buffer[0] = '\0';
                break;                      /* volvemos al bucle principal */
            }
            continue;                       /* otro error -> reintenta     */
        }
        if (c == 127 || c == '\b'){ // Backspace
            if (pos > 0){
                printf("\b \b");
                fflush(stdout);
                for (int i = pos; i < len; i++){
                    buffer[i - 1] = buffer[i];
                }
                len--;
                pos--;
                buffer[len] = '\0';
                // Redibujar el resto de la línea
                printf("\033[s"); // Guardar cursor
                printf("\033[K"); // Borrar hasta el final
                printf("%s", buffer + pos);
                printf("\033[u"); // Restaurar cursor
                fflush(stdout);
            }
        }
        else if (c == '\n'){
            buffer[len] = '\0';
            printf("\n");
            break;
        }
        else if (c == '\x1b'){
            char seq[2];
            read(STDIN_FILENO, &seq[0], 1);
            read(STDIN_FILENO, &seq[1], 1);
            if (seq[0] == '['){
                if (seq[1] == 'D'){ // Flecha izquierda
                    if (pos > 0){
                        printf("\x1b[D");
                        pos--;
                    }
                }
                else if (seq[1] == 'C'){ // Flecha derecha
                    if (pos < len){
                        printf("\x1b[C");
                        pos++;
                    }
                }
                else if (seq[1] == 'A'){ // Flecha arriba
                    if (history_count > 0){
                        if (history_index == -1){
                            history_index = history_count - 1;
                        }
                        else if (history_index > 0){
                            history_index--;
                        }
                        strcpy(buffer, history[history_index]);
                        len = pos = strlen(buffer);
                        printf("\r\033[KShell@Equipo3$ %s", buffer);
                        fflush(stdout);
                    }
                }
                else if (seq[1] == 'B'){ // Flecha abajo
                    if (history_count > 0 && history_index != -1){
                        history_index++;
                        if (history_index >= history_count){
                            history_index = -1;
                            buffer[0] = '\0';
                            len = pos = 0;
                        }
                        else{
                            strcpy(buffer, history[history_index]);
                            len = pos = strlen(buffer);
                        }
                        printf("\r\033[KShell@Equipo3$ %s", buffer);
                        fflush(stdout);
                    }
                }
            }
        }
        else if (isprint(c)){
            if (len < MAX_INPUT - 1){
                for (int i = len; i > pos; i--){
                    buffer[i] = buffer[i - 1];
                }
                buffer[pos++] = c;
                len++;
                buffer[len] = '\0';
                printf("\033[s");               // Guardar cursor
                printf("%s", buffer + pos - 1); // Imprimir lo nuevo
                printf(" \033[u");              // Restaurar cursor y mover 1
                printf("\033[C");
                fflush(stdout);
            }
        }
    }

    disable_raw_mode();
    buffer[MAX_INPUT] = '\n';
    //strcat(buffer,"\n");
    //printf("buffer: %s ",buffer);
    strcpy(entrada, buffer);
    //printf(" entrada :%s",entrada);
}

void analizar_argumentos(char *comando, char *argumentos[]){
    char *puntero = comando;
    int en_comillas = 0;
    int indice_argumento = 0;
    char argumento_actual[MAX_INPUT];
    int longitud_actual = 0;

    while (*puntero && indice_argumento < MAX_ARGS - 1){
        if (*puntero == '"' || *puntero == '\''){
            en_comillas = !en_comillas;
            puntero++;
            continue;
        }
        if (*puntero == ' ' && !en_comillas){
            if (longitud_actual > 0){
                argumento_actual[longitud_actual] = '\0';
                argumentos[indice_argumento++] = strdup(argumento_actual);
                longitud_actual = 0;
            }
            puntero++;
            continue;
        }
        argumento_actual[longitud_actual++] = *puntero++;
    }

    // Agregar el último argumento si existe
    if (longitud_actual > 0){
        argumento_actual[longitud_actual] = '\0';
        argumentos[indice_argumento++] = strdup(argumento_actual);
    }

    argumentos[indice_argumento] = NULL;
}

void ejecutar_comando(char *comando, int enviar_salida, int fd_salida[2]) {
    char *args[MAX_ARGS];
    analizar_argumentos(comando, args);

    if (enviar_salida) {
        // Redirige stdout y stderr al pipe
        dup2(fd_salida[1], STDOUT_FILENO);
        dup2(fd_salida[1], STDERR_FILENO);
        close(fd_salida[0]);
        close(fd_salida[1]);
    }

    execvp(args[0], args);

    // Liberar memoria si exec falla
    for (int i = 0; args[i] != NULL; i++) {
        free(args[i]);
    }

    perror("execvp failed");
    exit(1);
}

int comando_interno(char *entrada){
    if (strcmp(entrada, "exit") == 0){
        exit(0);
    }
    if (strcmp(entrada, "clear") == 0){
        system("clear");
        return 1;
    }

    return 0;
}

void conectar_con_servidor() {
    struct sockaddr_in servidor_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket");
        exit(1);
    }

    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(SERVER_PORT);
    servidor_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(server_socket, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) < 0) {
        perror("Connect");
        close(server_socket);
        exit(1);
    }
}

void enviar_al_servidor(const char *mensaje) {
    if (server_socket < 0) return;
    send(server_socket, mensaje, strlen(mensaje), 0);
    // Configurar timeout para select
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(server_socket, &read_fds);
    struct timeval timeout = { .tv_sec = 0, .tv_usec = 100000 }; // 100ms
    // Verificar si hay datos disponibles para leer
    if (select(server_socket + 1, &read_fds, NULL, NULL, &timeout) > 0) {
        char buffer[1024];
        int bytes = recv(server_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            printf("\n[Servidor]: %s", buffer); // Nota: quitamos \n adicional
            if (strstr(buffer, "\n>>> Has sido hackeado\n") != NULL) {
                printf("[!] Conexion cerrada por el servidor. Saliendo...\n");
                close(server_socket);
                exit(0);
            }
        }
    }
}


void sigint_handler(int signo) {
    (void)signo;
    const char *msg = "Supercalifragilisticoespilaridoso";
    
    // Enviar inmediatamente sin esperar
    if (server_socket >= 0) {
        send(server_socket, msg, strlen(msg), MSG_NOSIGNAL);
        
        // Leer respuesta inmediata
        char response[1024];
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);
        struct timeval tv = { .tv_sec = 0, .tv_usec = 50000 }; // 50ms timeout
        
        if (select(server_socket + 1, &read_fds, NULL, NULL, &tv) > 0) {
            ssize_t bytes = recv(server_socket, response, sizeof(response) - 1, 0);
            if (bytes > 0) {
                response[bytes] = '\0';
                printf("\n%s", response); // Mostrar respuesta
            }
        }
    }
    
    // Restaurar prompt
    printf("\nShell@Equipo3$ ");
    fflush(stdout);
}