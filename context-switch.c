// a test for single-core machine. Result: the same for "-DTHREAD" and without

#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>

#define ITERATIONS 400
#define SHMEM_SIZE 4096

#include "atomic.h"

static atomic_t *shm_buf;

void *shm_malloc_try(size_t size) {
	void *ret;

	int shmid = shmget(0, size, IPC_CREAT|0777);
	struct shmid_ds shmid_ds;
	if (shmid == -1) return NULL;

	ret = shmat(shmid, NULL, 0);
	if ((long)ret == -1) return NULL;

	shmctl(shmid, IPC_STAT, &shmid_ds);
/*	shmid_ds.shm_perm.mode = 7;
	shmctl(shmid, IPC_SET,  &shmid_ds); */

	printf("shm ptr: %p\n", ret);
	return ret;
}

void *shm_malloc(size_t size) {
	void *ret;

	ret = shm_malloc_try(size);
	if (ret == NULL) {
		fprintf(stderr, "Error: shm_malloc_try(%li)\n", size);
		exit(-1);
	}

	return ret;
}


void shm_free(void *ptr) {
	shmdt(ptr);
}


static inline void proc(char a) {
	volatile long i;
	void (*atomic_act)(atomic_t *v, int incr);
	printf("Started: %i\n", a);

	atomic_act = (a == 0 ? atomic_add : atomic_sub);
	i = 0;
	while (i < ITERATIONS) {
		while (atomic_read(shm_buf) != a);
		atomic_act(shm_buf, 1);
/*		printf("%i %i\n", a, atomic_read(shm_buf));*/
		i++;
	}

	return;
}

void *thread_proc(void *arg)
{
	printf("helper: pid %i!\n", getpid());
	proc(0);

/*	sleep(1); */
	return NULL;
}

void *xfork(void *(*funct)(void *arg), void *arg) {
	int pid;
	switch((pid = fork())) {
		case -1:
			fprintf(stderr, "Error: Cannot fork().");
		case 0:
			exit(funct(arg) == 0);
		default:
			return NULL;
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	unshare(CLONE_NEWIPC);

	shm_buf = shm_malloc(SHMEM_SIZE);
	atomic_set(shm_buf, 0);
	printf("master: pid %i!\n", getpid());

#ifdef THREAD
	pthread_t thread;
	pthread_create(&thread, NULL, thread_proc, NULL);
#else
	xfork(thread_proc, NULL);
#endif
	proc(1);

	while (atomic_read(shm_buf) == 1);
	shm_free(shm_buf);

#ifdef THREAD
	pthread_exit(NULL);
#endif

/*	sleep(1); */
	return 0;
}

