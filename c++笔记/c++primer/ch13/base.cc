#include<vector>
#include<iostream>
class Base
{
public:
    virtual void vf()
    {
        std::cout<<"virtual Base"<<std::endl;
    }
    void f()
    {
        std::cout<<"Base"<<std::endl;
    }
    
private:
    int i;
};

class D : public Base
{
public:
    D(int val):j(val){}
    D(){}
    virtual void vf()
    {
        std::cout<<"virtual D"<<std::endl;
    }
    void foo()
    {
        std::cout<<"D"<<std::endl;
    }
    void set(int val){
        j = val;
    }
    int get()
    {
        return j;
    }
private:
    int j;
};


int main()
{
    std::vector<Base> Bvec;
    std::vector<Base*> Bvecp;
    std::vector<D> Dvec;
    Base b;
    Base *b2 = new Base();
    Base *bd = new D(4); //
    D *dpd = new D();
    D d;
    Base bdcast = static_cast<D> (d); //子类转父类 安全 因为子类本身包含父类信息
    Base *bdcastp = static_cast<D*> (dpd); //子类转父类 安全 因为子类本身包含父类信息
    

    bdcastp->f();
    bdcastp->vf(); //虚函数




    //bdcast->foo();
    //D dbcast = static_cast<Base>(b); //不安全的转换

    //D *dbdycast = dynamic_cast<D*>(b2); //不安全的转换

    
    //D d2 = dynamic_cast<D>(b2);
    Bvec.push_back(d);
    //Dvec.push_back(b);
    Base b1 = Bvec.front();
    Bvecp.push_back(bd);
    Base *vbp = Bvecp.front();

    D *vdp = dynamic_cast<D*>(vbp);

    vdp->foo();
    std::cout<<vdp->get()<<std::endl;
    b1.f();
}