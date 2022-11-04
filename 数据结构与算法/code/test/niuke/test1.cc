#include<stdio.h>
#include<vector>
#include<string>
#include<iostream>
#include<unordered_set>
using namespace std;

string check(vector<int>& nums)
{
    //看每个数是不是2^n 或本身已经全部相等
    if(nums.size()<=1) return "YES";
    unordered_set<int> memo;
    for(int c: nums) memo.insert(c);
    if(memo.size()==1) return "YES";
    for(int c :nums)
    {
        int cnt = 0;
        while(c!=0)
        {
            if((c&1)==1) cnt++;
            c = c>>1;
        }
        if(cnt!=1) return "NO";
    }
    
    return "YES";
}

int main()
{
    int n;
    scanf("%d",&n);
    // 3 4 5 2
    // 6 8 10 4
    // 120
    vector<int> nums;
    for(int i=0;i<n;++i)
    {
        int x;
        scanf("%d",&x);
        nums.push_back(x);
    }
    cout<<check(nums)<<endl;
    return 0;
}