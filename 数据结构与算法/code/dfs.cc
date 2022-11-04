#include<vector>
#include <iostream>
#include <vector>
using namespace std;
int ans = INT_MAX;
vector<vector<int> > dir={{1,0},{-1,0}, {0,-1}, {0,1}};
void dfs(const vector<vector<int>>& inputs,vector<vector<bool>>&visited,int x,int y,int sum)
{
    if(sum>=ans) return;
    if(x==inputs.size()-1 && y==inputs[0].size()-1)
    {
        ans = sum;
        //cout<< "ans:" <<endl;
        return;
    }

    for(int i=0;i<4;++i)
    {
        //
        int dx = x + dir[i][0];
        int dy = y + dir[i][1];
        if(dx>=inputs.size()||dy>=inputs[0].size()
            ||dx<0||dy<0||visited[dx][dy]||inputs[dx][dy]==2) continue;

        visited[dx][dy] = true;
        if(inputs[dx][dy]==0) dfs(inputs,visited,dx,dy,sum+2);
        if(inputs[dx][dy]==1) dfs(inputs,visited,dx,dy,sum+1);
        visited[dx][dy] = false;
    }
    
    
}

int main()
{
    vector<vector<int>> inputs = {{1,1,1,1,0},
                                 {0,1,0,1,0},
                                 {1,1,2,1,1},
                                 {0,2,0,0,1}
                                 };
    int res;
    int m = inputs.size();
    int n = inputs[0].size();
    cout<<m<<" "<<n<<endl;
    vector<vector<bool> > visited(m,vector<bool>(n,false));
    dfs(inputs,visited,0,0,0);

    //dp[m][n];
    // vector<vector<int>> dp(m,vector<int>(n,0));
    // dp[0][0] = 0;
    
    // for(int i=m;i>=1;--i)
    // {
    //     for(int j=n;j>=1;--j)
    //     {
    //         if(inputs[i][j+1]==0)
    //         {
    //             //费用为2
    //             dp[i][j] = dp[i][j+1] + 2;
    //         }
    //     }
    // }
    // ans = ans == INT_MAX ? -1 : ans;
    // cout<<ans<<endl;
    // return ans;

}