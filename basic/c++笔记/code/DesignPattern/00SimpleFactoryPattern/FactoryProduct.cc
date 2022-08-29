#include "AbstractProduct.h"
#include "Product.h"
#include <iostream>
#include <memory>
using namespace std;
typedef unique_ptr<AbstractProduct> AbsProductPtr;

class FactoryProduct
{
private:
    /* data */
public:
    
    FactoryProduct(/* args */);
    ~FactoryProduct();
    //使用智能指针
    AbsProductPtr getProduct(const std::string& productName)
    {
        if(productName=="basketball")
        {
            AbsProductPtr objPtr(new BasketBall());
            return objPtr;
        }
        if(productName=="football")
        {
            AbsProductPtr objPtr(new Football());
            return objPtr;
        }
        return nullptr;
    }

};

FactoryProduct::FactoryProduct(/* args */)
{
}

FactoryProduct::~FactoryProduct()
{
}

int main()
{
    FactoryProduct factory;
    AbsProductPtr p = move(factory.getProduct("basketball"));
    p->productName();
    //忘了delete 当然是main可能无所谓。为了好一点使用智能指针
    //如果要添加一个产品怎么办？改了产品类还得改工厂类 不符合开闭原则
}