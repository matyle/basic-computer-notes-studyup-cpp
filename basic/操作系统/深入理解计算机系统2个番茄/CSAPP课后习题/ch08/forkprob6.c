#include "csapp.h"

void doit()
{
	if (Fork() == 0) {
		Fork();
		printf("hello\n");
		return;
	}
	return;
}

int main(void)
{
	doit();
	printf("hello\n");
	exit(0);
}