#include <functional>
#include <map>
#include <string>
#include <iostream>
#include <stack>
typedef std::function<int(int,int)> Functor;
  int add(int i,int j){return i+j;} //函数类型
  auto mod = [](int i,int j){return i % j;}; //lambda 产生一个未命名的函数对象类
  //函数对象类
  struct divide{
      int operator()(int denominator,int divisor){
          return denominator/divisor;
      }
  };

  int main()
  {
      struct divide div;
      Functor f1 = add;
      Functor f2 = mod;
      Functor f3 = div;
      std::map<char,Functor> mp;
      std::stack<int> dig;

      mp.insert({'+',f1});
      mp.insert({'%',f2});
      mp.insert({'/',f3});
      int a,b;
      std::string str; //输入计算表达式
      char op;

      std::getline(std::cin,str);

      for(int i = 0;str[i]!='\0';++i)
      {
          if(str[i]>='0'&&str[i]<='9')
          {
              dig.push(str[i]-'0');
          }
          else
          {
              op = str[i];
          }
      }
      b = dig.top();
      dig.pop();
      a = dig.top();
      dig.pop();
      std::cout<<"result: "<<mp[op](a,b)<<std::endl;
  }