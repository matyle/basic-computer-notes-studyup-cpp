#include "../csapp.h"
int main(int argc,char **argv){
    int fd;
    int n;
    rio_t rio;
    char buf[MAXLINE];
    if(argc==2){
        fd = Open(argv[1],O_RDONLY,0);
        dup2(fd,STDIN_FILENO);//标准输入
        close(fd);
    }
    Rio_readinitb(&rio,STDIN_FILENO);//初始化
    while((n=Rio_readlineb(&rio,buf,MAXLINE))!=0){
        Rio_writen(STDOUT_FILENO,buf,n);
    }
    return 0;
}