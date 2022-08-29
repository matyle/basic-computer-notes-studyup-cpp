#include "mycrt.h"

typedef void (*atexit_fun_t) (void);
typedef void (*cxa_fun_t) (void*); // typedef 返回值 函数指针

int __cxa_atexit(cxa_fun_t func,void* arg,void*);

int  atexit(atexit_fun_t);


//结点
typedef struct _func_node
{
    atexit_fun_t func;
    void* arg;
    int is_cxa;
    struct _func_node* next;

}func_node;


static func_node* atexit_list = 0;



int register_atexit(atexit_fun_t func,void *arg,int is_cxa)
{
    func_node* node;
    if(!func) return -1;
    node = (func_node*)malloc(sizeof(func_node));

    if(node==0) return -1;

    node->func = func;
    node->arg = arg;
    node->is_cxa = is_cxa;
    node->next = atexit_list; //头插法
    atexit_list = node;
    
    return 0;
}







int __cxa_atexit(cxa_fun_t func,void* arg,void* unused)
{
    return register_atexit((atexit_fun_t)func,arg,1);
}

int at_exit(atexit_fun_t func)
{
    return register_atexit((atexit_fun_t)func,0,0);

}


void my_crt_exit()
{
    func_node *p = atexit_list;
    for(;p!=0;p=p->next)
    {
        if(p->is_cxa)
        {
            ((cxa_fun_t)p->func)(p->arg);
        }

        else
        {
            p->func();
        }
        
    }
    free(p);
    atexit_list = 0;

}