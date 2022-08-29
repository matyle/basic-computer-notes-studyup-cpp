

typedef void (*ctor_func)(void);
ctor_func ctors_begin[1] __attribute__ ((section(".ctors")))=
{
    (ctor_func) -1 //链表初始化
};
void run_hooks()
{
    const ctor_func* list = ctors_begin;
    while((int)*list !=-1)
    {
        (**list)();/// 调用链表上的构造函数
    }
}


