// CSAPP 《深入理解计算机系统》 第2版
// 家庭作业 12.31 12.32 12.33 驱动程序
// tfgets-main.c
#include "csapp.h"

char *tfgets(char * restrict str, int size, FILE * restrict stream);

int main(void)
{
	char buf[MAXLINE];

	if (tfgets(buf, MAXLINE, stdin) == NULL)
		printf("BOOM!\n");
	else
		printf("%s", buf);

	exit(0);
}