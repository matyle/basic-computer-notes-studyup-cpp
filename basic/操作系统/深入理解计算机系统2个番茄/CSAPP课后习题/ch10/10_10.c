/* $begin cpfile */
#include "csapp.h"

int main(int argc, char **argv) 
{
    int n;
    rio_t rio;
    char buf[MAXLINE];

    if (argc == 2) {
		int fd = Open(argv[1], O_RDONLY, 0);
		dup2(fd, STDIN_FILENO);			// 应该使用重定向
		// Rio_readinitb(&rio, fd);
  // 	  } else
	}
		
	Rio_readinitb(&rio, STDIN_FILENO);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) 
		Rio_writen(STDOUT_FILENO, buf, n);
   
    exit(0);
  }
 