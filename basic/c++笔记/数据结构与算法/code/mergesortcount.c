#include<stdio.h>
#include <string.h>
int sum = 0;



void merge(int a[],int left,int mid,int right)
{
    //合并 
    int i  = left, j = mid+1; //j从q+1开始 也就是mid+1 [p,mid] [mid+1,r]
    int k = 0;
    //用一个新数组暂存 合并的数组
    int tmp[right-left+1]; //r-p+1个
    for(int l=0;l<right-left+1;++l) tmp[l] = 0; //初始化
    while(i<=mid && j<=right)
    {
        if(a[i]<=a[j])
        {
            tmp[k++] = a[i++]; //i往前走 小的存在temp中
        }
        else
        {
            //逆序对更新
            sum += (mid-i+1); //统计p-q之间 a[i] 大于a[j]的个数 为啥？ 
            //一但a[i]>a[j] i后面的值肯定都比a[j]大
            tmp[k++] = a[j++];
        }
    }

    while(i<=mid) //说明i还没走完
    {
        tmp[k++] = a[i++];
    }

    while(j<=right) //说明j还没走完
    {
        tmp[k++] = a[j++];
    }

    //将tmp部分拷贝回a中
    for(int l=0;l<right-left+1;++l)
    {
        a[left+l] = tmp[l];
    }

    //
}

void mergeSortCount(int a[],int p,int r)
{
    if(p >= r) return;//递归终止条件
    int mid = (p + r) / 2;//q为分割的一半

    mergeSortCount(a,p,mid); //前面那一半
    mergeSortCount(a,mid+1,r);
    merge(a,p,mid,r);
}


int main()
{
    //int a[6] = {2,4,3,1,5,6};
    int a[6] = {1,5,6,2,3,4};

    mergeSortCount(a,0,6-1);
    for(int i=0;i<6;++i)
    {
        printf("%d ",a[i]);

    }
    printf("\n");
    printf("%d\n",sum);
    //return sum;
}