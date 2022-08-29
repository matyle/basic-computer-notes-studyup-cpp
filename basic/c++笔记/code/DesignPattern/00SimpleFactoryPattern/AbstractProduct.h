#ifndef ABSTRACTPRODUCT_H
#define ABSTRACTPRODUCT_H

#include<string>
class AbstractProduct
{
private:
    /* data */
    std::string name_;
    int price_;
public:
    AbstractProduct(/* args */){}
    virtual ~AbstractProduct(){}
    virtual void productName()=0;
    virtual void price()=0;

};
#endif