#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main() {
  printf("Pre-fork()\n");
  pid_t p = fork();
  printf("Post-fork()\n");
  printf("Altro post-fork()\n");
  printf("%s\n", p);
}
