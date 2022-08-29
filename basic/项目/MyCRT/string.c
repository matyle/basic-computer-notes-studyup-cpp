#include "mycrt.h"
char* itoa(char* str,int n,int radix)

{
    //radix 转换的进制
    
    char digit[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //36

    char *p = str;
    char *head = str;

    if(!p|radix>36|radix<2) //2-36进制之间
    {
        return p;
    }
    //三种情况
    if(n<0&&radix!=10) return p; //不是10进制 但是是负数

    //n=0
    if(n==0)
    {
        *p++ = '\0';
        *p = 0;
        return p;
    }
    //10进制 负数
    if(n<0)
    {
        n = -n;
        *p++ ='-';
    }


    while(n)
    {
        *p++ = digit[n % radix];
        n /= radix;
    }

    *p = 0;
    //循环赋值给str 逆向 p最开始指向null
    for(--p;head<p;head++,p--)
    {
        char temp = *head;
        *head = *p;
        *p = temp;
    }
    return str;

}

//每个字符进行比较
int strcmp(const char* src,const char *dst)
{
    int ret = 0;
    unsigned char* p1 = (unsigned char*) src;
    unsigned char* p2 = (unsigned char*) dst; //
    while (!(ret=*p1-*p2)&&*p2) //如果有ret不等于0或者p2不为空的时
    {
        ++p1;
        ++p2;
    }
    if(ret<0) return -1;
    else if(ret>0) return 1;

    return 0;

}

char *strcpy(char* dest,const char* scr)
{
    char* ret = dest; //记录dest首地址

    while(*scr)
    {
        *dest = *scr;
        dest++;
        scr++;
    }
    *dest = '\0';//加上结束符
    return ret;
}

unsigned strlen(const char *str)
{
    int cnt = 0;
    if(!str) return 0; //str为空时
    for(;*str!='\0';++str)
    {
        ++cnt;
    }
    return cnt;
}
//test