#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char* argv[],char* envp[]){
    if(argc<=2) return 0;
    char* s1 = "PWD";
    //setenv(s1,s1,0);
    // printf("argv[0]:%s\n",argv[0]);
    // printf("argv[1]:%s\n",argv[1]);
    // printf("argv[2]:%s\n",argv[2]);
    int cnt = 0;
    while (argv[cnt]!=NULL)
    {
        printf("argv[%2d]:%s\n",cnt,argv[cnt]);
    }
    
    cnt = 0;
    //printf("envp[%d]:%s\n",cnt,getenv(s1));

    while (envp[cnt]!=NULL)   
    {
        printf("envp[%2d]:%s\n",cnt,envp[cnt]);
        cnt++;   
        
    }
    return 0;
    
}