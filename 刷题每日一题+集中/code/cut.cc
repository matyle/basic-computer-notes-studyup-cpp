#include<iostream>

using namespace std;

int dp(int n)
{
    if(n<2) return 0;
    if(n==2) return 1;
    if(n==3) return 2;
    for(int i=0;i<n;++i)
    {
        int res = max(dp(n-i)*i,(n-i)*i);//
    }

}