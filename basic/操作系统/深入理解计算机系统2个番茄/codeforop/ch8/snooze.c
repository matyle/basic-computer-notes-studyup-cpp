#include "../csapp.h"
int secs;
void snozze(unsigned int secs){
    int ret = sleep(secs);
    printf("Slept for %d of %d secs\n",secs-ret,secs);
}
void handler(int sig){
    return;
}

int main(int argc,char* argv[]){
    if(argc<2){
        return 0;
    }
    signal(SIGINT,handler);
    secs = atoi(argv[1]);
    snozze(secs);
    exit(0);
}