#include <assert.h>
#include <iostream>
int x = 0;
int &r = x;

void fun(int &i)
{
    int *p3 = &i;
    std::cout<<"p3:"<<p3<<std::endl;
}
int main()
{

    int *p = &x;
    int *p2 = &r;
    //r是啥？是引用吗？
    // assert(p == p2); // &x == &r
    // assert(&p != &p2);
    std::cout<<"p:"<<p<<std::endl;
    std::cout<<"p2:"<<p2<<std::endl;

    if(p==p2)
    {
        std::cout<<"p==p2"<<std::endl;
    }
    if(&p==&p2)
    {
        std::cout<<"&p==&p2"<<std::endl;
    }

    fun(r);
}