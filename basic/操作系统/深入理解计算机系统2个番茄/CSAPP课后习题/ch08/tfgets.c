// CSAPP 《深入理解计算机系统》 第2版
// 第8章家庭作业 8.25
// tfgets.c

#include "csapp.h"

jmp_buf buf;

void handler(int sig)
{
	longjmp(buf, 1);
}

char *tfgets(char * restrict str, int size, FILE * restrict stream)
{
	int rc;
	Signal(SIGALRM, handler);
	Alarm(5);
	rc = setjmp(buf);
	if (rc == 1)
		return NULL;
	else
		return fgets(str, size, stream);

}

int main(void)
{
	const int 	SIZE = 512;
	char 		buf[SIZE];
	char 		* p;

	p = tfgets(buf, SIZE, stdin);
	if (p != NULL)
		printf("%s", p);
	else 
		printf("超时退出！\n");
	
	return 0;
}