#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;
vector<int> res;
int count(vector<vector<int>> vec)
{
    // vec[0]评分，vec[1]持续天数
    //value , 容量就是天数
    int max_num = 0;
    sort(vec.begin(),vec.end(),
        [](vector<int>&a,vector<int>&b)
         {
             if(a[0]==b[0]) return a[1]>b[1];
             return a[0]>b[0];
         }
        );

    unordered_map<int> dict;
    for(int i=0;i<vec.size();++i)
    {
        for
        if(i+1<=vec[i][1])
        {
            max_num += vec[i][0];
        }
        //cout<<vec[i][0]<<endl;
         
    }
    
    return max_num;
}

int main(){
    //freopen("1.in","r",stdin);
    int n,ans = 0;
    cin>>n;
    vector<vector<int>> nums;
    for(int i=0;i<n;++i)
    {
        vector<vector<int>> nums;
        int m;
        cin>>m;
        for(int i=0;i<m;++i)
        {
        vector<int> vec;
        for(int j =0; j < 2; j++){
            int x; 
            scanf("%d",&x);
            vec.push_back(x);
        }
            nums.push_back(vec);
        }

        for(auto n :nums)
        {
            for(int i :n)
            {
                cout<<i<<" ";
            }
            cout<<endl;

        }
    
        ans = count(nums);
        res.push_back(ans);
        ans = 0;
    }

    
    for(int i=0;i<res.size();++i)
    {
        cout<<res[i]<<endl;
    }
    return 0;
}