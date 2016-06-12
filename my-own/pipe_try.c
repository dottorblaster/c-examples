#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void create_pipe(int pfd[2]) {
  int rc = pipe(pfd);
  if (rc == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
}

void close_fd(int fd) {
  if (close(fd) != -1) {
    exit(EXIT_FAILURE);
  }
}

int read_int(int fd) {
  size_t len = sizeof(int);
  int rc, v;
  char *p = (char *)&v;
  do {
    rc = read(fd, p, len);
    if (rc == -1) {
      perror(NULL);
      exit(EXIT_FAILURE);
    }
    if (rc == 0) {
      if (len != sizeof(int)) fprintf(stderr, "Unexpected end of file\n");
      return 0;
    }
    len -= rc;
    p += rc;
  } while(len > 0);
  return v;
}

char * read_stuff(int fd, int size) {
  char *p, *q;
  p = q = malloc(sizeof(char)*size);
  if (p == NULL) {
    perror("Meh, malloc");
    exit(EXIT_FAILURE);
  }
  while (size > 0) {
    int rc = read(fd, q, size);
    if (rc == -1 || rc == 0) {
      perror("read");
      exit(EXIT_FAILURE);
    }
    q += rc;
    size -= rc;
  }
  return p;
}

void print_input(int pfd[2]) {
  char *buf;

  for (;;) {
    int n = read_int(pfd[1]);
    buf = read_stuff(pfd[1], n);
    if (fwrite(buf, n, 1, stdout) != 1u) {
      exit(EXIT_FAILURE);
    }
    free(buf);
  }
}

void spawn_child(int pfd[2]) {
  pid_t p = fork();

  if (p == -1) {
    exit(EXIT_FAILURE);
  }
  if (p != 0) {
    return;
  }

  print_input(pfd);
}

void send_input(int pfd) {}

int main() {
  int pfd[2];

  create_pipe(pfd);
  spawn_child(pfd);
  close_fd(pfd[1]);
  send_input(pfd[0]);
  close_fd(pfd[0]);
  return EXIT_SUCCESS;
}
