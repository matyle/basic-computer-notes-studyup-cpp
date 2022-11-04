// csapp 第2章 2.58

#include <stdio.h>

int is_little_endian (void)
{
	int x = 1;
	unsigned char * byte_pointer = (unsigned char *) &x;
	printf("%.2x\n", x);
	if (*byte_pointer == 1)
		return 1;
	else 
		return 0;

}

int main (void)
{
	if (is_little_endian())
		printf("your computer is little endian\n");
	else
		printf("your computer is big endian\n");

	return 0;
}