// CSAPP Chapter 11
// Practice Problem 11.2
// hex2dd.c
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	struct in_addr addr;
	
	if (argc != 2) {
		fprintf(stderr, "Usage: argc must be 2, but argc = %d\n", argc);
		exit(0);
	}

	addr.s_addr = htonl((unsigned int)strtol(argv[1], NULL, 16));
	printf("%s\n", inet_ntoa(addr));
	printf("%u\n", addr.s_addr);
	printf("%x\n", addr.s_addr);

	return 0;
}