#include<vector>
#include<iostream>
using namespace std;
class Solution {
public:
    /**
     * Note: 类名、方法名、参数名已经指定，请勿修改
     *
     * 
     * 
     * @param grid int整型vector<vector<>> 
     * @return int整型
     */
    int numberOfIslands(vector<vector<int> >& grid) {
        // write code here
        int rsz = grid.size();
        if(rsz==0) return 0;
        int csz = grid[0].size(); //列
        int ans = 0;
        for(int row = 0;row<rsz;++row)
        {
            for(int col = 0;col<csz;++col)
            {
                if(grid[row][col]==1)
                {
                    ans++;
                    dfs(grid,row,col);
                }
            }
        }
        return ans;
    }

    void dfs(vector<vector<int> >& grid,int row,int col)
    {
        int rsz = grid.size();
        int csz = grid[0].size();
        grid[row][col] = 0;
        //四个方向搜索
        if(row-1>=0&&grid[row-1][col]==1) dfs(grid,row-1,col);// 往上一行搜
        if(row+1<rsz&&grid[row+1][col]==1) dfs(grid,row+1,col);// 往下一行搜
        if(col-1>=0&&grid[row][col-1]==1) dfs(grid,row,col-1);// 列搜
        if(col+1<csz&&grid[row][col+1]==1) dfs(grid,row,col+1);// 往下一行搜

    }
};

int main()
{
    Solution s;
    vector<vector<int>> nums ={{1,1,1,1,0},{1,1,0,1,0},{1,1,0,0,0},{0,0,0,0,0}};
    cout<<s.numberOfIslands(nums);
}