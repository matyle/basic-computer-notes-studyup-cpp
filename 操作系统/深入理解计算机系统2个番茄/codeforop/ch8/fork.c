#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>

void doit(){
    if(fork()==0) exit(0);
    fork();
    printf("hello\n");
    return;
}
int main(){
    pid_t pid;
    //int x = 1;
    //pid = fork();
    // if(pid==0){ //子进程中 如果不exit(0)退出 还会执行下面的代码因为是拷贝
    // printf("child: x = %d\n",++x);
    // //exit(0);
    // }
    int i;
    // printf("hello world111\n");
    // for(i =0;i<2;i++){
    //     fork();
    // }
    doit();
    printf("hello world\n");
    //printf("parent:x=%d\n",--x);
    exit(0);
}

