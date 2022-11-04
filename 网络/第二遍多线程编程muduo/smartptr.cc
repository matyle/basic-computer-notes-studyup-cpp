#include<memory>
#include<vector>
#include<iostream>
#include<string>
//具有对象语意 不可拷贝 应该是noncopyable的,应该继承noncopyable
typedef std::shared_ptr<Parent> ParentPtr;
typedef std::shared_ptr<Child> ChildPtr;
class Child;
class Parent:std::enable_shared_from_this<Parent>{
public:
    //std::weak_ptr<Child> Child_;
    std::shared_ptr<Child> Child_;
    Parent(){
        
        std::cout<<"Parent constructor function"<<std::endl;

    }
    ~Parent(){
        std::cout<<"Parent deconstructor function"<<std::endl;
    }

    void addChild(){
        Child_.reset(new Child(shared_from_this())); //直接给this可能会出现线程安全问题
    }

    Parent (const Parent& that){
        Child_ = that.Child_;
    }
    void print(){
        std::cout<<"Child_ count:"<<Child_.use_count()<<std::endl;

    }


};

class Child{
public:
    //std::weak_ptr<Parent> Parent_;
    std::weak_ptr<Parent> Parent_; //赋值之后引用计数为2

    Child(const ParentPtr& myParent ){
        std::cout<<"Child constructor function"<<std::endl;
        
    }
    ~Child(){
        std::cout<<"Child deconstructor function"<<std::endl;
    }
    Child(const Child& that){
        Parent_ = that.Parent_;
        std::cout<<"Parent_ count:"<<Parent_.use_count()<<std::endl;

    }
    void print(){
        std::cout<<"Parent_ count:"<<Parent_.use_count()<<std::endl;
    }

};
int main(){

    auto child = std::make_shared<Child>();  //计数+1
    auto Parent = std::make_shared<Parent>();

     Child->Parent_ = Parent;  //Parent+1 为2
    Parent->Child_ = Child; //Child+1 为2

    Child->print();
    Parent->print();



    std::cout<<"Child count:"<<Child.use_count()<<std::endl;
    std::cout<<"Parent count:"<<Parent.use_count()<<std::endl;

}

