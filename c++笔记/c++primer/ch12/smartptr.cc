#include<memory>
#include<vector>
#include<iostream>
#include<string>
class Son;
class Father{
public:
    //std::weak_ptr<Son> son_;
    std::shared_ptr<Son> son_;
    //Son *son_;
    Father(){
        
        std::cout<<"Father constructor function"<<std::endl;

    }
    ~Father(){
        delete son_;
        std::cout<<"Father deconstructor function"<<std::endl;
    }

    Father (const Father& that){
        son_ = that.son_;
    }
    void print(){
        //std::cout<<"son_ count:"<<son_.use_count()<<std::endl;
        std::cout<<"son_ count:"<<std::endl;


    }


};

class Son{
public:
    //std::weak_ptr<Father> father_;
    std::shared_ptr<Father> father_; //赋值之后引用计数为2
    Father *father_;
    Son(){
        std::cout<<"Son constructor function"<<std::endl;
        
    }
    ~Son(){
        delete father_;
        std::cout<<"Son deconstructor function"<<std::endl;
    }
    Son(const Son& that){
        father_ = that.father_;
        //std::cout<<"father_ count:"<<father_.use_count()<<std::endl;

    }
    void print(){
        //std::cout<<"father_ count:"<<father_.use_count()<<std::endl;
        std::cout<<"father_ count:"<<std::endl;

    }

};
int main(){
    // typedef std::shared_ptr<std::string> Ptr;
    // std::vector<Ptr> vec;//vector中存储ptr

    // std::string str = "hello world";
    // std::string str2 = "hello c++";

    // Ptr pStr = std::make_shared<std::string>(str); //
    // Ptr pStr2 = std::make_shared<std::string>(str2); //
    // pStr2 = pStr2;
    // vec.push_back(pStr);
    // //vec.push_back(pStr2);
    // auto it = vec.begin();
    // vec.erase(it);
    // std::cout<<pStr2.use_count()<<std::endl;

    auto son = std::make_shared<Son>();  //计数+1
    auto father = std::make_shared<Father>();

    // Son *son = new Son();
    // Father *father = new Father();

     son->father_ = father;  //father+1 为2
    father->son_ = son; //son+1 为2

    son->print();
    father->print();

    // delete son;
    // delete father;

    //
    //我想释放father但是发现 father_指针的计数是1 不能释放



    // std::cout<<"son count:"<<son.use_count()<<std::endl;
    // std::cout<<"father count:"<<father.use_count()<<std::endl;

}

