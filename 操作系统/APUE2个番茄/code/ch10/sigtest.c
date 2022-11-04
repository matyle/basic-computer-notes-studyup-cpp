#include<apue.h>

static void sig_quit(int signo)
{
    printf("caught SIGQUIT\n");
    if(signal(SIGQUIT,SIG_DFL)==SIG_ERR) //SIG_DFL执行默认动作
    {
        err_sys("can't reset SIGQUIT");
    }

}

static void sig_alarm(int signo)
{
    printf("caught SIGALRM\n");
    if(signal(SIGALRM,SIG_DFL)==SIG_ERR) //SIG_DFL执行默认动作
    {
        err_sys("can't reset SIGQUIT");
    }

}

int main()
{
    sigset_t newmask,oldmask,pendmask;
    if(signal(SIGQUIT,sig_quit)==SIG_ERR)
    {
        err_sys("can't catch SIGQUIT");

    }

    if(signal(SIGALRM,sig_alarm)==SIG_ERR)
    {
        err_sys("can't catch SIGALRM");

    }
    sigemptyset(&newmask);

    sigaddset(&newmask,SIGQUIT); //信号集中的信号

    sigaddset(&newmask,SIGALRM); //信号集中的信号


    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0)
        err_sys("SIG_BLOCK error");
    
    sleep(10);

    alarm(5);

    if(sigpending(&pendmask)<0)
        err_sys("pending error");

    if(sigismember(&pendmask,SIGQUIT))
        printf("\nSIGQUIT pending\n");
    
    //恢复成没有阻塞的信号屏蔽字

    if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
        err_sys("SIG_SETMASK error");

    printf("SIGQUIT unblocked\n");

    sleep(10);

    alarm(5);

    exit(0);
}

void  pr_mask(const char *str)
{
    sigset_t sigset;
    int errno_save;

    errno_save = errno;
    if(sigprocmask(0,NULL,&sigset)<0){
        err_ret("sigprocmask error");
    }else{
        printf("%s",str);
        if(sigismember(&sigset,SIGINT))
            printf("SIGINT\n");

        if(sigismember(&sigset,SIGQUIT))
            printf("SIGQUIT\n");
        
        if(sigismember(&sigset,SIGUSR1))
            printf("SIGUSR1\n");

        if(sigismember(&sigset,SIGALRM))
            printf("SIGALRM\n");

    }
    errno = errno_save;
}