// CSAPP 家庭作业 8.20
// myls.c
//

#include "csapp.h"

int main(int argcm, char *argv[], char *envp[])
{
	execve("/bin/ls", argv, envp);

	return 0;
}