#include "test.h"
template void fun<int>(int);
void test2();
void test1()
{
    fun(3);
}

int main()
{
    test1();
    test2();

}