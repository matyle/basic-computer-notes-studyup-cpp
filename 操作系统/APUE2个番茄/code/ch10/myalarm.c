#include<apue.h>
#include<pwd.h>

static void my_alarm(int signo){
    struct passwd * rootptr;
    printf("a handler of signal\n");
    if((rootptr=getpwnam("root"))==NULL){
        err_sys("getpwnam(root) error");
    }
    printf("%s\n",rootptr->pw_name);
    printf("a handler of signal end\n");
    alarm(1); //再次调用信号处理函数
}

int main(){
    struct passwd *ptr;
    signal(SIGALRM,my_alarm); //设置一个信号
    alarm(1);
    printf("main for\n");
    for(;;){
        if((ptr=getpwnam("matytan"))==NULL){
        err_sys("getpwnam(matytan) error");
        }
        if(strcmp(ptr->pw_name,"matytan")!=0){
            printf("return value corrupted!,pw_name=%s\n",ptr->pw_name);
        }
    }
    return 0;
}

/*
main for
a handler of signal

*/
//多次调用getpwnam导致死锁。