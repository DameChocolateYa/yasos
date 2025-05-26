#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

void printbp(const char* format, ...) {
    if (!format) {
        fprintf(stderr, "printbp, INVALID format\n");
        return;
    }
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);
}

char* scani() {
    char buffer[240];
 
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error al leer desde stdin\n");
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    char* result = (char*)malloc(len);
    if (result != NULL) {
        strcpy(result, buffer);
    }

    return result;
}

int isint(const char* s1) {
    for (int i = 0; s1[i] != '\0'; ++i) {
        if (s1[i] < '0' || s1[i] > '9') {
            return 0;
        }
    }
    return 1;
}

void waitk() {
    struct termios oldt, newt;

    // Intenta obtener el modo actual del terminal
    if (ioctl(0, TCGETS, &oldt) == -1) {
        // Si falla, simplemente leer una tecla y continuar
        char c;
        read(0, &c, 1);
        return;
    }

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);

    ioctl(0, TCSETS, &newt);  // Aplicar modo sin canon ni echo

    char c;
    read(0, &c, 1);  // Leer una sola tecla sin esperar ENTER

    ioctl(0, TCSETS, &oldt);  // Restaurar configuración original
}

void cls() {
    system("clear");
}

int sysexc(const char* command) {
    if (command == NULL) return 1;
    return system(command);
}
