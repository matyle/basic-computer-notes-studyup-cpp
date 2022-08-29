// myecho.c
// CSAPP 练习题8.6

#include <stdio.h>

int main(int argc, char *argv[], char * envp[])
{
    if (argc >= 3)              // 3个命令行参数以上
    {
        int i;
        printf("Command line arguments:\n");
        for (i = 0; argv[i] != NULL; i++)
        {
            printf("argv[%2d]: %s\n", i, argv[i]);
        }
        printf("\nEnvironment variables:\n");
        for (i = 0; envp[i] != NULL; i++)
        {
            printf("envp[%2d]: %s\n", i, envp[i]);
        }
    } else                      // 命令行参数不足3个
        printf("No enough command line arguments.\n");

     return 0;   
}