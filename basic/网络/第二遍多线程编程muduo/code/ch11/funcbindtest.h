#ifndef FUNCBINDTEST_H
#define FUNCBINDTEST_H

#include<string>
using namespace std;
class Foo
{
public:
    void methodA();
    void methodInt(int a,int b);
    void methodString(const string &str);
};

class Bar
{
public:
    void methodB();
};

#endif