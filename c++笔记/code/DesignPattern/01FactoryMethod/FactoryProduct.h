#include "AbstrctFactory.h"
#include "Product.h"
#include <iostream>
#include <memory>
using namespace std;
typedef unique_ptr<AbstrctFactory> AbsFactoryPtr;
class BasketballFac:public AbstrctFactory
{
private:
    /* data */
public:
    //使用智能指针
    BasketballFac()
    {
        cout<<"BasketballFac"<<endl;
    }
    AbsProductPtr getProduct()
    {
      
        AbsProductPtr objPtr(new BasketBall());
        return objPtr;
    }

};

class FootballFac:public AbstrctFactory
{
private:
    /* data */
public:
    //使用智能指针
    FootballFac()
    {
        cout<<"FootballFac"<<endl;

    }
    AbsProductPtr getProduct()
    {
      
        AbsProductPtr objPtr(new Football());
        return objPtr;
    }

};


//添加新产品 不会影响其他类