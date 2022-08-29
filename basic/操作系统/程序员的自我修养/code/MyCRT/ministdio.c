#include "mycrt.h"
int mini_crt_io()
{
    return 1;
}
// #ifdef WIN32
// #iincude<Windows.h>
// FILE* fopen(const char *filename,const char* mode)
// {
    
// }
//#else

//Linux中的实现


static int open(const char* pathname,int flags,int mode)
{
    int fd = 0;//
    asm("movl $5,%%eax \n\t" // 系统调用号
        "movl %1,%%ebx \n\t" // 占位符传参数
        "movl %2,%%ecx \n\t" //
        "movl %3,%%edx \n\t"
        "int $0x80 \n\t" //中断号0x80 系统调用
        "movl %%eax,%0 \n\t": //作为系统调用参数 系统调用号5  asm( : :)
        "=m"(fd): "m"(pathname),"m"(flags),"m"(mode)); //:之前的是输出
    return fd;
}

static int read(int fd,void* buffer, unsigned size)
{
    int ret = 0;
    asm("movl $3,%%eax \n\t" // 系统调用号3 代表read
    "movl %1,%%ebx \n\t" // 占位符传参数
    "movl %2,%%ecx \n\t" //
    "movl %3,%%edx \n\t"
    "int $0x80 \n\t" //中断号0x80 系统调用
    "movl %%eax,%0 \n\t": //作为系统调用参数 系统调用号3  asm( : :)
    "=m"(ret): "m"(fd),"m"(buffer),"m"(size)); //:之前的是输出

    return ret;
}


static int write(int fd,void* buffer, unsigned size)
{
    int ret = 0;
    asm("movl $4,%%eax \n\t" // 系统调用号4 代表write 从buffer中读到fd
    "movl %1,%%ebx \n\t" // 占位符传参数  1
    "movl %2,%%ecx \n\t" //
    "movl %3,%%edx \n\t"
    "int $0x80 \n\t" //中断号0x80 系统调用
    "movl %%eax,%0 \n\t": //作为系统调用参数 系统调用号19  asm( : :)
    "=m"(ret): "m"(fd),"m"(buffer),"m"(size)); //:之前的是输出

    return ret;
}

static int seek(int fd,int offset, int mode)
{
    int ret = 0;
    asm("movl $19,%%eax \n\t" // 系统调用号19 代表seek 
    "movl %1,%%ebx \n\t" // 占位符传参数
    "movl %2,%%ecx \n\t" //
    "movl %3,%%edx \n\t"
    "int $0x80 \n\t" //中断号0x80 系统调用
    "movl %%eax,%0 \n\t": //作为系统调用参数 系统调用号19  asm( : :)
    "=m"(ret): "m"(fd),"m"(offset),"m"(offset)); //:之前的是输出

    return ret;
}

static int close(int fd)
{
    int ret = 0;
    asm("movl $6,%%eax \n\t" // 系统调用号19 代表seek 
    "movl %1,%%ebx \n\t" // 占位符传参数
    "int $0x80 \n\t" //中断号0x80 系统调用
    "movl %%eax,%0 \n\t": //作为系统调用参数 系统调用号6  asm( : :)
    "=m"(ret): "m"(fd)); //:之前的是输出

    return ret;
}


FILE* fopen(const char* filename,const char *mode)
{
    int fd = -1;
    int flags = 0;
    int access = 00700;//创建文件权限
    


    if(strcmp(mode,"w")==0)
        flags |= O_WRONLY|O_CREAT|O_TRUNC;
    
    if(strcmp(mode,"w+")==0)
    flags |= O_RDWR|O_CREAT|O_TRUNC;

    if(strcmp(mode,"r")==0)
    flags |= O_RDONLY|O_CREAT;

    if(strcmp(mode,"r+")==0)
    flags |= O_RDWR|O_CREAT;


    fd = open(filename,flags,access);
    return (FILE*)fd;
}



int fread(void* buffer,int size,int count,FILE* stream)
{
    return read((int)stream,buffer,size*count);
}
//从buf中写出
int fwrite(const void* buffer,int size,int count,FILE* stream)
{
    return write((int)stream,buffer,size*count);
}

int fclose(FILE* fp)
{
    return close((int)fp);
}

int fseek(FILE* fp,int offset,int set)
{
    return seek((int)fp,offset,set);
}




//实现printf格式化输出
int fputc(int c,FILE* stream)
{
    if(fwrite(&c,1,1,stream)!=1)
        return EOF; //EOF符
    else
        return c;
}

int fputs(const char *str,FILE* stream)
{
    //将str写入 stream
    int len = strlen(str);//循环计算长度
    if(fwrite(str,1,len,stream)!=len) //str不能取地址符
        return EOF; //EOF符
    else
        return len;
}




//定义va_list 变量列表
#define va_list char*
#define va_start(ap,arg) (ap=(va_list)&arg+sizeof(arg))//从arg后面开始取参数地址
#define va_arg(ap,t) (*(t*)((ap+=sizeof(t))-sizeof(t))) //将ap转换为t类型
#define va_end(ap) (ap(va_list)0)


int vfprintf(FILE *stream,const char* format,va_list arglist)
{
    int translating = 0;
    int ret = 0;
    const char* p = 0;


    for(p=format;*p!='\0';++p)
    {
        switch(*p)
        {
            case '%':
                if(!translating)
                    translating = 1;

                else
                {
                    if(fputc('%',stream)<0)
                        return EOF;
                    ++ret;
                    translating = 0;
                }
                break;
            
            case 'd':
                if(translating)
                {
                    char buf[16];
                    for(int i=0;i<16;i++) buf[i] = '\0';
                    translating = 0;
                    itoa(buf,va_arg(arglist,int),10);//变长参数 int变字符串
                    if(fputs(buf,stream)<0) return EOF;
                    ret += strlen(buf);

                }
                else if(fputc('d',stream)<0) return EOF;

                else ++ret;
                break;
            
            case 's':
                if(translating)
                {
                    const char* str = va_arg(arglist,const char*);
                    translating = 0;
                    if(fputs(str,stream)<0) return EOF;
                    ret += strlen(str);
                }

                else if(fputc('s',stream)<0) return EOF;

                else ++ret;
                break;
            
            default:
                if(translating) translating=0;
                if(fputc(*p,stream)<0) return EOF;
                else ++ret;
                break;

        }
    }
    return ret;
}

int printf(const char* format,...)
{
    va_list(arglist);
    va_start(arglist,format);//从format后面开始取参数
    return vfprintf(stdout,format,arglist);
}

int fprintf(FILE* stream,const char* format,...)
{
    va_list(arglist);
    va_start(arglist,format);//从format后面开始取参数
    return vfprintf(stream,format,arglist);
}

