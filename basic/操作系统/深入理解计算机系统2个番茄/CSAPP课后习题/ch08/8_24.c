// CSAPP 
// 第8章家庭作业 8.24
// 8_24.c
// 一筹莫展！！！
// 

#include "csapp.h"
#define N 2

void handler1(int sig)
{
	pid_t 	pid;

	while ((pid = waitpid()))
}

int main(void)
{
	int 	status, i;
	pid_t 	pid;

	if (Signal(SIGSEGV, handler1) == SIG_ERR)			// 接收段故障的信号。
		unix_error("signal error");

	for (i = 0; i < N; i++)
		if ((pid = Fork()) == 0)
			exit(100 + i);

	while ((pid = waitpid(-1, &status, 0)) > 0) {
		if (WIFEXITED(status))
			printf("child %d terminated normally with exit status = %d\n", 
					pid, WEXITSTATUS(status));
		else
			printf("child %d terminated abnormally\n", pid);
	}

	if (errno != ECHILD)
		unix_error("waitpid error");

	exit(0);
}
