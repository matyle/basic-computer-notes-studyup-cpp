#include "pimpltest.h"
#include "Pimpl.h"
void pimpltest::print()
{
    pim->print();
}

pimpltest::pimpltest(/* args */)
{
    pim = new Pimpl();
}

pimpltest::~pimpltest()
{
    delete pim;
}
