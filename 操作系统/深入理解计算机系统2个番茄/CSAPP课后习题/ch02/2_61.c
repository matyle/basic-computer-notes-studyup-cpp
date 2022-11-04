// csapp 第二章章末习题 2.61

#include <stdio.h>

int main(void)
{
	int x = 0xf0000000;
	printf ("x = %.8x\n", x >> 24);

	while (scanf("%x", &x))		// while 的条件？当输入某些字符时会陷入死循环！
	{
		printf ("x = %.8x is %d\n", x, !~(x >> 24) || !(x << 24) );		// 别误写成了 按位与 和 按位取反 ！
		printf("在终端按ctrl+c 退出\n");
	}
	
	return 0;
}