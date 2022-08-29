#include<stdio.h>
#include<iostream>
#include<vector>
using namespace std;

// int partition(const vector<int>& nums,int left,int right)
// {
    
//     int mid = left + (right - left) / 2;
//     if(nums[left]>nums[mid]&&nums[right]>nums[mid]) ans++;
//     else if(nums[left]<nums[mid]&&nums[right]<nums[mid]) ans++;

//     partition(nums,mid)


// }
// int check(const vector<int>& nums,int start,int end)
// {
//     int mid = partition();



// }

int main()
{
    int n;
    scanf("%d,",&n);
    vector<int> nums;
    for(int i=0;i<n;++i)
    {
        int x;
        scanf("%d",&x);
        nums.push_back(x);
    }
    //找极值
    // for(int i=1;i<n-1;++i)
    // {
    //     if(nums[i]>nums[i-1]&&nums[i]>nums[i+1]||
    //     nums[i]<nums[i-1]&&nums[i]<nums[i+1])
    //     {
    //         ans++;
    //     }
    // }
    //cout<< ans+1<<endl;
    int flag = 0;
    int ans = 1;
    for(int i=1;i<n;++i)
    {
        //...i递增
        if(nums[i]>nums[i-1])
        {
            if(flag==0)  flag = 1; //状态变为递增
            if(flag==-1)
            {
                flag = 0; //恢复
                ans++;
            }
        }
        //...i 递减 flag==0 说明还没变
        else if(nums[i]<nums[i-1])
        {
            if(flag==0)  flag = -1; //状态记录为递减
            if(flag==1)
            {
                flag=0;
                ans++;
            }
        }
    }
    cout<<ans<<endl;
    return 0;
}