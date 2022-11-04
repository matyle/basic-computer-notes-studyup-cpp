#include<iostream>

class Foo
{
public:
    Foo(int i):i_(i)
    {
        std::cout<<"constructor function"<<std::endl;
    }
    ~Foo()
    {
        std::cout<<"deconstructor function"<<std::endl;

    }

    Foo(const Foo &that)
    {
        i_ = that.i_;
        std::cout<<"copy constructor function"<<std::endl;

    }

    Foo &operator=(const Foo &rhs)
    {
        i_ = rhs.i_;
        std::cout<<" operator= function"<<std::endl;
        return *this;
    }

private:
    int i_;

};

int main()
{
    Foo foo(2);
    Foo foocopy(foo);
    return 0;
}