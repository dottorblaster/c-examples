#include <stdio.h>
#include <stdlib.h>

void main() {
  printf("Pre-fork()\n");
  fork();
  printf("Post-fork()\n");
  printf("Altro post-fork()\n");
}
