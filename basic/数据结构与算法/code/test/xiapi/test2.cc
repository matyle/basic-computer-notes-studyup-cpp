//二分
#include<vector>
#include<iostream>
using namespace std;

int findleft(const vector<int>& nums,int target)
{
    int left = 0,right = nums.size()-1;
    while(left<=right)
    {
        int mid = left + (right - left) / 2;
        if(nums[mid]==target)
        {
            right = mid - 1;
        }
        else if(nums[mid]<target)
        {
            left = mid + 1;
        }
        else if(nums[mid]>target)
        {
            right = mid - 1;
        }
    }
    if(left>=nums.size()||nums[left]!=target) return -1;
    return left;
}
int findright(const vector<int>& nums,int target)
{
    int left = 0,right = nums.size()-1;
    while(left<=right)
    {
        int mid = left + (right - left) / 2;
        if(nums[mid]==target)
        {
            left = mid + 1;
        }
        else if(nums[mid]<target)
        {
            left = mid + 1;
        }
        else if(nums[mid]>target)
        {
            right = mid - 1;
        }
    }
    if(right<0||nums[right]!=target) return -1;
    return right;
}
int main()
{
    vector<int> nums = {1,2,3,3,3,3,4};
    int i = 0;
    cin>>i;
    cout<<findleft(nums,i)<<","<<findright(nums,i)<<endl;
    
}