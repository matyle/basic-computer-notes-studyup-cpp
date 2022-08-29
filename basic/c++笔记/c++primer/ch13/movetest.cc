#include<iostream>
//#include<utility>
#include<string>
class Foo
{
public:
    Foo(){}
    Foo(std::string i):i_(new std::string(i))
    {
        std::cout<<"constructor function"<<std::endl;
    }
    ~Foo()
    {
        delete i_; //释放内存 析构需要释放内存
        std::cout<<"deconstructor function"<<std::endl;

    }

    // void getAddr()
    // {
    //     std::cout<<"addr:"<<this<<std::endl;
    // }



    Foo(Foo &&that) noexcept
    {
        i_ = that.i_;
        that.i_ = nullptr;
        std::cout<<"move constructor function"<<std::endl;

    }

    Foo &operator=(Foo &&rhs) noexcept
    {   if(&rhs!=this)
        {
            i_ = rhs.i_;
            rhs.i_ = nullptr; //必须的，不然会析构掉
        }
        
        std::cout<<" operator= function"<<std::endl;
        return *this;
    }

    void print()
    {
        std::cout<<"string:"<<*i_<<std::endl; //地址上内容还没有被覆盖
    }
private:
    std::string *i_;
};

int main()
{
    Foo foo1;
    {
        Foo foo("helloworld");
        foo.print();
        foo1 = std::move(foo);
        foo.print();
    }

    foo1.print();
    //foo1.getAddr();
    //std::cout<<"foo1 addr:"<<&foo1<<std::endl;


}
// int main(){
//     const std::string &clr = "hello const lr";
//     const std::string & clr2 = clr;
//     //clr = "ffdafddfa";
//     std::string &&rr1 = "helloworld";
//     std::string &&rr2 = std::move(rr1);
//     rr1 = nullptr;
//     //std::string &&rr3 = std::move(rr1);

//     //rr1 = "nihoa";
//     std::cout<<&rr2<<": "<<rr2<<std::endl;
//     //std::cout<<&rr1<<": "<<rr1<<std::endl;
//     //std::cout<<&rr3<<": "<<rr3<<std::endl;

//     return 0;
// }