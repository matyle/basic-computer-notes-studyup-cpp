// CSAPP Excercise 5.21
//
// 求前置和
/* 
void psum1(float a[], float p[], long int n)
{
    long int i;
    p[0] = a[0];
    for (i = 1; i < n; i++)
        p[i] = p[i-1] + a[i];
}*/

// 循环展开和累积变量，不知道怎么使用重新结合。
void psum1(float a[], float p[], long int n)
{
    long int i;
    int sum1 = p[0] = a[0];
    int sum2 = p[1] = p[0] + a[1];
    for (i = 1; i < n; i += 2)
    {
        sum1 = sum2 + a[i];
        sum2 = sum1 + a[i+1];
        p[i] = sum1;
        p[i+1] = sum2;
    }

    for (; i < n; i++)
        p[i] = sum2 + a[i];
}