// CSAPP（深入理解计算机系统） 12章 家庭作业
// 12.16
// 12_16.c
#include "csapp.h"
void *thread(void *vargp);

int main(int argc, char *argv[0])
{
	if (argc != 2) {
		printf("Usage: %s <nthreads>\n", argv[0]);
		exit(0);
	}

	pthread_t tid;
	int n = atoi(argv[1]);

	for (int i = 0; i < n; i++) {
		Pthread_create(&tid, NULL, thread, NULL);
	}
	
	for (int i = 0; i < n; i++)
		Pthread_join(tid, NULL);

	exit(0);	
}

void *thread(void *vargp)
{
	printf("Hello world!\n");
	return NULL;
}