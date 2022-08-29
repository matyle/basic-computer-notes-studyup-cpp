#include<vector>
#include<iostream>
#include<stdio.h>
#include<algorithm>
using namespace std;
//前序和中序遍历序列分割 找叶子节点
// 每次按根分
vector<int> res;
void leafCount(vector<int> &pre,vector<int>&inorder,
int preStart,int preEnd,int inStart,int inEnd)
{

    int rootval = pre[preStart];
    int partId;
    if(inStart==inEnd)
    {
        res.push_back(inStart);
        return;
    }
    for(int i=inStart;i<inEnd;++i)
    {
        if(inorder[i]==rootval)
        {
            partId = i;
            break;
        }
    }

    //建树思维递归
    int left_size = partId-inStart; //减的是中序的
    //递归
    leafCount(pre,inorder,preStart,preStart+left_size,inStart,partId-1);
    leafCount(pre,inorder,preStart+left_size+1,preEnd,partId+1,inEnd);
}



int main()
{
    int n;
    scanf("%d",&n);
    vector<int> nums;
    for(int i=0;i<n;++i)
    {
        int x;
        scanf("%d",&x);
        nums.push_back(x);
    }
    vector<int> inorder(nums);
    sort(nums.begin(),nums.end());

    leafCount(nums,inorder,0,nums.size()-1,0,inorder.size()-1);

    
}