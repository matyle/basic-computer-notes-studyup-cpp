// CSAPP Excercise 5.20
// 多项式求值
// 循环展开，并行累积，重新结合

// Reference 5.5
/*
double poly(double a[], double x, int degree)
{
    long int i;
    double result = a[0];
    double xpwr = x;
    for (i = 1; i <= degree; i++)
    {
        result += a[i] * xpwr;
        xpwr = x * xpwr;
    }

    return result;
}
*/
double poly(double a[], double x, int degree)
{
    long int i;
    double xpwr = x;
    // 循环展开， 重新结合
    double result = a[0];
    // 2次循环展开
    for (i = 1; i <= degree; i = i + 2)
    {
        result += (a[i] * xpwr + a[i+1] * (xpwr * x));
        xpwr = x * x * xpwr;
    }
    // 循环展开， 并行累积
    /*double result = a[0];
    double result1 = a[1];
    for (i = 2; i <= degree; i = i + 2)
    {
        result += a[i] * xpwr;
        result1 += a[i+1] * xpwr * x;
        xpwr = x * x * xpwr;
    }
    result += result1;*/

    // 处理剩下的元素
    for ( ; i <= degree; i++)
    {
        result += a[i] * xpwr;
        xpwr = x * xpwr;
    }

    return result;
}

// Reference 5.6
// Horner方法
/*double polyh(double a[], double x, int degree)
{
    long int i;
    double result = a[degree];
    for (i = degree - 1; i >= 0, i--)
        result = a[i] + x * result;

    return result;
}*/

// 循环展开，我无法使Honor方法使用重新结合和累积变量
double polyh(double a[], double x, int degree)
{
    long int i;
    double result = a[degree];
    
    // 
    for (i = degree - 1; i >= 0; i = i - 2) 
    {
        result = a[i-1] + x * (a[i] + x * result);
    }
    
    // 处理剩下的元素
    for ( ; i >= 0; i--) 
    {
        result = a[i] + x * result;
    }

    return result;
}