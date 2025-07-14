#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <time.h>
#include <stdint.h>
#include <sys/random.h>
#include <sys/syscall.h>
#include "math.h"

#define SYS_fork    57
#define SYS_execve  59
#define SYS_wait4   61
#define SYS_exit    60

long syscall(long number, ...); // prototype to avoi libc warning

const char path[] = "/bin/sh";
const char arg0[] = "sh";
const char arg1[] = "-c";

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

int is_int(const char* s1) {
	if (s1[0] == '\0') return 0;
    for (int i = 0; s1[i] != '\0'; ++i) {
        if (s1[i] != '-' && (s1[i] < '0' || s1[i] > '9')) {
            return 0;
        }
        if (i > 0 && s1[i] == '-') {
            return 0;
        }
    }
    return 1;
}

int is_float(const char* s1) {
	if (s1[0] == '\0') return 0;
    int decimal = false;
    for (int i = 0; s1[i] != '\0'; ++i) {
        if (s1[i] != '-' && s1[i] != '.' && (s1[i] < '0' || s1[i] > '9')) {
            return 0;
        }
        if ((i > 0 && s1[i] == '-')) {
            return 0;
        }
        if (s1[i] == '.') {
            if (!decimal) decimal = true;
            else return 0;
        }
    }
    if (!decimal) return 0;
    return 1;
}

int is_num(const char* s1) {
	if (s1[0] == '\0') return 0;
    int decimal = false;
    for (int i = 0; s1[i] != '\0'; ++i) {
        if (s1[i] != '-' && s1[i] != '.' && (s1[i] < '0' || s1[i] > '9')) {
            return 0;
        }
        if ((i > 0 && s1[i] == '-')) {
            return 0;
        }
        if (s1[i] == '.') {
            if (!decimal) decimal = true;
            else return 0;
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
    printf("\e[1;1H\e[2J");
    fflush(stdout);
}

int sysexc(const char* command) {
    if (command == NULL) return 1;
    const char* argv[] = {arg0, arg1, command, NULL};
    const char term[] = "TERM=linux";
    const char* envp[] = {term, NULL};

    long pid = syscall(SYS_fork);
    if (pid == 0) {
        syscall(SYS_execve, path, argv, envp);
        syscall(SYS_exit, 1);
    }
    else if (pid > 0) {
        int status;
        syscall(SYS_wait4, pid, 0, 0, 0);

        if ((status & 0x7f) == 0) {
            int exit_code = (status >> 8) & 0xff;
            return exit_code;
        }
        else {
            return 1;
        }
    }
    else {
        return 1;
    }
    return 1;
}

void termc(const int color) {
    if (color >= 0 && color <= 7) {
        // Set bright text with foreground color
        printf("\033[1;3%dm", color);
    } else {
        // Reset terminal colors
        printf("\033[0m");
    }
}

uint32_t get_secure_random_uint32() {
    uint32_t num;
    ssize_t result = getrandom(&num, sizeof(num), 0);
    if (result != sizeof(num)) {
        perror("getrandom failed");
        return 0;
    }
    return num;
}

int randi(int min, int max) {
    uint32_t r = get_secure_random_uint32();
    return (r % (max - min + 1)) + min;
}

double randf(double min, double max, int decimals) {
	uint32_t r = get_secure_random_uint32();
	double normalized = (double)r / (double)UINT32_MAX;
	double scaled = min + normalized * (max - min);

	double factor = pow(10.0, decimals);
	return round(scaled * factor, decimals) / factor;
}

void nap(const unsigned int seconds) {
	sleep(seconds);
}

void* alloc(size_t size) {
	return malloc(size);
}

void* ralloc(void* ptr, size_t size) {
	return realloc(ptr, size);
}
