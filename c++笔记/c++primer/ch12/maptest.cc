#include<map>
#include<string>
#include<iostream>
int main()
{
    std::map<int,std::string> mp;
    mp[1] = "hello";
    mp[2] = "hello2";
    mp[3] = "hello3";
    mp[14] = "hello14";
    mp[24] = "hello24";
    mp[11] = "hello11";

    auto iter = mp.lower_bound(25); //指向第一个大于等于25的 没有
    auto iter1 = mp.upper_bound(3);  //第一个大于25的

    std::cout<<iter->second<<std::endl;
    std::cout<<iter1->second<<std::endl;

}