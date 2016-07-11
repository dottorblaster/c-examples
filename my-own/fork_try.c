#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main() {
  printf("Pre-fork()\n");
  pid_t p = fork();
  printf("Post-fork()\n");
  printf("Altro post-fork()\n");
  if (p == 0) {
    printf("0 found :-)\n");
  } else {
    printf("Else :-(\n");
  }
}
