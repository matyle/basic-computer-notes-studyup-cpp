#include<string>
#include<iostream>
using namespace std;
string caseTransform(string &str)
{
    //四种输入   
    int state; //0表示首字母大写，1 表示首字母小写 后面全大写
    string ans = str + " ";
    for(int i=0;i<str.size();++i)
    {
        if(i==0) 
        {
            if('A'<=str[i]&&str[i]<='Z')
            {
                str[i] = str[i] + ('a'-'A'); //大写转小写
                state = 0;
                ans = ans + str + " ";
                continue;
            }
            else if('a'<=str[i]&&str[i]<='z'&&str.find('_')==-1&&str.find('-')==-1)
            {
                str[i] = str[i] - ('a'-'A'); //小写转大写
                state = 1;
                continue;
            }

            else state==2;

        }
        if(state==0 || state==1)
        {
            if('A'<=str[i]&&str[i]<='Z')
            {
                str[i] = str[i] + 32;
                str.insert(str.begin()+i, '_');
            }
        }

        else if(state==2) //带 ’_‘
        {
            auto f = str.find('_');
            int f2 = str.find('-');

            if(f!=-1)
            {
                while(f!=-1)
                {
                    str.replace(str.begin()+f,str.begin()+f+1,1,'-');
                    f = str.find('_');
                }
                ans = ans + str + " "; //加入_

                f = str.find('-'); //替换为大写
                while(f!=-1)
                {
                    str.erase(str.begin()+f);
                    str[f] = str[f] - 32;
                    f = str.find('-');
                }
                ans = ans + str + " "; 
            }
            else
            {
                while(f2!=-1)
                {
                    str.replace(str.begin()+f,str.begin()+f+1,1,'_');
                    f2 = str.find('-');
                }
                ans = ans + str + " "; //加入_
                auto f = str.find('_');
                while(f!=-1)
                {
                    str.erase(str.begin()+f);
                    str[f] = str[f] - 32;
                    f = str.find('-');
                }
                ans = ans + str + " "; 
            }
            

        }

    }
    if(state==0||state==1)
    {
        ans = ans + str + " ";
        auto f = str.find('_');
        while(f!=-1)
        {
            str.replace(str.begin()+f,str.begin()+f+1,1,'-');
            f = str.find('_');
        }
        ans = ans + str;
    }

    else  //将首字母大写
    {
        str[0] = str[0] - 32;
         ans = ans + str;
    }

    return ans;
    
}

int main()
{
    string str;
    while(cin>>str)
    {
        cout<<caseTransform(str)<<endl;
    }
}