#include <cstdlib>
#include <unistd.h>

void kill(const int process_id) {
  system("kill " + process_id);
}

void advanced_kill(const char* process_name) {
  system("pkill " + process_name);
}

void killall(const char* process_name) {
  system("killall " + process_name);
}
