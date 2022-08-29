#include<iostream>
#include<math.h>
using namespace std;
int main()
{
    int n;
    cin>>n;
    if(n<=2) cout<<n<<endl;
    int k = 0;
    int i = 0;
    cout<<n<<"=";
    for(i=2;i<n;)
    {

        if(n%i==0)  //即能整除则输出
        {
            cout<<i<<"*";
            n/=i; //n减小 
        }
        else 
        ++i;
        
    }
    cout<<i;
    
    return 0;
}