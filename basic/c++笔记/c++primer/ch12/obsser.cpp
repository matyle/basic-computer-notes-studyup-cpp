#include<memory>
#include<vector>
#include<iostream>
using namespace std;
//目标中有多个观察者，
class Subject; // 前向声明

class Observer {
private:
    weak_ptr<Subject> object; 
public:
    Observer(weak_ptr<Subject> obj) : object(obj) {
        cout<<"Observer construtor"<<endl;
    }

    ~Observer(){
        cout<<"Observer deconstrutor"<<endl;
    }
    void obssay(){
        cout<<"I am observer\n"<<endl;
    }
    // 其它代码
};

class Subject {
private:
    std::vector<weak_ptr<Observer>> observers;
public:
    Subject() {cout<<"Subjuct construtor"<<endl;}
    void addObserver(weak_ptr<Observer> ob) {
        observers.push_back(ob);
    }

    void notify(){
        for(auto it=observers.begin();it!=observers.end();++it){
            shared_ptr<Observer> obj = (*it).lock();
            if(obj) obj->obssay();
        }
    }
    ~Subject() {cout<<"Subjuct deconstrutor"<<endl;}


};


int main(){
    shared_ptr<Subject> subp = make_shared<Subject>();
    //subp引用计数+1
    cout<<subp.use_count()<<endl;
    auto obs = make_shared<Observer>(subp);//obs引用了subp
    
    //然后在二次obs对象建立时subp计数再+1 =2
    cout<<subp.use_count()<<endl;

    //在subp中加入obs，然而obs中是自身的引用
    subp->addObserver(obs); // subp又引用了obs 循环引用了 在sub中引用了自身！！！！！

    subp->notify();

    cout<<subp.use_count()<<endl;

    //改成weak_ptr解决问题

    
}

