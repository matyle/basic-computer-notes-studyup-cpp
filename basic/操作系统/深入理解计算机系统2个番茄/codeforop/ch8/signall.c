#include "../csapp.h"

void handler1(int sig){
    pid_t pid ;
    // if((pid = waitpid(-1,NULL,0))<0){
    //     unix_error("waitpid error");
    // }
    while ((pid = waitpid(-1,NULL,0)) > 0)
    {
        printf("handler reaped child %d\n",pid);
    }
    // 改进
    if(errno!=ECHILD)
        unix_error("waitpid error");
    sleep(2);
    return;
}

int main(){
    int i,n;
    char buf[MAXBUF];
    if(signal(SIGCHLD,handler1)==SIG_ERR)
        unix_error("signal error");
    
    //创建子进程
    for(i=0;i<3;i++){
        if(Fork()==0){
            //子进程
            printf("hello from child %d\n",(int)getpid());
            Sleep(1);
            exit(0);
        }
    }

    //父进程等待输入然后处理
    if((n=read(STDIN_FILENO,buf,sizeof(buf)))<0){
        unix_error("read error");
    }
    printf("parent processing input\n");
    while (1)
    {
        ;
    }
    exit(0);
}