#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct node_t {
	unsigned long long value;
	struct node_t *next;
};

struct node_t *head;
struct node_t *ptr_last;
struct node_t *ptr_safe;

pthread_mutex_t mtx_last;
pthread_mutex_t mtx_safe;

pthread_cond_t cond_last;
pthread_cond_t cond_safe;

void error_exit(char *err) {
	printf("Error: %s\n", err);
	exit(EXIT_FAILURE);
}

void lock(pthread_mutex_t *mtx) { if (pthread_mutex_lock(mtx)) { error_exit("Error in pthread_mutex_lock()"); } }

void unlock(pthread_mutex_t *mtx) { if (pthread_mutex_unlock(mtx)) { error_exit("Error in pthread_mutex_unlock()"); } }

void wait(pthread_cond_t *cnd, pthread_mutex_t *mtx) { if (pthread_cond_wait(cnd, mtx)) { error_exit("Error in pthread_cond_wait()"); } }

void awake(pthread_cond_t *cnd) { if (pthread_cond_signal(cnd)) { error_exit("Error in pthread_cond_signal()"); } }

void insert_value(unsigned long long val, struct node_t **n) {
	struct node_t *new = malloc(sizeof(struct node_t));
	if (new == NULL) { error_exit("Memory allocation error"); }
	new->value = val;
	new->next = *n;
	*n = new;
}

int is_divisible(unsigned long long val, struct node_t *limit) {
	struct node_t *p = head;
	if (limit == NULL) {
		return 0;
	}
	while (p != limit) {
		if (val % p->value == 0) {
			return 1;
		}
		p = p->next;
	}
	return 0;
}

void remove_next(struct node_t *n) {
	struct node_t *tbr = n->next;
	n->next = tbr->next;
	free(tbr);
}

void t1() {
	unsigned long long value;
	struct node_t *p = head;

	for (value = 3; value > 2; ++value) {
		insert_value(value, &p->next);
		lock(&mtx_last);
		ptr_last = p->next;
		awake(&cond_last);
		unlock(&mtx_last);
		p = p->next;
	}
}

void t2() {
	struct node_t *p = head, *prev = NULL;

	for (;;) {
		lock(&mtx_last);
		if (p == ptr_last) { wait(&cond_last, &mtx_last); }
		unlock(&mtx_last);
		if (is_divisible(p->value, prev)) {
			p = p->next;
			remove_next(prev);
		} else {
			if (ptr_safe != p) {
				lock(&mtx_safe);
				ptr_safe = p;
				awake(&cond_safe);
				unlock(&mtx_safe);
			}
			prev = p;
			p = p->next;
		}
	}
}

void t3() {
	struct node_t *p = head;
	for (;;) {
		lock(&mtx_safe);
		if (p == ptr_safe) {
			wait(&cond_safe, &mtx_safe);
		}
		unlock(&mtx_safe);
		printf("%llu", p->value);
		p = p->next;
	}
}

void initialize() {
	pthread_t thread2, thread3;
	if (pthread_mutex_init(&mtx_last, NULL) || pthread_mutex_init(&mtx_safe, NULL)) {
		error_exit("Error in pthread_mutex_init()");
	}
	if (pthread_cond_init(&cond_last, NULL) || pthread_cond_init(&cond_safe, NULL)) {
		error_exit("Error in pthread_cond_init()");
	}
	if (pthread_create(&thread2, NULL, t2, NULL) || pthread_create(&thread3, NULL, t3, NULL)) {
		error_exit("Error in pthread_create()");
	}
	head = NULL;
	insert_value(2, &head);
	ptr_last = ptr_safe = head;
}

int main() {
	initialize();
	t1();
	return 0;
}
