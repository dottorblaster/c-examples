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

void write_buf(int fd, char *rec, int size) {
  while (size > 0) {
    int rc = write(fd, rec, size);
    if (rc == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }
    rec += rc;
    size -= rc;
  }
}

void spawn_child(int pfd[2]) {
  pid_t p = fork();
  if (p == -1) {
    perror(NULL);
    exit(EXIT_FAILURE);
  }
  if (p != 0) {
    return;
  }
  print_records(pfd);
}

void close_fd(int fd) {
  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }
}

int open_file(const char *name) {
  int fd = open(name, O_RDWR);
  if (fd == -1) {
    perror(name);
    exit(EXIT_FAILURE);
  }
  return fd;
}

char * read_record(int fd, int size) {
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

void invert_bytes(char *rec, int len) {
  char *last;
  for (last = rec+(len-1); rec < last; --last) {
    char c = *rec;
    *rec = *last;
    *last = c;
  }
}

void send_records(int ifd, int ofd) {
  for (;;) {
    char *buf;
    int n = read_int(ifd);
    if (n == 0) return;
    buf = read_record(ifd, n);
    write_buf(ofd, (char *)&n, sizeof(n));
    write_buf(ofd, buf, n);
    free(buf);
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

void print_records(int pfd[2]) {
  char *buf;
  close_fd(pfd[1]);
  for(;;) {
    int n = read_int(pfd[0]);
    if (n == 0) exit(EXIT_SUCCESS);
    buf = read_record(pfd[0], n);
    printf("--- START OF RECORD ---\n");
    if (fwrite(buf, n, 1, stdout) != 1u) {
      perror("stdout");
      exit(EXIT_FAILURE);
    }
    printf("\n--- END OF RECORD ---\n");
    free(buf);
  }
}

int main(int argc, char const *argv[]) {
  int pfd[2], fd;
  if (argc != 2) {
    fprintf("Usage: %s <file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  create_pipe(pfd);
  spawn_child(pfd);
  close_fd(pfd[0]);
  fd = open_file(argv[1]);
  send_records(fd, pfd[1]);
  close_fd(pfd[1]);
  close_fd(fd);
  return EXIT_SUCCESS;
}
