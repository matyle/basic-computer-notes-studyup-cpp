#include "test.h"
extern template void fun<int>(int); //不会实例化
void test2()
{
    fun(4);//类型相同
}