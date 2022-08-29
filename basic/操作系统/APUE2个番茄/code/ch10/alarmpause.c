#include<apue.h>
#include<setjmp.h>
unsigned int sleep2(int seconds);
static jmp_buf env_alarm;
static void sig_handle(int signo){
    printf("a handler of signal\n");
    longjmp(env_alarm,1);
}

int main(){
    
    sleep2(5);
    return 0;
}

unsigned int sleep1(int seconds){
    if(signal(SIGALRM,sig_handle)==SIG_ERR)
        return seconds;
    alarm(seconds); //竞争条件
    pause();
    return(alarm(0));
}



unsigned int sleep2(int seconds){
    if(signal(SIGALRM,sig_handle)==SIG_ERR)
        return seconds;
    if(setjmp(env_alarm)==0){ //成功返回0！！！
        alarm(seconds); //竞争条件
        pause();
    }

    return(alarm(0));
}
