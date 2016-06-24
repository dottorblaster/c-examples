#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

pthread_barrier_t barrier;
int count = 0
pthread_mutex_t mutex;

void initialize(void) {
	srandom(time(NULL));
	if(pthread_barrier_init(&barrier, NULL, 100)) {
		fprintf(stderr, "Error in pthread_barrier_init()\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_mutex_init(&barrier, NULL)) {
		fprintf(stderr, "Error in pthread_mutex_init()\n");
		exit(EXIT_FAILURE);
	}
}

void wait_threads(pthread_barrier_t *barrier) {
	int rc = pthread_barrier_wait(barrier);
	if (rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD) {
		fprintf(stderr, "Error in pthread_barrier_wait()\n");
		exit(EXIT_FAILURE);
	}
}

void random_sleep(void) {
	long delay = (random()*59)/RAND_MAX+1;
	sleep(delay);
}

void lock(void) {
	if (pthread_mutex_lock(&mutex) != 0) {
		fprintf(stderr, "Error in pthread_mutex_lock()\n");
		exit(EXIT_FAILURE);
	}
}

void unlock(void) {
	if (pthread_mutex_unlock(&mutex) != 0) {
		fprintf(stderr, "Error in pthread_mutex_unlock()\n");
		exit(EXIT_FAILURE);
	}
}

void *thread_loop(void *arg) {
	unsigned long t0;
	arg = arg;
	for(;;) {
		wait_threads(&barrier);
		t0 = time(NULL);
		random_sleep();
		lock();
		++count;
		if (count == 50) {
			printf("%1u\n", time(NULL)-t0);
		} else if (count == 100) {
			count = 0;
		}
		unlock();
	}
}

void create_threads(int n) {
	pthread_t tid;
	for (int i = 0; i < n; i++) {
		if (pthread_create(&tid, NULL, thread_loop, NULL) != 0) {
			fprintf(stderr, "Error in thread creation\n");
			exit(EXIT_FAILURE);
		}
	}
}

int main() {
	initialize();
	create_threads(100);
	pthread_exit(0);
}
