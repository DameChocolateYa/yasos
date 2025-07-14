#include <stdlib.h>
#include <stdio.h>


typedef enum { TYPE_INT, TYPE_CHAR, TYPE_DOUBLE } TypeTag;

typedef struct {
    TypeTag type;
    union {
        int i;
        char c;
        double d;
    } value;
} Any;

typedef struct {
    Any* data;
    int size;
    int capacity;
} List;

void push_back(List* list, Any item) {
    if (list->size >= list->capacity) {
        int new_capacity = (list->capacity == 0) ? 4 : list->capacity * 2;
        Any* new_data = (Any*)realloc(list->data, new_capacity * sizeof(Any));
        if (!new_data) exit(1);
        list->data = new_data;
        list->capacity = new_capacity;
    }

    list->data[list->size] = item;
    list->size++;
}
