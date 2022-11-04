//冒泡排序
//插入
//选择
#include <iostream>
void bubble(int a[], int n);
void insertsort(int a[], int n);
void chiocesort(int a[], int n);
void mergesort(int a[], int n);
void merge(int a[], int p, int q, int r);
void mergesort_c(int a[], int p, int r);

void quick_sort(int a[], int n);
void quick_sort_c(int a[], int p, int r);
int partition(int a[], int p, int r);
void swap(int &a, int &b);
int main()
{
    int n = 6;
    int a[6] = {4, 5, 6, 3, 2, 1};
    int b[6] = {4, 5, 6, 3, 2, 1};
    int c[6] = {4, 5, 6, 3, 2, 1};
    int d[6] = {4, 5, 6, 3, 2, 1};
    int e[6] = {4, 7, 6, 10, 2, 1};

    bubble(a, 6);
    insertsort(b, 6);
    chiocesort(c, 6);
    mergesort(d, 6);
    quick_sort(e, 6);
    for (int i = 0; i < 6; ++i)
    {
        std::cout << a[i];
    }
    std::cout << std::endl;
    for (int i = 0; i < 6; ++i)
    {
        std::cout << b[i];
    }
    std::cout << std::endl;
    for (int i = 0; i < 6; ++i)
    {
        std::cout << c[i];
    }
    std::cout << std::endl;
    for (int i = 0; i < 6; ++i)
    {
        std::cout << d[i];
    }
    std::cout << std::endl;
    for (int i = 0; i < 6; ++i)
    {
        std::cout << e[i];
    }
}

void bubble(int a[], int n)
{
    //冒泡排序 带flag
    bool flag = true;
    for (int i = 0; i < n; ++i)
    {
        flag = false; //表示还没有交换
        for (int j = 0; j < n - i - 1; ++j)
        { //每一个元素进行比较,因为循环中有j+1呀！！ 所以j最多到n-2
            if (a[j] > a[j + 1])
            {
                int temp = a[j + 1];
                a[j + 1] = a[j];
                a[j] = temp;
                flag = true;
            }
        }
        if (!flag)
            break;
    }
    return;
}
void insertsort(int a[], int n)
{
    //插入排序，从未有序区插入到有序区
    int value = 0; //未有序区的值
    //默认 a[0]有序
    int j = 0;
    for (int i = 1; i < n; ++i)
    {
        //从a[1]开始
        value = a[i];
        for (j = i - 1; j >= 0; --j)
        {
            if (a[j] > value)
            {                    //如果说比当前小，继续往前移动，因为前面可能还有更小的
                a[j + 1] = a[j]; //移动元素 要从后往前移动！！！！
            }
            else
            {
                break; //如果当前值比最后一个还大 不可能比前面的大 因为都是升序有序
            }
        }
        //j到0时，这时还会执行一次，然后执行完再执行--j就变成了-1！！！
        a[j + 1] = value; //为什么是j+1
    }
    return;
}

void chiocesort(int a[], int n)
{
    //选择排序
    int min_val = 0;
    int min_idx = 0;
    for (int i = 0; i < n; ++i)
    {
        min_idx = i;
        for (int j = i + 1; j < n; ++j)
        { //不用移动 所以不需要逆序，只需要交换
            if (a[j] < a[min_idx])
            {
                min_idx = j;
            }
        }
        //交换 a[i]和最小值
        int temp = a[i];
        a[i] = a[min_idx];
        a[min_idx] = temp;
    }
    return;
}

// 归并排序
void mergesort(int a[], int n)
{
    //归并排序 分治思想
    mergesort_c(a, 0, n - 1); //0~n-1
}
void mergesort_c(int a[], int left, int right)
{
    if (left >= right)
        return;

    int mid = (left + right) / 2; //取中间位置 (p-n/2)+n

    mergesort_c(a, left, mid-1);
    mergesort_c(a, mid + 1, right);
    merge(a, left, mid, right);
}

void merge(int a[], int left, int mid, int right)
{

    int i = left, j = mid + 1, cnt = 0;
    //数组
    int *tmp = new int[right - left + 1]; //每次将新的顺序插入到tmp中
    while (i <= mid && j <= right) //前一半和后一半
    {
        if (a[i] <= a[j]) //
        {
            tmp[cnt++] = a[i++];
        }
        else
        {
            tmp[cnt++] = a[j++];
        }
    }
    while (i <= mid)
    {
        tmp[cnt++] = a[i++];
    }
    while (j <= right)
    {
        tmp[cnt++] = a[j++];
    }

    //拷贝到原数组
    for (int i = 0; i <= right - left; ++i)
    {                      //这里不对 tmp[r-p+1]
        a[left + i] = tmp[i]; //为什么？因为tmp是从0开始的！！！！a从left开始
    }
    delete[] tmp;
    return;
}

//快速排序
void quick_sort(int a[], int n)
{
    quick_sort_c(a, 0, n - 1);
}

void quick_sort_c(int a[], int p, int r)
{

    if (p >= r)
        return;

    int pivot = partition(a, 0, r);

    quick_sort_c(a, 0, pivot - 1); //因为pivot在数组中位置已经固定所以不需要再传pivot，而归并需要合并两个数组
    quick_sort_c(a, pivot + 1, r);
}

int partition(int a[], int p, int r)
{
    //三数取中
    // int q = (p+r)/2;
    // int pivot = 0;
    // if(a[r]>a[p]&&a[r]<a[q]||a[r]<a[p]&&a[r]>a[q]) pivot = a[r];
    // else if(a[p]>a[r]&&a[p]<a[q]||a[p]>a[q]&&a[p]<a[r]) pivot = a[p];
    // else pivot = a[q];

    int pivot = a[r];
    int i = p; //p~i-1都是小于pivot的元素 已处理区间，i-r-1都是未处理区间，r已经被拿来当pviot了
    int j = p; //利用j和i交换 从而避免移动元素

    for (; j < r; j++)
    {
        if (a[j] < pivot)
        {
            swap(a[i], a[j]);
            i++;
        }
    }
    swap(a[i], a[r]); //pivot的值
    return i;
}

void swap(int &a, int &b)
{
    int tmp = a;
    a = b;
    b = tmp;
}