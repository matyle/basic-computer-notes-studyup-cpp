// CSAPP 第8章 异常控制流
// 家庭作业 8.22
// 未完成！！！
// 
// 

#include "csapp.h"

int mysystem(char *command)
{
	pid_t 		pid;
	char 		*argv[3];					// 只需 3 个参数
	int 		status;
	argv[0] = "/bin/sh";
	argv[1] = "-c";
	argv[2] = command;

	if ((pid = Fork()) == 0) {							// Child
		if (execve("/bin/sh", argv, envp) < 0) {
			printf("%s: Command not found.\n", "/bin/sh");
		exit(0);
	}

	if ((pid = wait(&status)) > 0) {		// 正常终止
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status)) {		// 异常终止
			return WTERMSIG(status);
		}
	}
}

int main(int argc, char *argv[])
{
	return mysystem(argv[1]);
}