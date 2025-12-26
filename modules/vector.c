/*
 * yasos - a compiler for yasos language
 * Copyright (c) 2025 DameChocolateYa
 * Licensed under the BSD 3-Clause License.
 * See LICENSE file in the project root for full license text.
*/

#include <string.h>
#include "std.h"
#include "mem.h"

// NOT RECOMMENDED TO USE, DINAMIC ARRAYS ARE MORE RELIABLE

__attribute__((visibility("default")))
struct Vec {
    void **data;
    int size;
    int capacity;
    int elem_size;
};

__attribute__((visibility("default")))
struct Vec new$MODvec(int elem_size) {
    struct Vec vec;
    vec.size = 0;
    vec.capacity = 4;
    vec.elem_size = elem_size;
    vec.data = alloc$MODmem(vec.capacity * elem_size);
    if (vec.data == NULL) {
        perror$MODstd("ERROR: could not allocate memory for vector (element size %d bytes)\n", elem_size);
    }

    return vec;
}

__attribute__((visibility("default")))
void init$MODvec(struct Vec *vec, int elem_size) {
    vec->size = 0;
    vec->capacity = 4;
    vec->elem_size = elem_size;
    vec->data = alloc$MODmem(vec->capacity * elem_size);
    if (vec->data == NULL) {
        perror$MODstd("ERROR: could not allocate memory for vector (element size %d bytes)\n", elem_size);
    }
}

__attribute__((visibility("default")))
void free$MODvec(struct Vec *vec) {
    free$MODmem(vec->data);
    vec->data = NULL;
    vec->capacity = 0;
    vec->size = 0;
    vec->elem_size = 0;
}

__attribute__((visibility("default")))
void push$MODvec(struct Vec *vec, const void *elem) {
    if (vec->size >= vec->capacity) {
        vec->capacity *= 2;
        vec->data = realloc$MODmem(vec->data, vec->capacity * vec->elem_size);
    }

    void *dest = (char *)vec->data + vec->size * vec->elem_size;
    memcpy(dest, elem, vec->elem_size);
    vec->size++;
}

__attribute__((visibility("default")))
void *pop$MODvec(struct Vec *vec) {
    if (vec->size <= 0) {
        perror$MODstd("ERROR: tried to pop back an empty vector\n");
        return NULL;
    }

    void *src = (char *)vec->data + (vec->size - 1) * vec->elem_size;
    void *elem = alloc$MODmem(vec->elem_size);

    memcpy(elem, src, vec->elem_size);
    memset(src, 0, vec->elem_size);

    return elem;
}

__attribute__((visibility("default")))
void edit$MODvec(struct Vec *vec, int index, void *elem) {
    if (index < 0 || index >= vec->size) {
        perror$MODstd("ERROR: index out of range\n");
        return;
    }

    void *pos = (char *)vec->data + index * vec->elem_size;
    memset(pos, 0, vec->elem_size);
    memcpy(pos, elem, vec->elem_size);
}

__attribute__((visibility("default")))
void increase$MODvec(struct Vec *vec, int spaces_to_add) {
    vec->capacity += spaces_to_add;
    vec->data = realloc$MODmem(vec->data, vec->capacity * vec->elem_size);
}

__attribute__((visibility("default")))
void *get$MODvec(struct Vec *vec, int index) {
    if (index < 0 || index >= vec->size) {
        perror$MODstd("ERROR: index out of range (%d > %d)\n", index, vec->size);
        return NULL;
    }

    return (char *)vec->data + index * vec->elem_size;
}

__attribute__((visibility("default")))
void erase$MODvec(struct Vec *vec, int index) {
    if (index < 0 || index >= vec->size) {
        perror$MODstd("ERROR: index out of range (%d > %d)\n", index, vec->size);
        return;
    }

    for (int i = 0, j = 0; i < vec->size; i++) {
        if (i == index) continue;

        void *pos = (char *)vec->data + j * vec->elem_size;
        void *elem = (char *)vec->data + i * vec->elem_size;
        memcpy(pos, elem, vec->elem_size);

        j++;
    }
    vec->size--;
}

__attribute__((visibility("default")))
void erase_all$MODvec(struct Vec *vec) {
    if (vec->size <= 0) return;

    free$MODmem(vec->data);
    vec->data = NULL;
    vec->size = 0;
}

__attribute__((visibility("default")))
void increment$MODvec(struct Vec *vec, int amount) {
    vec->capacity += amount;

    vec->data = realloc$MODmem(vec->data, amount * vec->elem_size);
    if (vec->data == NULL) {
        perror$MODstd("ERROR: could not reallocate memory for vector\n");
    }
}

__attribute__((visibility("default")))
void decrease$MODvec(struct Vec *vec, int amount) {
    vec->capacity -= amount;

    vec->data = realloc$MODmem(vec->data, amount * vec->elem_size);
    if (vec->data == NULL) {
        perror$MODstd("ERROR: could not allocate memory for vector\n");
    }
}

__attribute__((visibility("default")))
int size$MODvec(struct Vec *vec) {
    return vec->size;
}

__attribute__((visibility("default")))
void *front$MODvec(struct Vec *vec) {
    return (char *)vec->data;
}

__attribute__((visibility("default")))
void *back$MODvec(struct Vec *vec) {
    return (char *)vec->data + (vec->size - 1) * vec->elem_size;
}
