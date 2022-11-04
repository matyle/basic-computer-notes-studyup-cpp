// CSAPP 《深入理解计算机系统》 第2版
// 第12章家庭作业 12.32
// tfgets_select.c
#include "csapp.h"

char *tfgets(char * restrict str, int size, FILE * restrict stream)
{
	fd_set fgets_set;

	FD_ZERO(&fgets_set);
	FD_SET(STDIN_FILENO, &fgets_set);
	Select(STDIN_FILENO + 1, &fgets_set, NULL, NULL, NULL);

	if (FD_ISSET(STDIN_FILENO, &fgets_set))
		return fgets(str, size, stream);

	return NULL;
}