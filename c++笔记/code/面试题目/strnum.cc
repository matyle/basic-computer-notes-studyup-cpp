#include<iostream>
#include<string>
#include<vector>
using namespace std;
inline bool isNum(char c)
{
    return c>='0'&&c<='9';
}
int main()
{
    string s;
    string word="";
    getline(cin,s);
    int sz = s.size();
    int flag = -1;
    for(int i=0;i<sz;++i)
    {
        
        if(isNum(s[i])&&flag==-1)
        {
           word.push_back('(');
           flag = 1;
        }
        else if(!isNum(s[i])&&flag!=-1)
        {
            word.push_back(')');
            flag = -1;
        }
        word.push_back(s[i]);
    }
    cout<<word<<endl;
}