#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main() {
    void *handle;
    int (*suma)(int, int);
    char *error;

    handle = dlopen("./test.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    dlerror();  // limpia errores previos

    *(void **) (&suma) = dlsym(handle, "suma");

    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    int resultado = suma(3, 4);
    printf("Resultado: %d\n", resultado);

    dlclose(handle);
    return 0;
}
