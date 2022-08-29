#include<iostream>
#include<array>
template<class T>
class Circlequeue{
    public:
        Circlequeue(int capacity):n(capacity),head(0),tail(0){
            items = new T[n];
            if(items==nullptr) return;
        }
        ~Circlequeue(){
            delete[] items;
        }

        bool enqueue(T &item);//入队
        bool dequeue(T &item);//出队

    private:
        int n;//队列大小
        int head;//头
        int tail;//尾
        T *items;

};

template<class T>
bool Circlequeue<T>::enqueue(T& item){
    if((tail+1)%n==head){ //队列满
        return false;
    }
    items[tail] = item;
    tail = (tail+1)%n;
    return true;
}
template<class T>
bool Circlequeue<T>::dequeue(T &item){
    //判断队列是否为空
    if(head==tail){
        return false;
    }
    //出队
    T temp = items[head];
    head = (head+1)%n;//队头+1
    return true;

}