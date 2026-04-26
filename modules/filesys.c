#include <stdio.h>
#include "mem.h"
#include "string.h"
#include <string.h>

typedef struct {
  int fd;
  char *buffer;
  int bufsize;
  int pos;
  int flags;
  int is_open;
} File;

extern int sys_open(const char *path, int flags, int mode);
extern void sys_close(const int fd);
extern int sys_write(int fd, const char *data, int bytes);
extern int sys_read(int fd, char *buffer, int bytes);
extern int sys_seek(int fd, int offset, int whence);
extern void sys_fsync(int fd);

// File implementation equivalent

// The File flags system only works in GNU/Linux

__attribute__((visibility("default")))
File *open$MODFile(const char *path, const char *raw_flags) {
  int flags = 0;

  if (strchr(raw_flags, 'r') && strchr(raw_flags, 'w')) {
    flags |= 2; // Writable and readable
  } else if (strchr(raw_flags, 'r')) {
    flags |= 0; // Read only
  } else if (strchr(raw_flags, 'w')) {
    flags |= 1; // Write only
  }

  if (strchr(raw_flags, 'c')) flags |= 64; // CREATE
  if (strchr(raw_flags, 'e')) flags |= 128; // EXCL
  if (strchr(raw_flags, 't')) flags |= 512; // TRUNC
  if (strchr(raw_flags, 'a')) flags |= 1024; // APPEND

  File *f = alloc$MODmem(sizeof(File));
  f->is_open = true;
  f->fd = sys_open(path, flags, 0644);

  if (f->fd < 0) {
    fprintf(stderr, "Could not open file (%s)\n", path);
    f->is_open = false;
    return f;
  }

  f->bufsize = 4096;
  f->buffer = alloc$MODmem(f->bufsize);
  if (!f->buffer) {
    fprintf(stderr, "Could not allocate memory for File management\n");
    f->is_open = false;
    return f;
  }

  cpy$MODstring(f->buffer, "");

  f->pos = 0;
  f->flags = flags;

  return f;
}

__attribute__((visibility("default")))
void close$MODFile(File *self) {
  if (self->fd >= 0) {
    sys_close(self->fd);
  }

  if (self->buffer) {
    free$MODmem(self->buffer);
  }

  self->fd = -1;
  self->buffer = NULL;
  self->is_open = false;
  self->bufsize = 0;
  self->flags = 0;
  self->pos = 0;

  free$MODmem(self);
}

__attribute__((visibility("default")))
int is_open$MODFile(File *self) {
  return self->is_open;
}

__attribute__((visibility("default")))
int write$MODFile(File *self, const char *data) {
  if (self == NULL || self->fd < 0 || self->buffer == NULL) {
    fprintf(stderr, "File::write: Tried to write in a NULL File\n");
    return -1;
  }

  if (!data) {
    fprintf(stderr, "File::write: invaled data\n");
    return -1;
  }

  int len = len$MODstring(data);

  int bytes = sys_write(self->fd, data, len);

  return bytes;
}

__attribute__((visibility("default")))
char *read$MODFile(File *self) {
  if (self->fd < 0) {
    fprintf(stderr, "File::read: Tried to read bytes from a NULL File\n");
    return "(null)";
  }

  if (!self->buffer) {
    fprintf(stderr, "File::read: Tried to read bytes from a NULL buffer in a File\n");
    return "(null)";
  }

  int bytes_readed = sys_read(self->fd, self->buffer, self->bufsize);
  if (bytes_readed < 0) {
    perror("sys_read");
    return NULL;
  }

  if (bytes_readed >= 0 && bytes_readed < self->bufsize) {
    self->buffer[bytes_readed] = '\0';  
  }

  return self->buffer;
}

__attribute__((visibility("default")))
void seek$MODFile(File *self, int pos, int whence) {
  if (self->fd < 0) {
    fprintf(stderr, "File::seek: Tried to do operations in a NULL File\n");
    return;
  }

  int new_pos = sys_seek(self->fd, pos, whence);
  if (new_pos < 0) {
    fprintf(stderr, "File::seek: Failed to seek\n");
  }
}

__attribute__((visibility("default")))
int ssize$MODFile() {
  return sizeof(File);
}