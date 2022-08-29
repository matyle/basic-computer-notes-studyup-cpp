#include<iostream>
#include<vector>
#include<algorithm>
#include<map>
#include<math.h>
using namespace std;
struct node{
    int num_child;
    int value;
    int sum;
    int child[2];
};
int sum=0;
map<int,node> node_map;
int caculate_sum(int root)
{
    int ret=node_map[root].value;
    for(int i=0;i<node_map[root].num_child;i++)
    {
        int x=caculate_sum(node_map[root].child[i]);
        ret+=x;
    }
    node_map[root].sum=ret;
    return ret;
}
int main()
{
    int n,root;
    vector<bool> isroot;
    cin>>n;
    isroot=vector<bool>(n,true);
    node tmp;
    for(int i=0;i<n;i++)
    {
        int x;
        cin>>x;
        //结点
        node_map[i].num_child=0;
        node_map[i].value=x;
        sum+=x;
    }
    //
    for(int i=0;i<n-1;i++)
    {
        int fa,child;
        cin>>fa>>child;
        isroot[child]=false;
        node_map[fa].child[node_map[fa].num_child++]=child;
    }
    for(int i=0;i<n;i++)
    {
        if(isroot[i])
        {
            root=i;
            break;
        }
    }
    caculate_sum(root);
    int maxm=0;
    int index=0;
    for(auto &i:node_map)
    {
        int cha=2*i.second.sum-sum;
        if(maxm<abs(cha)){
            maxm=abs(cha);
            index=i.first;
        }
        else if(maxm==abs(cha)&&i.first<index)
        {
            index=i.first;
        }
    }
    cout<<index<<endl;
    return 0;
}