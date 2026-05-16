/*
 * yasos - a compiler for yasos language
 * Copyright (c) 2025-2026 DameChocolateYa
 * Licensed under the BSD 3-Clause License.
 * See LICENSE file in the project root for full license text.
*/

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <ftw.h>

#include "mem.h"
#include "vector.h"
#include "string.h"
#include <string.h>

typedef struct {
  char *path;
  int fd;
  char *buffer;
  int bufsize;
  int pos;
  int flags;
  bool is_open;
} File;

typedef struct {
  String *path;
  bool destroy_on_exit; // Delete the directory when File::destroy is called
} Dir;

extern int sys_open(const char *path, int flags, int mode);
extern int sys_unlink(const char *path);
extern void sys_close(const int fd);
extern int sys_write(int fd, const char *data, int bytes);
extern int sys_read(int fd, char *buffer, int bytes);
extern int sys_seek(int fd, int offset, int whence);
extern void sys_fsync(int fd);

bool directory_exists_internal(const char *path) {
  struct stat buffer;

  if (stat(path, &buffer) == 0) {
    return S_ISDIR(buffer.st_mode);
  }

  return 0;
}

int mkdir_internal(const char *path) {
  if (mkdir(path, 0755) == -1) {
    perror("Error creating directory");
    return 1;
  }

  return 0;
}

static int erase_directory_recursive_internal(const char *path, const struct stat *sb, int type_flag, struct FTW *ftwbuf) {
  return remove(path);
}

// Filesys (File section)
__attribute__((visibility("default")))
bool touch$MODFilesys(const char *path) {
  if (!path) return false;

  int fd = sys_open(path, 1 | 64, 0644);
  if (fd <= 0) {
    fprintf(stderr, "Filesys::touch: Could not create file\n");
    return false;
  }

  sys_close(fd);

  return true;
}

__attribute__((visibility("default")))
bool erase$MODFilesys(const char *path) {
  return sys_unlink(path);
}

// File implementation equivalent

// The File flags system only works in GNU/Linux

__attribute__((visibility("default")))
File *open$MODFile(char *path, const char *raw_flags) {
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

  File *f = (File *)alloc$MODmem(sizeof(File));
  f->path = path;
  f->is_open = true;
  f->fd = sys_open(path, flags, 0644);

  if (f->fd < 0) {
    fprintf(stderr, "Could not open file (%s)\n", path);
    f->is_open = false;
    return f;
  }

  f->bufsize = 4096;
  f->buffer = (char *)alloc$MODmem(f->bufsize);
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
bool touch$MODFile(File *self) {
  if (!self || self->is_open) return false;

  self->fd = sys_open(self->path, self->flags, 0644);
  self->is_open = true;

  if (!self->buffer) {
    self->bufsize = 4096;
    self->buffer = (char *)alloc$MODmem(self->bufsize);
    cpy$MODstring(self->buffer, "");
  }

  return true;
}

__attribute__((visibility("default")))
bool erase$MODFile(File *self) {
  if (!self->is_open) return true;
  else if (self->fd <= 0) return false;

  self->is_open = false;
  return sys_unlink(self->path);
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
 return "a";
}

__attribute__((visibility("default")))
Vec *read_lines$MODFile(File *self) {
  Vec *lines = new$MODVec(sizeof(String*));
  if (self->fd < 0) {
    fprintf(stderr, "File::read_lines: Tried to read lines from a NULL File\n");
    return lines;
  }

  if (!self->buffer) {
    fprintf(stderr, "File::read_lines: Tried to read lines from a NULL buffer in a File\n");
    return lines;
  }

  char *start = self->buffer;
  char *newline;

  while ((newline = strchr(start, '\n')) != NULL) {
    *newline = '\0';
    String *line = from$MODString(start);
    push_string$MODVec(lines, line);

    start = newline + 1;
  }

  if (*start != '\0') {
    String *line = from$MODString(start);
    push_string$MODVec(lines, line);
  }

  return lines;
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

__attribute__((visibility("default")))
bool mkdir$MODFilesys(const char *path) { // HEY! this does not require Dir struct...
  return mkdir_internal(path);
}

__attribute__((visibility("default")))
bool erase_dir_recursive$MODFilesys(const char *path) {
  return nftw(path, erase_directory_recursive_internal, 64, FTW_DEPTH | FTW_PHYS) == 0;
}

__attribute__((visibility("default")))
bool exist$MODDir(Dir *self) {
  if (!self || !self->path) return false;

  return directory_exists_internal(to_str$MODString(self->path));
}

__attribute__((visibility("default")))
Dir *new$MODDir(const char *path, const char *flags) {
  Dir *dir = (Dir *)alloc$MODmem(sizeof(Dir));
  if (!dir || !path || !flags) {
    fprintf(stderr, "Dir::new: Failed to create Dir instance\n");
    return NULL;
  }

  dir->path = from$MODString(path);
  if (!dir->path) {
    fprintf(stderr, "Dir::new: Could not create a String instance of path\n");
    return NULL;
  }

  if (!exist$MODDir(dir) && strchr(flags, 'c')) {
    mkdir_internal(path);
  }

  return dir;
}

__attribute__((visibility("default")))
bool mkdir$MODDir(Dir *self) { // Creates the directory in the File::path
  if (!self) return false;
  
  if (exist$MODDir(self)) return true;
  return mkdir_internal(to_str$MODString(self->path));
}

__attribute__((visibility("default")))
bool erase_recursive$MODDir(Dir *self) {
  if (!self || !exist$MODDir(self)) return false;

  return nftw(to_str$MODString(self->path), erase_directory_recursive_internal, 64, FTW_DEPTH | FTW_PHYS) == 0;
}

__attribute__((visibility("default")))
void destroy$MODDir(Dir *self) {
  destroy$MODString(self->path);
  free$MODmem(self);
}