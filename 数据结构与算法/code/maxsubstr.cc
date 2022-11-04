#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
using namespace std;
int lengthOfLongestSubstring(string s)
{
    // 滑动窗口 只能做子序列
    // 什么时候收缩窗口？有重复字符的时
    int left = 0, right = 0;
    int ans = 0;
    unordered_map<int, int> window;
    while (right < s.size())
    {
        char d = s[right];
        window[d]++;
        ++right;
        while (window[d] > 1) //有重复 该收缩了
        {
            char c = s[left];
            ++left;
            window[c]--;
            if (window[c] == 0)
                window.erase(c);
        }
        int sz = window.size();
        cout<<sz<<" ";
        ans = max(ans,sz);
    }
    cout<<endl;
    return ans;

    //动态规划
    // int sz = s.size();
    // vector<vector<int>> dp(sz,vector<int>(sz,0));
    // for(int i=0;i<sz;++i) dp[i][i] = 1;
    // for(int i=sz-2;i>=0;--i)
    // {
    //     for(int j=i+1;j<=sz-1;++j)
    //     {
            
    //         if(s[i]==s[j])
    //         {
    //             dp[i][j] = max(dp[i][j-1],dp[i+1][j]);
    //         }
    //         else
    //         {
    //             dp[i][j] = dp[i][j-1] + 1;
    //         }
    //         cout<<dp[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    // return dp[0][sz-1];
}

int main()
{
    //string s = "pwwkew";
    string s = "abcabcbb";
    cout<<lengthOfLongestSubstring(s)<<endl;
    
    return 0;

}
