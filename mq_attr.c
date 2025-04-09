#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // O_CREAT, O_RDWR, O_NONBLOCK
#include <sys/stat.h>   // mode_t
#include <mqueue.h>
#include <string.h>
#include <errno.h>

int main() {
    mqd_t mqd;
    struct mq_attr attr, old_attr;
    const char *cola = "/micola";

    // Configuración inicial
    attr.mq_flags = 0;             // Bloqueante
    attr.mq_maxmsg = 5;            // Max 5 mensajes
    attr.mq_msgsize = 128;         // Max 128 bytes por mensaje
    attr.mq_curmsgs = 0;           // Ignorado en mq_open()

    // Crear cola
    mqd = mq_open(cola, O_CREAT | O_RDWR, 0666, &attr);
    if (mqd == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    printf("Cola creada.\n");

    // Obtener atributos actuales
    if (mq_getattr(mqd, &attr) == -1) {
        perror("mq_getattr");
        exit(EXIT_FAILURE);
    }

    printf("Atributos actuales:\n");
    printf("mq_flags   = %ld\n", attr.mq_flags);
    printf("mq_maxmsg  = %ld\n", attr.mq_maxmsg);
    printf("mq_msgsize = %ld\n", attr.mq_msgsize);
    printf("mq_curmsgs = %ld\n", attr.mq_curmsgs);

    // Modificar flag a O_NONBLOCK
    struct mq_attr new_attr;
    new_attr.mq_flags = O_NONBLOCK; // solo este campo es válido

    if (mq_setattr(mqd, &new_attr, &old_attr) == -1) {
        perror("mq_setattr");
        exit(EXIT_FAILURE);
    }

    printf("\nFlag cambiado a O_NONBLOCK\n");
    printf("Valor anterior de mq_flags: %ld\n", old_attr.mq_flags);

    // Verificar nuevo valor
    if (mq_getattr(mqd, &attr) == -1) {
        perror("mq_getattr");
        exit(EXIT_FAILURE);
    }

    printf("Nuevo mq_flags = %ld\n", attr.mq_flags);

    // Cerrar y eliminar la cola
    mq_close(mqd);
    mq_unlink(cola);

    printf("\nCola cerrada y eliminada.\n");

    return 0;
}
