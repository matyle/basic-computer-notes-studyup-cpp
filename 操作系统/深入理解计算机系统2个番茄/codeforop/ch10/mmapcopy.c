#include<unistd.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/stat.h>
int main(int argc,char* argv[]){
    if(argc<2){
        printf("usage: %s and filename",argv[0]);
        return 0;
    }
    char* filename = argv[1];
    int fd = open(filename,O_RDONLY);
    struct stat stat;
    fstat(fd,&stat); //这个函数的作用
    int size = stat.st_size;
    char *bufp;
    bufp = mmap(NULL,size,PROT_READ,MAP_PRIVATE,fd,0);
    write(STDOUT_FILENO,bufp,size);
    
    return 0;
}