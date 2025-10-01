#include "math.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/random.h>
#include <sys/syscall.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define SYS_fork 57
#define SYS_execve 59
#define SYS_wait4 61
#define SYS_exit 60

#define true 1
#define false 0

void sys_exit(int) __attribute__((visibility("hidden")));
void sys_write(int, char*, int) __attribute__((visibility("hidden")));
void sys_putchar(int, char) __attribute__((visibility("hidden")));
unsigned int sys_sleep(double) __attribute__((visibility("hidden")));

//long syscall(long number, ...); // prototype to avoid libc warning

__attribute__((visibility("default")))
char *format_std(const char *format, ...) {
  char *result;
  va_list args;

  va_start(args, format);
  if (vasprintf(&result, format, args) == -1) {
    result = NULL;
  }
  va_end(args);

  return result;
}

__attribute__((visibility("default")))
void bufformat$MODstd(const char **buf_and_format, ...) {
  char *result;
  va_list args;

  va_start(args, *buf_and_format);
  if (vasprintf(&result, *buf_and_format, args) == -1) {
    result = NULL;
  }
  va_end(args);

  *buf_and_format = result;
}

#include <stdarg.h>
#include <stdint.h>
#include <string.h>

void *memalloc$MODmem(int size);
void *memrealloc$MODmem(void *ptr, int size);
void memfree$MODmem(void *ptr);

static int uint64_to_str(uint64_t n, int base, char *buf, int bufsize) {
  const char *digits = "0123456789abcdef";
  char tmp[32];
  int i = 0;
  if (n == 0) {
    if (bufsize > 0)
      buf[0] = '0';
    return 1;
  }
  while (n > 0 && i < 32) {
    tmp[i++] = digits[n % base];
    n /= base;
  }
  int len = 0;
  while (i > 0 && len < bufsize) {
    buf[len++] = tmp[--i];
  }
  return len;
}

static int int_to_str(int n, char *buf, int bufsize) {
  if (n < 0) {
    if (bufsize > 0)
      buf[0] = '-';
    int len = uint64_to_str((uint64_t)(-n), 10, buf + 1, bufsize - 1);
    return len + 1;
  } else {
    return uint64_to_str((uint64_t)n, 10, buf, bufsize);
  }
}

static int ptr_to_str(void *ptr, char *buf, int bufsize) {
  if (bufsize < 3)
    return 0;
  buf[0] = '0';
  buf[1] = 'x';
  int len = uint64_to_str((uintptr_t)ptr, 16, buf + 2, bufsize - 2);
  return len + 2;
}

static int double_to_str(double val, int precision, char *buf, int bufsize) {
  if (precision < 0)
    precision = 6;

  int len = 0;
  if (val < 0) {
    if (len < bufsize)
      buf[len++] = '-';
    val = -val;
  }

  long long intpart = (long long)val;
  double frac = val - (double)intpart;

  char tmp[64];
  int intlen = int_to_str(intpart, tmp, sizeof(tmp));
  if (len + intlen >= bufsize)
    intlen = bufsize - len;
  memcpy(buf + len, tmp, intlen);
  len += intlen;

  if (precision > 0 && len < bufsize) {
    buf[len++] = '.';
    for (int i = 0; i < precision && len < bufsize; i++) {
      frac *= 10.0;
      int digit = (int)frac;
      buf[len++] = '0' + digit;
      frac -= digit;
    }
  }

  if (len < bufsize)
    buf[len] = '\0';
  return len;
}

__attribute__((visibility("default")))
char *strbuf$MODstd(const char *fmt, int *len_r, va_list args) {
  int capacity = 256;
  char *out = (char *)memalloc$MODmem(capacity);
  if (!out)
    return NULL;
  int pos = 0;

  const char *p = fmt;
  while (*p) {
    int precision = 6;

    if (*p != '%') {
      if (pos >= capacity) {
        capacity *= 2;
        out = (char *)memrealloc$MODmem(out, capacity);
      }
      out[pos++] = *p++;
      continue;
    }

    p++; // saltamos '%'

    // precisión opcional
    if (*p == '.') {
      p++;
      precision = 0;
      while (*p >= '0' && *p <= '9') {
        precision = precision * 10 + (*p - '0');
        p++;
      }
    }

    if (*p == '\0')
      break;

    char tmp[128];
    int len = 0;

    switch (*p) {
    case 's': {
      char *s = va_arg(args, char *);
      if (!s)
        s = "(nil)";
      for (int i = 0; s[i]; i++) {
        if (pos >= capacity) {
          capacity *= 2;
          out = (char *)memrealloc$MODmem(out, capacity);
        }
        out[pos++] = s[i];
      }
      break;
    }
    case 'd': {
      int d = va_arg(args, int);
      len = int_to_str(d, tmp, sizeof(tmp));
      for (int i = 0; i < len; i++) {
        if (pos >= capacity) {
          capacity *= 2;
          out = (char *)memrealloc$MODmem(out, capacity);
        }
        out[pos++] = tmp[i];
      }
      break;
    }
    case 'x': {
      unsigned int x = va_arg(args, unsigned int);
      len = uint64_to_str(x, 16, tmp, sizeof(tmp));
      for (int i = 0; i < len; i++) {
        if (pos >= capacity) {
          capacity *= 2;
          out = (char *)memrealloc$MODmem(out, capacity);
        }
        out[pos++] = tmp[i];
      }
      break;
    }
    case 'p': {
      void *ptr = va_arg(args, void *);
      len = ptr_to_str(ptr, tmp, sizeof(tmp));
      for (int i = 0; i < len; i++) {
        if (pos >= capacity) {
          capacity *= 2;
          out = (char *)memrealloc$MODmem(out, capacity);
        }
        out[pos++] = tmp[i];
      }
      break;
    }
    case 'f': {
      double f = va_arg(args, double);
      len = double_to_str(f, precision, tmp, sizeof(tmp));
      for (int i = 0; i < len; i++) {
        if (pos >= capacity) {
          capacity *= 2;
          out = (char *)memrealloc$MODmem(out, capacity);
        }
        out[pos++] = tmp[i];
      }
      break;
    }
    case 'c': {
      char c = (char)va_arg(args, int);
      if (pos >= capacity) {
        capacity *= 2;
        out = (char *)memrealloc$MODmem(out, capacity);
      }
      out[pos++] = c;
      break;
    }
    case '%': {
      if (pos >= capacity) {
        capacity *= 2;
        out = (char *)memrealloc$MODmem(out, capacity);
      }
      out[pos++] = '%';
      break;
    }
    default: {
      if (pos >= capacity) {
        capacity *= 2;
        out = (char *)memrealloc$MODmem(out, capacity);
      }
      out[pos++] = '%';
      if (pos >= capacity) {
        capacity *= 2;
        out = (char *)memrealloc$MODmem(out, capacity);
      }
      out[pos++] = *p;
      break;
    }
    }

    p++; // avanzar tras especificador
  }

  if (pos >= capacity) {
    capacity += 1;
    out = (char *)memrealloc$MODmem(out, capacity);
  }
  out[pos] = '\0';
  *len_r = pos;
  return out;
}

__attribute__((visibility("default")))
void print$MODstd(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int len;
  char *s = strbuf$MODstd(fmt, &len, args);
  va_end(args);

  sys_write(1, s, len);

  memfree$MODmem(s);
}

__attribute__((visibility("default")))
void println$MODstd(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int len;
  char *s = strbuf$MODstd(fmt, &len, args);
  va_end(args);

  sys_write(1, s, len);

  memfree$MODmem(s);
  sys_putchar(1, '\n');
}

__attribute__((visibility("default")))
void put_char$MODstd(const char c) {
  sys_putchar(1, c);
}

__attribute__((visibility("default")))
void prerror$MODstd(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int len;
  char *s = strbuf$MODstd(fmt, &len, args);
  va_end(args);

  sys_write(2, s, len);

  memfree$MODmem(s);
}

__attribute__((visibility("default")))
void panic$MODstd(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int len;
  char *s = strbuf$MODstd(fmt, &len, args);
  va_end(args);

  sys_write(2, s, len);

  memfree$MODmem(s);
  
  sys_exit(1);
}

__attribute__((visibility("default")))
void cuspanic$MODstd(const char *fmt, int status, ...) {
  va_list args;
  va_start(args, status);
  int len;
  char *s = strbuf$MODstd(fmt, &len, args);
  va_end(args);

  sys_write(2, s, len);

  memfree$MODmem(s);
  
  sys_exit(status);
}

__attribute__((visibility("default")))
char *scani$MODstd() {
  char buffer[240];

  if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
    fprintf(stderr, "Error al leer desde stdin\n");
  }
  size_t len = strlen(buffer);
  if (len > 0 && buffer[len - 1] == '\n') {
    buffer[len - 1] = '\0';
  }

  char *result = (char *)memalloc$MODmem(len);
  if (result != NULL) {
    strcpy(result, buffer);
  }

  return result;
}

__attribute__((visibility("default")))
char *ask$MODstd(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int msg_len;
  char *s = strbuf$MODstd(fmt, &msg_len, args);
  va_end(args);

  sys_write(2, s, msg_len);
  memfree$MODmem(s);

  char buffer[240];

  if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
    fprintf(stderr, "Error trying to read frpm stdin\n");
    return NULL;
  }
  size_t len = strlen(buffer);
  if (len > 0 && buffer[len - 1] == '\n') {
    buffer[len - 1] = '\0';
  }

  char *result = (char *)memalloc$MODmem(len);
  if (result != NULL) {
    strcpy(result, buffer);
  }

  return result;
}

__attribute__((visibility("default")))
int is_int$MODstd(const char *s1) {
  if (s1[0] == '\0')
    return false;
  for (int i = 0; s1[i] != '\0'; ++i) {
    if (s1[i] != '-' && (s1[i] < '0' || s1[i] > '9')) {
      return false;
    }
    if (i > 0 && s1[i] == '-') {
      return false;
    }
  }
  return true;
}

__attribute__((visibility("default")))
int is_decimal$MODstd(const char *s1) {
  if (s1[0] == '\0')
    return false;
  int decimal = false;
  for (int i = 0; s1[i] != '\0'; ++i) {
    if (s1[i] != '-' && s1[i] != '.' && (s1[i] < '0' || s1[i] > '9')) {
      return false;
    }
    if ((i > 0 && s1[i] == '-')) {
      return false;
    }
    if (s1[i] == '.') {
      if (!decimal)
        decimal = true;
      else
        return false;
    }
  }
  if (!decimal)
    return false;
  return true;
}

__attribute__((visibility("default")))
int is_num$MODstd(const char *s1) {
  if (s1[0] == '\0')
    return false;
  int decimal = false;
  for (int i = 0; s1[i] != '\0'; ++i) {
    if (s1[i] != '-' && s1[i] != '.' && (s1[i] < '0' || s1[i] > '9')) {
      return false;
    }
    if ((i > 0 && s1[i] == '-')) {
      return false;
    }
    if (s1[i] == '.') {
      if (!decimal)
        decimal = true ;
      else
        return false;
    }
  }
  return true;
}

__attribute__((visibility("default")))
void waitk$MODstd() {
  scani$MODstd();
}

__attribute__((visibility("default")))
void cls$MODstd() {
  printf("\e[1;1H\e[2J");
  fflush(stdout);
}

__attribute__((visibility("default")))
int sysexc$MODstd(const char *command) {
  /*if (command == NULL)
    return 1;
  
  const char path[] = "/bin/sh";
  const char arg0[] = "sh";
  const char arg1[] = "-c";

  const char *argv[] = {arg0, arg1, command, NULL};
  const char term[] = "TERM=xterm-256color";
  const char *envp[] = {term, NULL};

  long pid = syscall(SYS_fork);
  if (pid == 0) {
    syscall(SYS_execve, path, argv, envp);
    syscall(SYS_exit, 1);
  } else if (pid > 0) {
    int status;
    syscall(SYS_wait4, pid, 0, 0, 0);

    if ((status & 0x7f) == 0) {
      int exit_code = (status >> 8) & 0xff;
      return exit_code;
    } else {
      return 1;
    }
  } else {
    return 1;
  }
  return 1;*/
}

__attribute__((visibility("default")))
void termc$MODstd(const int color) {
  if (color >= 0 && color <= 7) {
    // Set bright text with foreground color
    printf("\033[1;3%dm", color);
  } else {
    // Reset terminal colors
    printf("\033[0m");
  }
}

static uint32_t get_secure_random_uint32() {
  uint32_t num;
  ssize_t result = getrandom(&num, sizeof(num), 0);
  if (result != sizeof(num)) {
    perror("getrandom failed");
    return 0;
  }
  return num;
}

__attribute__((visibility("default")))
int randi$MODstd(int min, int max) {
  uint32_t r = get_secure_random_uint32();
  return (r % (max - min + 1)) + min;
}

__attribute__((visibility("default")))
double randf$MODstd(double min, double max, int decimals) {
  uint32_t r = get_secure_random_uint32();
  double normalized = (double)r / (double)UINT32_MAX;
  double scaled = min + normalized * (max - min);

  double factor = pow(10.0, decimals);
  return round(scaled * factor, decimals) / factor;
}

__attribute__((visibility("default")))
unsigned int nap$MODstd(double secs) { // Wrapper for sys_sleep
  return sys_sleep(secs);
}