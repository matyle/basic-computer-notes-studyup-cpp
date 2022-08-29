#include "../csapp.h"
#define MAXARGS 128
void eval(char* cmdline);
int parseline(const char* cmdline,char **argv);
int builtin_command(char **argv);

int main(){
    char cmdline[MAXLINE];
    while (1)
    {
        printf(">");
        Fgets(cmdline,MAXLINE,stdin);
        if(feof(stdin))
            exit(0);
        eval(cmdline);
    }
    
}

//eval实现
void eval(char *cmdline){
    //执行的参数
    char *argv[MAXARGS];
    int bg;//bg or fg?
    pid_t pid;

    bg = parseline(cmdline,argv);
    
    if(argv[0]==NULL){
        return;
    }

    if(!builtin_command(argv)){ //不是内置参数
        if((pid=fork())==0){
            //运行在子进程中
            if(execve(argv[0],argv,environ)<0){
                printf("%s:Command not found.\n",argv[0]);
            }
        }
        //父进程等待前一个任务终止
        if(!bg){
            int status;
            if(waitpid(pid,&status,0)<0){
                unix_error("waitfg: waitpid error");
            }
            else
                printf("%d,%s",pid,cmdline);
        }
        return;
    }
}
//如果第一个参数是内置的 返回true

int builtin_command(char** argv){
    if(!strcmp(argv[0],"quit")) //strcmp==0 表示相等
        exit(0);
    if(!strcmp(argv[0],"&")){ //忽略&
        return 1;//
    }
    return 0;
}

int parseline(const char *cmdline,char **argv){
    char array[MAXLINE];
    char *buf = array;
    char *delim;//指向第一个空格

    int argc;
    int bg;//后台运行？

    strcpy(buf,cmdline);
    buf[strlen(buf)-1] = ' '; //结束符用空格代替

    while(*buf&&(*buf== ' ')) //忽略前面空格  ！！注意是==
        buf++;

    argc = 0;
    while((delim=strchr(buf,' '))){
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while(*buf&&(*buf== ' ')) //忽略头一个空格
        buf++;

    }
    argv[argc] = NULL;//最后一个设置为null

    if(argc == 0)//忽略空白行
        return 1;

    if((bg = (*argv[argc-1]=='&'))!=0) //是否需要运行在后台
        argv[--argc] = NULL;
    
    return bg;
    
    

}