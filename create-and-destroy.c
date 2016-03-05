#include <signal.h>
#include <stdlib.h>
#include <stdio.h>


#define ITERATIONS 200000

#ifdef THREAD
#	include <pthread.h>
#else
#	include <unistd.h>
#	include <sys/types.h>
#	include <sys/wait.h>
#endif

void *helper_proc(void *arg) {
#ifdef THREAD
	return NULL;
#else
	exit(0);
#endif
}

int main() {
	volatile long i=0;

	while (i++ < ITERATIONS) {
#ifdef THREAD
		void *ret;
		/* result on my machine:
			real    0m1.455s
			user    0m0.304s
			sys     0m1.512s
		 */
		pthread_t thread;
		pthread_create(&thread, NULL, helper_proc, NULL);
		pthread_join(thread, &ret);
#else
		/* result on my machine:
			real    0m18.040s
			user    0m0.192s
			sys     0m6.280s
		 */
		int status;

		if (!fork())
			helper_proc(NULL);

		wait(&status);
#endif
	}

	return 0;
}
