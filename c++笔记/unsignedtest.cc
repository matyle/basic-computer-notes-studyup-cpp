#include<iostream>
using namespace std;
int main()
{
    int n = 100;
    unsigned int un = (unsigned int)(n);
    if(n<0)
    {
        un = (unsigned int)(-n);
    }
    cout<<n<<endl;
    cout<<un<<endl;
}