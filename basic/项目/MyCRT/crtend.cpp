typedef void (*ctor_func)(void);

ctor_func ctors_end[1] __attribute__ ((section(".ctors")))=
{
    (ctor_func) -1 //链表初始化
};