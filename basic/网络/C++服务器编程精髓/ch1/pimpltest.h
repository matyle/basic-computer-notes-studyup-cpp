#ifndef PIMPLTEST_H
#define PIMPLTEST_H


#include<iostream>
#include<memory>
class A
{
public:
    A();
    ~A();
    void fun1();
private:
    class PImple; //内部类前向声明
    std::unique_ptr<PImple> pim_;
};
#endif