#include<iostream>
#include<vector>
void JustHeap(std::vector<int>& vec_,int count, int i);
void buildHeap(std::vector<int>& arr,int count_)
{   
    
    for (int i = count_ / 2; i >= 1; --i) //i从
    {
        JustHeap(arr, count_, i); //下标从1开始
    }
}
void JustHeap(std::vector<int>& vec_,int count, int i)
{ //从哪个点开始
    //int maxpos = i;
    while (true)
    { //一直往下调整！！！
        int maxpos = i;
        //左孩子
        if (2 * i < count && vec_[2 * i] > vec_[maxpos])
            maxpos = 2 * i;
        //右孩子
        if (2 * i + 1 < count && vec_[2 * i + 1] > vec_[maxpos])
            maxpos = 2 * i + 1;

        if (maxpos == i)
            break; //等于没调整
        //交换
        std::swap(vec_[maxpos], vec_[i]);

        i = maxpos; //
    }
}

void sort(std::vector<int>& arr,int n){
    buildHeap(arr,n);
    while(n>1){
        std::swap(arr[1],arr[n-1]);
        n--;
        JustHeap(arr,n,1); //从1开始调整
    }
}
int main()
{
    std::vector<int> vec = {0,2,3,23,14,32,16,7,9}; //0不存元素
    //buildHeap(vec,9); //8个元素
    sort(vec,9);
    for(int i=1;i<vec.size();++i){
        std::cout<<vec[i]<<' ';
    }
    return 0;
}