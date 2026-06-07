#define YSEXPORT __attribute__((visibility("default")))
#define ITERATION_END -1

#include <stdint.h>
#include <stdio.h>
#include <sys/random.h>

#include "vector.h"
#include "mem.h"
#include "math.h"

static uint32_t get_secure_random_uint32() {
  uint32_t num;
  ssize_t result = getrandom(&num, sizeof(num), 0);
  if (result != sizeof(num)) {
    perror("getrandom failed");
    return 0;
  }
  return num;
}

static int randi(int min, int max) {
  uint32_t r = get_secure_random_uint32();
  return (r % (max - min + 1)) + min;
}

static double randf(double min, double max, int decimals) {
  uint32_t r = get_secure_random_uint32();
  double normalized = (double)r / (double)UINT32_MAX;
  double scaled = min + normalized * (max - min);

  double factor = pow(10.0, decimals);
  return round$MODmath(scaled * factor, decimals) / factor;
}

typedef struct {
    int min;
    int max;
    Vec *internal_data;
    Vec *copies;
} GeneratorInt;

YSEXPORT GeneratorInt *new$MODGeneratorInt() {
    GeneratorInt *self = (GeneratorInt *)alloc$MODmem(sizeof(GeneratorInt));

    Vec *internal_data = new$MODVec(sizeof(int));
    Vec *copies = new$MODVec(sizeof(Vec *));

    self->internal_data = internal_data;
    self->copies = copies;

    return self;
}

YSEXPORT void gen$MODGeneratorInt(GeneratorInt *self, int n, int min, int max) {
    for (int i = 0; i < n; i++) {
        push_int$MODVec(self->internal_data, (int)randi(min, max));
    }

    push_vec$MODVec(self->copies, self->internal_data);
    self->min = min;
    self->max = max;
}

YSEXPORT void gen_new$MODGeneratorInt(GeneratorInt *self, int n, int min, int max) {
    erase_all$MODVec(self->internal_data);
    for (int i = 0; i < n; i++) {
        push_int$MODVec(self->internal_data, (int)randi(min, max));
    }

    push_vec$MODVec(self->copies, self->internal_data);
    self->min = min;
    self->max = max;
}

YSEXPORT Vec *get_data$MODGeneratorInt(GeneratorInt *self) {
    return self->internal_data;
}

YSEXPORT Vec *gen_and_get$MODGeneratorInt(GeneratorInt *self, int n, int min, int max) {
    gen$MODGeneratorInt(self, n, min, max);
    return get_data$MODGeneratorInt(self);
}

YSEXPORT Vec *gen_and_get_new$MODGeneratorInt(GeneratorInt *self, int n, int min, int max) {
    gen_new$MODGeneratorInt(self, n, min, max);
    return get_data$MODGeneratorInt(self);
}

YSEXPORT int get_at$MODGeneratorInt(GeneratorInt *self, int i) {
    if (i < 0 || i >= self->internal_data->size) {
        fprintf(stderr, "GeneratorInt::get_at: index out of range\n");
        return -1;
    }

    return get_int$MODVec(self->internal_data, i);
}

YSEXPORT int iterate$MODGeneratorInt(GeneratorInt *self, int *i) {
    if (*i >= self->internal_data->size) {
        *i = ITERATION_END;
        return ITERATION_END;
    }

    return get_at$MODGeneratorInt(self, *i);
}

YSEXPORT void destroy$MODGeneratorInt(GeneratorInt *self) {
    destroy$MODVec(self->copies);
    destroy$MODVec(self->internal_data);
    free$MODmem(self);
}