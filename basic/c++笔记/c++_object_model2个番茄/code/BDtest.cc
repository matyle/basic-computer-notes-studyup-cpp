//测试父子进程

#include <iostream>

class Base
{
public:
    Base()
    {
        std::cout<<"Base()"<<std::endl;
    }

    ~Base()
    {
        std::cout<<"~Base()"<<std::endl;
    }

    virtual void foo()
    {
        std::cout<<"fooBase()"<<std::endl;
        
    }
};



//为什么private不能转换？
class Derived:public Base
{
public:
    Derived()
    {
        std::cout<<"Derived()"<<std::endl;
    }

    ~Derived()
    {
        std::cout<<"~Derived()"<<std::endl;
    }

    void foo()
    {
        std::cout<<"fooDe()"<<std::endl;
        
    }


};

int main()
{
    //Derived *d = ;
    Base *b =  new Derived();
    b->foo();
    delete b; 
    //由于~Base不是virtual delete b时,b实际是被截断了的，能访问子类的foo是因为虚表
    //因此它访问不了子类的析构导致 子类无法析构


}