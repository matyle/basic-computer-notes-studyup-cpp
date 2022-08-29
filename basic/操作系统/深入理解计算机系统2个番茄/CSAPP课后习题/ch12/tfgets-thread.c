// CSAPP 《深入理解计算机系统》 第2版
// 第12章家庭作业 12.33
// tfgets-thread.c
#include "csapp.h"
void *thread1(void *vargp);
void *thread2(void *vargp);

char *tfgets(char * restrict str, int size, FILE * restrict stream)
{
	pthread_t tid1, tid2;
	//char *returnp = NULL;
	struct var {
		char * str;
		int size;
		FILE * stream;
	} *p;

	strcpy(p->str, str);
	p->size = size;
	p->stream = stream;

	Pthread_create(&tid1, NULL, thread1, (void *)p);
	Pthread_create(&tid2, NULL, thread2, &tid1);
	
	return p;
}

void *thread1(void *vargp)
{
	vargp = fgets(vargp->str, vargp->size, vargp->stream);
}

void *thread2(void *vargp)
{
	Sleep(5);
	pthread_cancel(*vargp);
}