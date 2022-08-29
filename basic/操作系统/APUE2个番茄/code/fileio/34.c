#include"apue.h"
#include<fcntl.h>
//打印文件描述符标识
int main(int argc,char* argv[]){
    int val;
    if(argc!=2)
        err_quit("usage:a.out<descriptor#");

    if((val = fcntl(atoi(argv[1]),F_GETFL,0))<0)
        err_sys("fcntl error for fd %d",atoi(argv[1]));
    
    switch (val&O_ACCMODE)
    {
    case O_RDONLY:
        printf("read only");
        break;
    case O_WRONLY:
        printf("write only");
        break;
    case O_RDWR:
        printf("read write");
        break;
    
    default:
        err_dump("unkown access mode");
    }
    if(val&O_APPEND)
        printf("append");
    if(val&O_NONBLOCK)
        printf("nonblocking");
#if defined(O_SYNC)
    if(val&O_SYNC)
        printf(",syncchronous writes");
#endif

#if !defined(_POSIX_C_SOURCE)&&defined(O_FSYNC)
    if(val&O_SYNC)
        printf(",syncchronous writes");  

#endif
    putchar('\n');
    exit(0);
}