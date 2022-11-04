#include <functional>
#include <algorithm>
#include <vector>
#include "funcbindtest.h"
#include <iostream>
using namespace std;
void Foo::methodA(){
    cout<<"method A"<<endl;
}
void Foo::methodInt(int a,int b){
    cout<<"methodInt a:"<< a <<endl;
    cout<<"methodInt b:"<< b <<endl;

}

void Foo::methodString(const string &str){

    cout<<"methodStr:"<< str <<endl;
}

void Bar::methodB(){
    cout<<"method B"<<endl;
}

void methodC(int c){
    cout<<"method C"<<endl;
}
int main()
{
    typedef function<void()> Functor; //定义函数对象名
    typedef function<void(int,int)> CallbackInt; //定义函数对象名
    using std::placeholders::_1;
    using std::placeholders::_2;

    Functor f1;                       // 无参数无返回值
    CallbackInt f2;//
    Functor f3;

    int i=10;
    int j=42;


    Foo foo;
    f1 = bind(&Foo::methodA, &foo);
    f1();//调用f1

    //f2 = bind(&Foo::methodInt,&foo,42);
    f2 = bind(&Foo::methodInt,&foo,_2,_1); //利用占位符 占位符按第2个参数放_1
    f2(i,j); //没参数，因为没有占位符 

    f3 = bind(methodC,40);

    f3();

}
