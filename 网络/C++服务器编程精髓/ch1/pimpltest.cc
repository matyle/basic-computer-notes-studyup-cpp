#include "pimpltest.h"

class A::PImple
{
public:
    PImple(){std::cout<<__func__<<std::endl;}
    ~PImple(){std::cout<<__func__<<std::endl;}
    void foo();

public:
    int pi;
};

void A::PImple::foo()
{
    std::cout<<__func__<<std::endl;
}

A::A():pim_(new PImple())
{
    std::cout<<__func__<<std::endl;
    pim_->pi;
}

A::~A()
{
    std::cout<<__func__<<std::endl;
}

void A::fun1()
{
    pim_->foo();
}