#include<cassert>
#include<iostream>
#include<cstring>
using namespace std;

enum FeatureSupport{
    C99 = 0x0001,
    ExtInt = 0x0002,
    SAssert = 0x0004,
    Noexcept = 0x0008,
    SMAX = 0x0010,

};

// SMAX = 0x0010,利用SMAX检查宏是否有重复

struct Compiler{
    const char* name;
    int spp;
};


template<typename T,typename U> int bit_copy(T& a, U& b){
    //编译时期报错
    static_assert(sizeof(a)==sizeof(b),"should have same width"); //判断相同
    memcpy(&b,&a,sizeof(b));
    return b;
}

int main()
{
    //动态assert
    assert((SMAX-1)==(C99|ExtInt|SAssert|Noexcept));//检查是否有重复或者错误 如果有 一定不等于SMAX-1
    Compiler a = {"abc",(C99|SAssert)};

    if(a.spp & C99) //存在
    {
        cout<<"C99"<<endl;
    }

    int aa = 0x2468; 
    //int b;
    double b;//Assertion failed: (sizeof(a)==sizeof(b)), function bit_copy, file 2-5-2assert.cc, line 24.
    //cout<<hex<<bit_copy<int,int>(aa,b)<<endl;;
    cout<<bit_copy(aa,b)<<endl;;

}


