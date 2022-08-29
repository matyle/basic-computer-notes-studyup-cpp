#include "csapp.h"

void foo(int n)
{
	int i;

	for (i = 0; i < n; i++) 
		Fork();
	printf("hello\n");
	
	exit(0);
}

int main(void)
{
	int n = 4;
	foo(n);

	return 0;
} 