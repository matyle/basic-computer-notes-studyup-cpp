#include<iostream>
int  partition(int a[],int p,int r);
void swap(int &a,int &b){
    int tmp = a;
    a = b;
    b = tmp;
}

int findk_c(int a[],int p,int n,int k){
    if(p==n) return a[n];
    int pivot = partition(a,p,n);
    int len = pivot-p+1;
    if(len==k){
        return a[pivot]; //返回的是pivot
    }
    else if(len<k){ //在后面找
        return findk_c(a,pivot+1,n,k-len);
    }
    else{
        return findk_c(a,p,pivot-1,k); //前半段
    }

}
int  partition(int a[],int p,int r){
    //
    int pivot = a[r];
    int i = p;//p~i-1都是小于pivot的元素 已处理区间，i-r-1都是未处理区间，r已经被拿来当pviot了
    int j = p;//利用j和i交换 从而避免移动元素

    for(;j<r;j++){
        if(a[j]>pivot){ //
            swap(a[i],a[j]);
            i++;
        }
    }
    swap(a[i],a[r]);
    return i;
}
int main(){
    int e[6] = {4,7,6,3,2,1};
    int kth=0;
    std::cin>>kth;
    int k = findk_c(e,0,5,kth);
    std::cout<<"The th largest is "<<k<<std::endl; //
    return 0;
}