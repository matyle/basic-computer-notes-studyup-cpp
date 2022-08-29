#include "mycrt.h"
int main(int argc,char* argv[])
{
    int i;
    FILE* fp;
    char** v = malloc(argc * sizeof(char*));

    for(i = 0;i < argc; ++i)
    {
        v[i] = malloc(strlen(argv[i])+1);
        strcpy(v[i],argv[i]);
    }
    fp = fopen("test.txt","w");
    for(int i=0;i<argc;++i)
    {
        int len = strlen(v[i]);
        fwrite(&len,1,sizeof(int),fp);
        fwrite(v[i],1,len,fp);
    }
    close(fp);

    fp = fopen("test.txt","r");

    for(i=0;i<argc;++i)
    {
        int len = strlen(v[i]);
        char* buf;
        fread(&len,1,sizeof(int),fp);//从文件读入len
        buf = malloc(len+1);
        fread(buf,1,len,fp);//char
        buf[len] = '\0';// 字符串
        printf("%d,%s \n",len,buf);
        free(buf);
        free(v[i]);
    }
    close(fp);
    free(v);

}