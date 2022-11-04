// //#include <bits/stdc++.h>
// #include<iostream>
// #include<math.h>
// #include <iomanip>
// using namespace std;
// int main()
// {
//     int m;
//     double n,ans;
//     while(cin>>n>>m)
//     {
//         ans = 0.0;
//         for(int i=1;i<=m;++i)
//         {
//                 ans += n;
//                 n = sqrt(n);
//         }
//         cout<<fixed<<setprecision(2);
//         cout<<ans<<endl;

//     }
//     return 0;

// }




#include<iostream>
#include<math.h>
#include <iomanip>
#include<vector>
#include<string>
#include<bitset>
using namespace std;
int countone(int num)
{
    int n1 = 0;
    while(num!=0)
    {
        if((num & 1) == 1)
        {
           n1++;
        }
        num = num>>1;
    }
    return n1;
}


using namespace std;
int main()
{
    int n;

    vector<int> res;
    res.push_back(1);
    string str;
    cin>>n;
    getchar();
    getline(cin,str);
    // for(int i=1;i<=n;++i)
    // {
    //     if(str[i-1]=='0')
    //      	pre[i] = pre[i-1] + 1;
    //     else 
    //     	pre[i] = pre[i-1];
    //     for(int j=1;j<i;++j)
    //     {
    //         int a = pre[j];
    //         int b = j - pre[j];
    //         int d = i - pre[i] -b;
    //         int c = i - j - d;
            
    //         if(a*d==c*b) ++res[i-1];
    //     }
        	
    // }
    
    for(int i=1;i<=n;++i)
    {
        string r = str.substr(0,i);
        
        bitset<10> bit(r);
        int num = bit.to_ullong();
        int n1 = countone(num); //1的个数
        cout<<n1<<" "<<endl;
        int n0 = i - n1 + 1; //i=2 - 0


        if(n1>=n0&&n1%n0==0)
        {
            res.push_back(n0);
        }
        
        
    }
   
    for(int i=0;i<n-1;++i)
    {
        cout<<res[i]<<" ";
    }
    cout<<res.back()<<endl;


}