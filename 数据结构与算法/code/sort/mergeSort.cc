#include<vector>
using namespace std;
void mergeSort(vector<int> &nums,vector<int> &dst,int left,int right)
{
    if(left>=right) return;
    
    int mid = left + ((right-left)>>1);
    mergeSort(nums,dst,left,mid-1);
    mergeSort(nums,dst,mid+1,right);


    //排序 分两段排序
    int i = left , j = mid, k = left;

    while(i<mid || j<=right) //分别来
    {
        if(nums[i]<nums[j])
        {
            dst[k++] = nums[i]; 
        }
        else
        {
            dst[k++] = nums[j];
        }
    }
    

}