#include<memory>
#include<vector>
#include<iostream>
#include<string>
//具有对象语意 不可拷贝 应该是noncopyable的,应该继承noncopyable
//不需要都用weak_ptr 其中之一用就行了

class Parent;
typedef std::shared_ptr<Parent> ParentPtr;

class Child{
public:
    //std::weak_ptr<Parent> Parent_;
    std::weak_ptr<Parent> mom_; //赋值之后引用计数为2
    std::weak_ptr<Parent> dad_; //赋值之后引用计数为2
    Child(const ParentPtr& dad, const ParentPtr& mam){
        std::cout<<"Child constructor function"<<std::endl;
        
    }
    ~Child(){
        std::cout<<"Child deconstructor function"<<std::endl;
    }
    // void print(){
    //     std::cout<<"Parent_ count:"<<Parent_.use_count()<<std::endl;
    // }

};

typedef std::shared_ptr<Child> ChildPtr;

class Parent:std::enable_shared_from_this<Parent>{
public:
    std::weak_ptr<Parent> mySpouse;

    std::vector<ChildPtr> myChildren;// 孩子们
    Parent(){
        
        std::cout<<"Parent constructor function"<<std::endl;

    }
    ~Parent(){
        std::cout<<"Parent deconstructor function"<<std::endl;
    }

    void addChild(const ChildPtr& child){
        myChildren.push_back(child);
        //Child_.reset(new Child(shared_from_this())); //直接给this可能会出现线程安全问题
    }

    void setSpouse(const ParentPtr &spouse){
        
        mySpouse = spouse;
        
    }

    // void print(){
    //     std::cout<<"Child_ count:"<<Child_.use_count()<<std::endl;

    // }


};


int main(){

    
    auto dad = std::make_shared<Parent>();
    auto mom = std::make_shared<Parent>();
    //循环引用！！！
    dad->setSpouse(mom);
    mom->setSpouse(dad);
    auto child = std::make_shared<Child>(dad,mom);  //计数+1
    //  Child->Parent_ = Parent;  //Parent+1 为2
    // Parent->Child_ = Child; //Child+1 为2

    // Child->print();
    // Parent->print();



    std::cout<<"Child count:"<<child.use_count()<<std::endl;
    std::cout<<"dad count:"<<dad.use_count()<<std::endl;
    std::cout<<"mam count:"<<mom.use_count()<<std::endl;


}

