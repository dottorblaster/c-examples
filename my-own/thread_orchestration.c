#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

struct slave_thread_t {
  pthread_t tid;
  pthread_mutex_t mtx;
  pthread_cond_t got_new_word;
  int turn;
  char *word;
  struct node_t *list_head;
};

struct node_t {
  char *word;
  struct node_t *next;
};

int main() {
  struct slave_thread_t slaves[26];
  create_threads(slaves);
  read_words_from_stdin(slaves);
  print_word_lists(slaves);
  pthread_exit(0);
}
