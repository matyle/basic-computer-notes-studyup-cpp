#include<iostream>
#include<stdio.h>
#include<vector>
using namespace std;
//vector<vector<int>> dir= {{-1,0},{1,0},{0,1},{0,-1}};
int ans=1;
void check(vector<vector<int>>& nums,int i,int j)
{
    if(i<0||i>=nums.size()||j<0||nums[i][j]==2) 
    {
        return;
    }

    if(j>=nums[0].size())
    {
        ans++;
        return;
    }

    nums[i][j]=2;
    
    check(nums,i+1,j);
    check(nums,i-1,j);
    check(nums,i,j-1);
    check(nums,i,j+1);
    nums[i][j]=1;
    return;
}
int main()
{
    int n,m;
    cin>>n>>m;
    vector<vector<int>> nums(n,vector<int>(m,0));
    for(int i=0;i<n;++i)
    {
        for(int j=0;j<m;++j)
        {
            int x; 
            scanf("%d",&x);
            nums[i][j] = x;
        }
    }

    check(nums,0,0);
    cout<<ans<<endl;
}