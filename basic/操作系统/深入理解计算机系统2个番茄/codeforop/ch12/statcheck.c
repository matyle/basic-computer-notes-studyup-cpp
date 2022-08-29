#include "../csapp.h"
int main(int argc,char **argv){
    struct stat stat;
    char *type,*readok;
    int fd = atoi(argv[1]);
    char *filename;
    // fcntl(fd,F_GETPATH,filename);
    // printf("%s\n",filename);
    Fstat(fd,&stat);

    if(S_ISREG(stat.st_mode))//文件类型
        type = "regular";
    
    if(S_ISDIR(stat.st_mode))
        type = "directory";

    else
        type = "other";

    if((stat.st_mode&S_IRUSR))//读访问权限
        readok = "yes";
    else
        readok = "no";
        
    printf("type:%s,read:%s\n",type,readok);
    exit(0);
}