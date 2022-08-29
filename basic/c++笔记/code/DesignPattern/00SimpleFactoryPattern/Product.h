#ifndef PRODUCT_H
#define PRODUCT_H
#include "AbstractProduct.h"
#include <iostream>
using namespace std;
class BasketBall:public AbstractProduct
{
private:
    /* data */
public:
    BasketBall(/* args */);
    ~BasketBall();
    void price(){cout<<"50$"<<endl;};
    void productName(){cout<<"basketball"<<endl;}
};

BasketBall::BasketBall(/* args */)
{
}

BasketBall::~BasketBall()
{
}

class Football:public AbstractProduct
{
private:
    /* data */
public:
    Football(/* args */);
    ~Football();
    void price(){cout<<"90$"<<endl;};
    void productName(){cout<<"football"<<endl;}
    
};

Football::Football(/* args */)
{
}

Football::~Football()
{
}
#endif