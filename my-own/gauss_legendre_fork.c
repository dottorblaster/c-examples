#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

int pipe12[2], pipe23[2];

void error_exit(char *err) {
	printf("Error: %s\n", err);
	exit(EXIT_FAILURE);
}

// TODO
void send_val(int fd, double val) {}

// TODO
void recv_val(int fd, double *val) {}

void init() {
	if (pipe(pipe12) == -1 || pipe(pipe23) == -1) { error_exit("Error in pipe creation"); }
}

// TODO
void process1() {
	double a = 1.0, b = 1.0/sqrt(2), new_a, new_b;
	if (close(pipe23[0]) != 0 || close(pipe23[1]) != 0 || close(pipe12[0]) != 0) { error_exit("Error in closing pipe endpoints"); }
	for(;;) {
		new_a = (a+b)/2.0;
		new_b = sqrt(a*b);
	}
}

// TODO
void process2() {}

// TODO
void process3() {}

int main() {
	pid_t p;
	init();
	p = fork();
	if (p == 0) { process1(); }
	p = fork();
	if (p == 0) { process2(); }
	process3();
}
