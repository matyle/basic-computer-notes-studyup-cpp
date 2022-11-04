#include<memory>
#include<vector>
class Observer;
class Subject{
public:
    Subject();
    ~Subject();
    void registObs(std::shared_ptr<Observer> obs); // 添加观察者
    void removeObs(std::shared_ptr<Observer>  obs);
    void notifySubject(); //遍历obs


private:
    std::vector<std::shared_ptr<Observer>> observers_;


};


class Observer{
public:
    Observer();
    ~Observer();
    // void notifySubject();
private:
};

