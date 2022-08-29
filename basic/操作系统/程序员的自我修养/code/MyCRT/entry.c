#include "mycrt.h"
#ifdef WIN32
//#include<Windows.h>
#endif



extern int main(int argc,char* argv[]);//调用main函数哈哈哈

void exit(int);


static void crt_fatal_error(const char* msg)
{
    //printf
    exit(1);
}

void mini_crt_entry(void)
{
    int ret = 0;
    //windows实现
    #ifdef Win32
    //GetCommandLineA();//可以获取命令行

    #else //Linux

    int argc;
    char** argv;

    char* ebp_reg = 0;
    //ebp_reg=%ebp
    asm("movl %%ebp,%0 \n": "=r" (ebp_reg));//获取%ebp的值

    argc = *(int*)(ebp_reg+4);//是加
    argv = (char**) (ebp_reg+8);//是加 char类型 指针宽度为1

    #endif

    if(!heap_init()) //初始化堆
        crt_fatal_error("heap init failed");

    if(!mini_crt_io())  //初始化io
        crt_fatal_error("io init failed");

    ret = main(argc,argv);
    exit(ret);

}
void exit(int exitCode)
{
    asm(
        "movl %0, %%ebx \n\t"
        "movl $1, %%eax \n\t"
        "int $0x80 \n\t"
        "hlt \n\t"::"m"(exitCode)
    );
}