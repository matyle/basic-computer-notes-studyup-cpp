// CSAPP Chapter 11
// Practice Problem 11.3
// dd2hex.c

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	struct in_addr addr;

	if (argc == 2) {
		if (inet_aton(argv[1], &addr)) {
			printf("0x%x\n", ntohl(addr.s_addr));	// 转换成主机字节顺序
			printf("%u\n", addr.s_addr);			// 测试 
			printf("%s\n", inet_ntoa(addr));		// 测试
		} else {
			fprintf(stderr, "inet_aton error");
			exit(1);
		}
	} else {
		fprintf(stderr, "Usage: argc must be 2, but argc = %d\n", argc);
		exit(0);
	}

	return 0;
}	