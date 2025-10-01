// mem.c
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct block {
  size_t size;
  struct block *next;
  struct block *prev;
  int free;
  uint32_t magic;
} block_t;

#define HEADER_SIZE (sizeof(block_t))
#define ALIGN8(x) (((x) + 7) & ~((size_t)7))
#define MAGIC 0xDEADBEEF
#define FREED_MAGIC 0xBEEFDEAD

static block_t *base = NULL;
static pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

/* Helpers */
static block_t *find_block(block_t **last, size_t size) {
  block_t *b = base;
  while (b && !(b->free && b->size >= size)) {
    *last = b;
    b = b->next;
  }
  return b;
}

static block_t *extend_heap(block_t *last, size_t size) {
  void *request = sbrk(0);
  if (sbrk(HEADER_SIZE + size) == (void *)-1)
    return NULL;
  block_t *b = (block_t *)request;
  b->size = size;
  b->next = NULL;
  b->prev = last;
  b->free = 0;
  b->magic = MAGIC;
  if (last)
    last->next = b;
  return b;
}

static void split_block(block_t *b, size_t size) {
  // b is big; create new header after allocated payload
  block_t *newb = (block_t *)((char *)(b + 1) + size);
  newb->size = b->size - size - HEADER_SIZE;
  newb->next = b->next;
  newb->prev = b;
  newb->free = 1;
  newb->magic = MAGIC;
  b->size = size;
  b->next = newb;
  if (newb->next)
    newb->next->prev = newb;
}

static block_t *fusion(block_t *b) {
  if (b->next && b->next->free) {
    b->size += HEADER_SIZE + b->next->size;
    b->next = b->next->next;
    if (b->next)
      b->next->prev = b;
  }
  return b;
}

/* API */
__attribute__((visibility("default")))
void *memalloc$MODmem(size_t size) {
  if (size == 0)
    return NULL;
  size = ALIGN8(size);

  pthread_mutex_lock(&global_lock);

  block_t *b;
  if (!base) {
    b = extend_heap(NULL, size);
    if (!b) {
      pthread_mutex_unlock(&global_lock);
      return NULL;
    }
    base = b;
  } else {
    block_t *last = base;
    b = find_block(&last, size);
    if (b) {
      if (b->size - size >= (HEADER_SIZE + 8))
        split_block(b, size);
      b->free = 0;
      b->magic = MAGIC;
    } else {
      b = extend_heap(last, size);
      if (!b) {
        pthread_mutex_unlock(&global_lock);
        return NULL;
      }
    }
  }
  pthread_mutex_unlock(&global_lock);
  return (void *)(b + 1); // payload pointer
}

__attribute__((visibility("default")))
void memfree$MODmem(void *ptr) {
  if (!ptr)
    return;

  pthread_mutex_lock(&global_lock);

  block_t *b = (block_t *)ptr - 1;
  if (b->magic != MAGIC) {
    fprintf(stderr, "myfree: pointer appears invalid (magic=0x%x)\n", b->magic);
    pthread_mutex_unlock(&global_lock);
    return;
  }
  if (b->free) {
    fprintf(stderr, "myfree: double free detected\n");
    pthread_mutex_unlock(&global_lock);
    return;
  }

  b->free = 1;

  memset((void *)(b + 1), 0xDD, b->size);

  b->magic = FREED_MAGIC;

  // coalesce prev/next libres
  if (b->prev && b->prev->free)
    b = fusion(b->prev);
  if (b->next)
    fusion(b);

  if (b->next == NULL) {
    if (b->prev)
      b->prev->next = NULL;
    else
      base = NULL;
    if (sbrk(-(HEADER_SIZE + b->size)) == (void *)-1) {
    }
  }

  pthread_mutex_unlock(&global_lock);
}

__attribute__((visibility("default")))
void *memrealloc$MODmem(void *ptr, size_t size) {
  if (!ptr)
    return memalloc$MODmem(size);
  if (size == 0) {
    memfree$MODmem(ptr);
    return NULL;
  }

  size = ALIGN8(size);
  block_t *b = (block_t *)ptr - 1;

  pthread_mutex_lock(&global_lock);
  if (b->magic != MAGIC) {
    pthread_mutex_unlock(&global_lock);
    fprintf(stderr, "myrealloc: invalid pointer\n");
    return NULL;
  }

  if (b->size >= size) {
    if (b->size - size >= (HEADER_SIZE + 8))
      split_block(b, size);
    pthread_mutex_unlock(&global_lock);
    return ptr;
  }

  if (b->next && b->next->free && (b->size + HEADER_SIZE + b->next->size) >= size) {
    fusion(b);
    if (b->size - size >= (HEADER_SIZE + 8))
      split_block(b, size);
    b->free = 0;
    b->magic = MAGIC;
    pthread_mutex_unlock(&global_lock);
    return ptr;
  }
  size_t old_size = b->size;
  pthread_mutex_unlock(&global_lock);

  void *newp = memalloc$MODmem(size);
  if (!newp)
    return NULL;
  memcpy(newp, ptr, old_size);
  memfree$MODmem(ptr);
  return newp;
}

__attribute__((visibility("default")))
void *memcalloc$MODmem(size_t nmemb, size_t size) {
  size_t total = nmemb * size;
  void *p = memalloc$MODmem(total);
  if (p)
    memset(p, 0, total);
  return p;
}
