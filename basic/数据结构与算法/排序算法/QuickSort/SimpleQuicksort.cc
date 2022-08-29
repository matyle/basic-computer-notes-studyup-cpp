#include<vector>
#include<time.h>
#include<cstdlib>
#include<iostream>
using namespace std;
int partition(vector<int> &arr,int left,int right)
{
    //i,j从左边开始，注意不是0 易错点1
    int i=left; 
    int j=left; //若为从小到大 [0,i)表示小于pivot，(i,j]大于pivot 区间概念特别重要
    int pivot = arr[right];//选择最后一个数为轴，这个是随意选的 易错点2

    for(;j<right;++j) //j从0开始搜索
    {   
        //取决于怎么排序，从小到大还是从大到小？
        //从小到大则[0,i]表示小于pivot,(i,j]大于pivot,现在arr[j]比pivot小
        //说明应该到[0,i]区间，因此交换
        if(arr[j]<pivot)
        {
            swap(arr[i],arr[j]); //交换i,j
            i++; //此时i的左边都是比pivot小的啦

        }
    }
    //操作完毕 表示区间已经有序
    // arr[right] = arr[i]; //中间的值和pivot位置的值交换
    // arr[i] = pivot; //把pivot放到中间位置
    swap(arr[right],arr[i]);
    return i;//返回中间位置
}

void quickSort(vector<int> &arr,int left,int right)
{
    //base case 易错点
    if(left>=right) return;
    int pivot = partition(arr,left,right);
    quickSort(arr,0,pivot-1);//排左边
    quickSort(arr,pivot+1,right);//排右边
    
}

void sort(vector<int> &arr)
{
    int n = arr.size()-1;
    quickSort(arr,0,n);
}

int main()
{
    int n = 1000;
    int l = 0,r = 1000;
    vector<int> arr(n,0);
    srand(time(NULL));
    for(int i=0;i<n;++i)
    { 
        arr[i] = rand() % (r - l + 1) + l;
    }
    for(int i : arr)
        cout<<" "<<i;
    
    
    cout<<endl;
    sort(arr);
    for(int i : arr)
        cout<<" "<<i;
    return 0;
}