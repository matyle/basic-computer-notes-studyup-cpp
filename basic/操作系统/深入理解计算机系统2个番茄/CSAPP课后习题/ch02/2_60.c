//csapp 第2章练习题 2.60

#include <stdio.h>

unsigned repleace_byte (unsigned x, unsigned char b, int i)
{
	return ((x & ~(0xff << (i * 8))) | (unsigned int )b << (i * 8));
}

int main(void)
{
	unsigned x = 0x12345678;
	unsigned char b= 0xab;
	int i = 0;		// 字节号

	printf ("x = 0x%x  ", x);
	printf("b = 0x%.2x  ", b);
	//printf("b = 0x\t", b);	// 怎么打印出 b 的十六进制值？	
	printf("i = %d\n", i);
	// printf("输入一个 unsigend int 值:\n");
	// scanf ("%ux", &x);
	// printf("输入一个 unsigend char 值:\n");
	// scanf ("%x", &b);
	// printf("输入一个要替换的字节号:\n");
	// scanf ("%d", &i);

	unsigned int y = repleace_byte (x, b, i);
	//unsigned int y = repleace_byte (0x12345678, 0xab, 2);
	// 我原本想直接对函数返回值取址， 结果出现下面的报错，也许和 左值右值 这些知识有关
	//2_60.c:14:40: error: cannot take the address of an rvalue of type 'unsigned int'
	// ...char * z = (unsigned char *) &repleace_byte (0x12345678, 0xab, 2);
	//                                ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//1 error generated.

	unsigned char * z = (unsigned char *) &y;
	for (int i = 0; i < sizeof(unsigned); i++)
		printf (" %.2x", z[i]);		// 显示替代后的字节
	printf ("\n");
	
	return 0;
} 