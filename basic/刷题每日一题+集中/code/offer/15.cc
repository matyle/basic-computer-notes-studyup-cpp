#include<iostream>
using namespace std;
int countOne(int n)
{
   unsigned int flag = n;
   int cnt = 0;
   while(flag!=0)
   {
       if((flag&1) == 1)
       {
           ++cnt;
       }
       flag>>=1;
   }
   return cnt;

}

int countOne2(int n)
{
   int cnt = 0;
   while(n!=0)
   {
       ++cnt;
       n = (n-1) & n;

   }
   return cnt;

}

int main()
{
    int n;
    cin>>n;
    cout<<countOne(n)<<endl;

    /*
    -1 输出32个1
    */
}