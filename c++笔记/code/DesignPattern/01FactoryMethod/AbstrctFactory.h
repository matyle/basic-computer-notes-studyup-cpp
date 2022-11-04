#ifndef ABSTRACTFACTORY_H
#define ABSTRACTFACTORY_H

#include "AbstractProduct.h"
#include <memory>
typedef std::unique_ptr<AbstractProduct> AbsProductPtr;
class AbstrctFactory
{
private:
    /* data */
public:
    AbstrctFactory(/* args */);
    virtual ~AbstrctFactory();
    virtual AbsProductPtr getProduct()=0;
};

AbstrctFactory::AbstrctFactory(/* args */)
{
    
}

AbstrctFactory::~AbstrctFactory()
{
}

#endif