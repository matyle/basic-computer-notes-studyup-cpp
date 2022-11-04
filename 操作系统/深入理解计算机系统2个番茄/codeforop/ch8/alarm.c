#include "csapp.h"

void handler(int sig){
    static int beeps = 0;
    printf("BEEP\n");
    if(++beeps<5)
        Alarm(1); //调用alarm
    else{
        printf("BOOM\n");
        exit(0);
    }
}

int main(){
    Signal(SIGALRM,handler);//信号处理
    Alarm(1);
    while (1)
    {
        ; //信号处理函数每次在循环中返回
    }
    exit(0);
    
}
