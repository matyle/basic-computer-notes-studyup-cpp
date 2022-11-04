// CSAPP 《深入理解计算机系统》 第2版
// 第12章家庭作业 12.31
// tfgets_proc.c

#include "csapp.h"

jmp_buf buf;

void handler(int sig)
{
	longjmp(buf, 1);
}

char *tfgets(char * restrict str, int size, FILE * restrict stream)
{
	int rc;
	Signal(SIGCHLD, handler);

	rc = setjmp(buf);
	if (fork() == 0) {
		Sleep(5);
		exit(0);
	}

	if (rc == 1)
		return NULL;
	else
		return fgets(str, size, stream);
}

