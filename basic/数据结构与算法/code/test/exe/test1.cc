#include<unordered_map>
#include<iostream>
#include<string>
#include<initializer_list>
using namespace std;
class Solution {
public:
    /**
     * 
     * @param s string字符串 
     * @return int整型
     */
    int romanToInt(string s) {
        // write code here
        unordered_map<char, int> roman={{'I',1},{'V',5},{'X',10},
                                        {'L',50},{'C',100},{'D',500},{'M',1000}};
        int ans=0;
        int i = 0;
        int n = s.size();
        while(i<n)
        {
            switch (s[i])
            {
               case 'I': 
                    {
                    if(i<n-1&&s[i+1]=='V'||s[i+1]=='X')
                    {
                        ans += roman[s[i+1]] - 1;
                        i=i+2;
                    }
                    else{
                        ans += 1;
                        ++i;
                    }
                        
                    break;
                    }
                case 'X':
                    {
                    if(i<n-1&&s[i+1]=='L'||s[i+1]=='C')
                    {
                        ans += roman[s[i+1]] - 10;
                        i=i+2;
                    }
                    else
                    {
                        ans += 10;
                          ++i;
                    }
                    break;
                    }

                    
                case 'C':
                    {
                    if(i<n-1&&s[i+1]=='M'||s[i+1]=='D')
                    {
                        ans += roman[s[i+1]] - 100;
                        i=i+2;
                    }
                    else 
                    {
                        ans += 100;
                        ++i;
                    }
                        
                    break;
                    }

                default:
                    {
                    ans += roman[s[i]];
                    ++i;
                    }
                    
            }
           
        }
        return ans;
    }
};

int main()
{
    Solution s;
    string str;
    cin>>str;
    cout<<s.romanToInt(str)<<endl;
}