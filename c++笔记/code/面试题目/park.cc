//停车场类
//car类
#include <memory>
#include <vector>
#include <map>
#include <utility>
using namespace std;
class park
{
private:
    /* data */
    park(int size); //单例模式
    map<int,int> cars;// (id,time);记录对应id进入停车场时间
    int maxSize;
    
    
public:
    void parkCar();
    void init();//初始化停车场对象
    void leave();//出 计算费用
    void countFee();//计费
    void destory();
    

};
