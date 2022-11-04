    #include <vector>
    #include <iostream>
    using namespace std;
    int minCostClimbingStairs(vector<int>& cost) {
        //
        //f3 = min(f2,f1)
        //f5 = min(f4,f3)

        //递归 自底向上
        int n = cost.size(); //楼梯数
        if(n==1) return cost[0];
        if(n==2) return min(cost[0],cost[1]);

        vector<int> dp(n+1,0);
        //到达i层楼的最少体力
        dp[0]=dp[1]=0; //dp[1]啥意思，就是index=1的楼，比如[10],我可以从-1直接到1，也可以从0到1
        //dp[2] = min(cost[0],cost[1]);
        for(int i=2;i<=n;++i)
        {
            dp[i] = min(dp[i-1]+cost[i-1],dp[i-2]+cost[i-2]); //到dp[i]的两种选择
        }
        for(auto i : dp)
        {
            cout<<i<<" ";
        }
        cout<<endl;
        return dp[n]; //n
    }

    int main()
    {
        //vector<int> cost = {1,100,1,1,1,100,1,1,100,1};
        vector<int> cost = {10,15,4};
        cout<< minCostClimbingStairs(cost)<<endl;
        return 0;
    }