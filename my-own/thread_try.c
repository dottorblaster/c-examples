#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *slave_fn1(void *v) {
  size_t l;
  for (l = 0; l < 5; l++) {
    printf("From the thread with love\n");
  }
  pthread_exit(NULL);
}

int main() {
  pthread_t tid1;
  pthread_t tid2;
  size_t i;
  void *tret;
  printf("Creating thread\n");
  pthread_create(tid1, NULL, slave_fn1, NULL);
  printf("Thread created 1\n");
  printf("Thread created 2\n");
  for (i = 0; i < 5; i++) {
    printf("Main!\n");
  }
  return 0;
}
