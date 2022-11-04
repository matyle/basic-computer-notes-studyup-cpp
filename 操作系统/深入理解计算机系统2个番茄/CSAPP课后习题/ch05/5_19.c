// 深入理解计算机系统
// 家庭作业 5.19
// basic_memset的更有效的版本
#include <stddef.h>		// 定义size_t的头文件
#include <stdio.h>

void *effective_memset(void *s, int c, size_t n)
{
	size_t cnt;
	const unsigned int K = sizeof(unsigned long);			// x86-64: 8 * char; IA32: 4 * char
	unsigned long fill_word = c;					// 填充字节，fill_word为4/8个c
	int count = 0;
	unsigned char *schar = s;
	unsigned long *slong;

	// 使 fill_word 填充4／8个c
	while (count++ < K)
	{
		fill_word = fill_word | (fill_word << 8);
	}

	while ((unsigned)schar % K != 0)					// 获取schar的地址值？
	{
		*schar++ = (unsigned char) c;
	}
	slong = (unsigned long *) schar;
	for (unsigned i = 0; i < n/K; i++)		// 以字为单位填充
	{
		*slong++ = fill_word; 
	}
	schar = (unsigned char *) slong;
	count = (unsigned)schar - (unsigned)s;			// 当前地址与初始地址之差
	while (count++ < n)			// 填充剩余的字节
	{
		*schar++ = (unsigned char) c;	
	}

	return s;
}

// 测试程序
int main(void)
{
	char buff[100];
	char c = '$';

	for (int i = 0; i < 100; i++)
		printf("%c", buff[i]);
	printf("__END__");
	printf("\n--------------------------------------------------\n");
	effective_memset(buff, c, 100);
	for (int i = 0; i < 100; i++)
		printf("%c", buff[i]);
	printf("\n--------------------------------------------------\n");
	return 0;
}









