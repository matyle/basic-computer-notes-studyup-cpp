#include<iostream>
class A
{
public:
    //A(int i):a(i){}
    //~A(){}
    virtual void foo(){}
    void bar(){}

private:
    //int &a;
    //int i; // 4 byte
    //int *p; //8byte
    static int a;
};

int main()
{
    A a1;
    std::cout<<sizeof(a1)<<std::endl;
}