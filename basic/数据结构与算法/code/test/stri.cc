#include<string>
#include<math.h>
#include<iostream>
#include<algorithm>
using namespace std;
class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 
     * @param s string字符串 
     * @param k int整型 
     * @return int整型
     */
    
    int maxValue(string s, int k) {
        // write code here
        string window;
        int max_val = -1;
        int left = 0, right=0;
        while(right<s.size())
        {
            char c = s[right];
            ++right;
            window.push_back(c);
            //收缩窗口
            while(window.size()==k)
            {
                max_val = max(max_val,atoi(window));
                ++left;
                window.erase(window.begin());
            }
        }
        return max_val;
        
    }
    int atoi(string s)
    {
        int ans = 0;
        int n = s.size()-1;
        for(int i=n;i>=0;--i)
        {
            ans += (s[i]-'0') * pow(10,n-i);
            cout<<ans<<" ";
        }
        cout<<endl;
        return ans;
    }
};

int main()
{
    Solution s;
    string str= "123";
    cout<<s.atoi(str);
    return 0;
}