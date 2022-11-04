// csapp 2.64
// 2016/04/10

#include <stdio.h>
#include <stdbool.h>
// return 1 when any even bit of x equals 1;0 otherwise
int any_even_one (unsigned x)
{
	return (bool)(0x55555555 & x);
}


int main(void)
{
	unsigned x = 0x55555555;
	unsigned y = 0xaaaaaaaa;

	printf("x is %d\n", any_even_one(x));
	printf("y is %d\n", any_even_one(y));

	return 0;
}