#include "csapp.h"

int main(void)
{
	int x = 3;

	if (Fork() != 0)
		printf("x=%d\n", ++x);

	printf("x=%d\n", --x);
	exit(0);
}