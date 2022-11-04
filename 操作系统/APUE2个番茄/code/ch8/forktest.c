#include<apue.h>

// #include<fcntl.h>
// char buf[] = "write to stdout\n";
// int globvar = 6;
// int main(){
    
//     pid_t pid;
//     int var;

//     var = 88;
//     int fd = open("test",O_RDWR,0);
//     //重定向  将 STDOUT_FILENO指向描述符fd指向的位置
//     //dup2(fd,STDOUT_FILENO); //将fd重定向到STDOUT_FILENO
//     // if(write(STDOUT_FILENO,buf,sizeof(buf)-1)!=sizeof(buf)-1){
//     //     err_sys("write error");
//     // }
    
//     fputs(buf,stdout);
//     printf("before fork;\n");

//     // if((pid = vfork())<0){
//     if((pid = fork())<0){
//         err_sys("fork error");
//     }else if(pid==0){ //返回了两次 pid 等于0？
//         globvar++;
//         var++;
//         printf("child pid = %ld,glob=%d,var=%d\n",(long)getpid(),globvar,var);
//     }
//     sleep(2);//父进程休眠两秒 vfork不需要
//     printf("pid = %ld,glob=%d,var=%d\n",(long)getpid(),globvar,var);
//     printf("child pid = %d,glob=%d,var=%d\n",pid,globvar,var);
    

//     exit(0);

// }

int main(){
    pid_t pid;
    pid = fork(); //子进程还要打印一次
    printf("%d\n",pid);
    // if(pid==0){
    //     pid = fork();
    //     exit(0);
    // }
    //printf("%d\n",pid);
}