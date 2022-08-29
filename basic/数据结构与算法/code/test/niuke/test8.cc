#include<stdio.h>
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int slove(vector<vector<int>>& nums)
{
    sort(nums.begin(),nums.end(),
    [](vector<int> &a,vector<int> &b){
        if(b[1]==a[1]) return a[0]
     }    );

}

int main()
{
    int n;
    scanf("%d,",&n);
    vector<vector<int> > nums;
    
    for(int i=0;i<n;++i)
    {
       vector<int> temp;
        for(int j=0;j<2;++j)
        {
            int x;
            scanf("%d",&x);
            temp.push_back(x);
        }
        nums.push_back(temp);
    }
    
}