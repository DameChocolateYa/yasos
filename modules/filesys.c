#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>

void openf(FILE** file, const char* path, const char* mode) {
  *file = fopen(path, mode);
}

char* readf(FILE** file) {
	fseek(*file, 0, SEEK_END);
    long size = ftell(*file);
    rewind(*file);  // o fseek(file, 0, SEEK_SET)

    char* content = (char*)malloc(size + 1);
    if (!content) return NULL;

    size_t read = fread(content, 1, size, *file);
    content[read] = '\0';
    return content;
}

void writef(FILE** file, const char* text) {
    if (*file != NULL && text != NULL) {
        fputs(text, *file);
    }
}

void closef(FILE** fileptr) {
    if (*fileptr && *fileptr) {
        fclose(*fileptr);
        *fileptr = NULL;
    }
}

int is_fopen(FILE* file) {
	if (file == NULL) return 0;
	else return 1;
}

int makedir(const char* path) {
    int result = mkdir(path, 0777);
    return result;
}

int makedirs(const char *path) {
    char tmp[512];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = '\0';

    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(tmp, 0777) != 0 && errno != EEXIST) {
                perror("mkdir");
                return -1;
            }
            *p = '/';
        }
    }
    if (mkdir(tmp, 0777) != 0 && errno != EEXIST) {
        perror("mkdir");
        return -1;
    }
    return 0;
}

int removef(const char* filepath) {
    int result = remove(filepath);
    return result;
}

int removedir(const char* dirpath) {
    int result = rmdir(dirpath);
    return result;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>  // Para PATH_MAX

int removedirr(const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (!dir) {
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(full_path, &st) != 0) {
            continue;  // No se pudo obtener información del archivo
        }

        if (S_ISDIR(st.st_mode)) {
            // Es un subdirectorio
            if (removedirr(full_path) != 0) {
                closedir(dir); // No olvides cerrar el directorio antes de salir
                return 1;
            }
        } else {
            // Es un archivo
            if (remove(full_path) != 0) {
                closedir(dir);
                return 1;
            }
        }
    }

    closedir(dir);

    // Elimina el directorio actual
    if (rmdir(path) != 0) {
        return 1;
    }

    return 0;
}
