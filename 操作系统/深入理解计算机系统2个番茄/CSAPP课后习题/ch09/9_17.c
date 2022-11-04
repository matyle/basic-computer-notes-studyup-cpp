// CSAPP 家庭作业9.17
// 
//

static void *find_fit(size_t asize)
{
	// 下一次适配
	void 		*bp;
	// static 的变量第一次初始化后似乎再调用此函数不必初始化，我记的不大真切。
	static void *last_bp = heap_listp;	// heap_listp 指向何处？我仍有疑问。

	// 怎么得到上一次的bp值？
	for (bp = last_bp; GET_SIZE(HDRP(bp)) != 0; bp = NEXT_BLKP(bp))
		if (!GET_ALLOC(HDRP(bp)) && （GET_SIZE(HDRP(bp)) > = asize)）{
			last_bp = bp;
			return bp;
		}
	last_bp = heap_listp; 				// 此时没有在剩余的块中找到合适的块

	return NULL;
}


/* 测试静态局部变量的性质
#include <stdio.h>

void test(void)
{
	static int a = 10;
	printf("%d\n", a);

	a = 100;
}

int main(void)
{
	test();
	test();
	test();

	return 0;
}*/