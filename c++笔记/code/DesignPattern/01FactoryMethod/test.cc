#include "FactoryProduct.h"
int main()
{
    //工厂方法模式， 即产品和工厂都提供抽象接口
    AbsFactoryPtr factory(new BasketballFac());
    AbsProductPtr product;

    product = move(factory->getProduct());
    product->productName();
    //忘了delete 当然是main可能无所谓。为了好一点使用智能指针

    //工厂2
    factory.reset(new FootballFac());// 注意要用reset重设
    product = move(factory->getProduct());
    product->productName();


    //如果我要添加一个产品呢？
    //只需要实例扩展就行了 不会影响抽象(类)接口。
    //也不用修改任何之前的类 只需要添加！
}