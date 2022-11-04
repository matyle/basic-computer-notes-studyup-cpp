#include "../csapp.h"

void handler(int sig){
    pid_t pid;
    while((pid=waitpid(-1,NULL,0))>0)
        deletejob(pid);
    if(errno!=ECHILD)
        unix_error("waitpid error");
}
int main(int argc,char* argv[]){
    int pid;
    sigset_t mask;
    Signal(SIGCHLD,handler);
    initjobs();//初始化工作列表

    while (1)
    {
        Sigemptyset(&mask);
        Sigaddset(&mask,SIGCHLD);
        Sigprocmask(SIG_BLOCK,&mask,NULL);//调用fork之前 阻塞SIGCHILD信号

        //子进程
        if((pid=Fork())==0){
            Sigprocmask(SIG_UNBLOCK,&mask,NULL);//子进程中继承了父进程的阻塞set，所以需要解除子进程阻塞的信号
            Execve("/bin/ls",argv,NULL);

        }
        //父进程
        addjob(pid);
        Sigprocmask(SIG_UNBLOCK,&mask,NULL);
    }
    eixt(0);
    
}