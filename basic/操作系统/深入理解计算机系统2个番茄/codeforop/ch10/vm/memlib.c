#include "csapp.h"

//一般分配器设计

//私有全局变量
static char* mem_start_brk;//指向堆第一个字节的指针
static char* mem_brk;//最后一个字节
static char* mem_max_addr;//堆的最大虚拟地址

//初始化内存模型
void mem_init(int size){
    mem_start_brk = (char*) Malloc(size);
    mem_brk = mem_start_brk; //堆最开始是空的
    mem_max_addr = mem_start_brk + size;//堆的最大虚拟地址
}

//

void *mem_sbrk(int incr){
    char * old_brk = mem_brk;

    if((incr<0)||(mem_brk+incr)>mem_max_addr){ //分配失败
        errno = ENOMEM;
        return (void*)-1;
    }
    mem_brk += incr; //堆指针+incr
    return old_brk;// 返回旧指针 
}

