// CSAPP 家庭作业9.14
//
//

#include "csapp.h"
// #include <stdio.h>
// #include <unistd.h>
// #include <sys/mman.h>
// #include <stdlib.h>

int main(void)
{
	void *bufp;
	const int size = 24;
	int fd = open("hello.txt", O_RDWR);
	// FILE *fp;
	// fp = fopen("hello.txt", "r+");

	bufp = mmap(NULL, size, PROT_WRITE, MAP_SHARED, fd, 0);	// 此时无出错的处理
	((char *)bufp)[0] = 'J';			// 强制类型转换
	if (munmap(bufp, size) == -1) {		// 回收出错
		printf("munmap error!\n");
		exit(0);
	};

	return 0;
}