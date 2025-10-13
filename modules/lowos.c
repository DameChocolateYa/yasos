#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#include "std.h"
#include "string.h"

__attribute__((visibility("default")))
int execute$MODos(const char* cmd) {
  if (cmd == NULL) return 1;

  pid_t pid = fork();

  if (pid < 0) {
    perror$MODstd("ERROR: fork\n");
    return -1;
  }

  if (pid == 0) {
    execl("/bin/sh", "sh", "-c", cmd, (char*)NULL);
    perror$MODstd("ERROR: execl\n");
    _exit(127);
  }

  int status;
  if (waitpid(pid, &status, 0) == -1) {
    perror$MODstd("ERROR: waitpd");
    return -1;
  }

  if (WIFEXITED(status)) return WEXITSTATUS(status);
  else if (WIFSIGNALED(status)) return 128 + WTERMSIG(status);
  
  return -1;
}

extern char **environ;
__attribute__((visibility("default")))
char *getenv$MODos(const char *var) {
  if (!var || *var == '\0') return NULL;

  int len = len$MODstring(var);

  for (char **env = environ; *env != NULL; env++) {
    if (strncmp(*env, var, len) == 0 && (*env)[len] == '=') {
      return *env + len + 1;
    }
  }

  return NULL;
}