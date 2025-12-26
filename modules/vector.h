struct Vec {
    void **data;
    int size;
    int capacity;
    int elem_size;
};

struct Vec new$MODvec(int elem_size);
void init$MODvec(struct Vec *vec, int elem_size);

void free$MODvec(struct Vec *vec);

void push$MODvec(struct Vec *vec, const void *elem);

void* get$MODvec(struct Vec *vec, int index);

int size$MODvec(struct Vec *vec);