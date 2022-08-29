#include "../csapp.h"

// int main(){
//     int fd1,fd2;
//     char c;
//     fd1 = Open("foo.txt",O_RDONLY,0);
//     fd2 = Open("foo.txt",O_RDONLY,0);

//     Read(fd1,&c,1);
//     Read(fd2,&c,1);

//     printf("c : %c\n",c);

//     exit(0);
// }

//11.3
// int main(){
//     int fd;
//     char c;
//     fd = Open("foo.txt",O_RDONLY,0);

//     if(Fork()==0){
//         Read(fd,&c,1);
//         exit(0);
//     }

//     Wait(NULL); //等待子进程执行
//     Read(fd,&c,1);
//     printf("c : %c\n",c); //同一个文件表，记录的同一个位置

//     exit(0);
// }

//11.5
int main(){
    int fd1,fd2;
    char c;
    fd1 = Open("foo.txt",O_RDONLY,0);
    fd2 = Open("foo.txt",O_RDONLY,0);

    
    Read(fd2,&c,1);
    Dup2(fd2,fd1);
    Read(fd1,&c,1);

    printf("c : %c\n",c);

    exit(0);
}