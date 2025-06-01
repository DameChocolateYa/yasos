#include <stdio.h>
#include <stdlib.h>

FILE* openf(const char* path, const char* mode) {
	FILE* file = fopen(path, mode);
	return file;
}

char* readf(FILE* file) {
	fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);  // o fseek(file, 0, SEEK_SET)

    char* content = (char*)malloc(size + 1);
    if (!content) return NULL;

    size_t read = fread(content, 1, size, file);
    content[read] = '\0';
    return content;
}

void writef(FILE* file, const char* text) {
    if (file != NULL && text != NULL) {
        fputs(text, file);
    }
}

void closef(FILE** fileptr) {
    if (fileptr && *fileptr) {
        fclose(*fileptr);
        *fileptr = NULL;
    }
}

int isfopen(FILE* file) {
	if (file == NULL) return 0;
	else return 1;
}
