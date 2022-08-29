#include<iostream>
#include<stack>

using namespace std;

struct TreeNode
{
    TreeNode *left;
    TreeNode *right;
    int val;
    TreeNode(int v):val(v),left(NULL),right(NULL){}
};

void preorder(TreeNode* root)
{
    stack<TreeNode*> stk;
    TreeNode *p = root;
    TreeNode *pre = p; 
    while(p || !stk.empty())
    {
        if(p)
        {
            cout<<p->val<<" ";
            stk.push(p);
            p = p->left;
        }
        else
        {
            p = stk.top()->right;//往右走
            stk.pop();
        }
    }
}

void inorder(TreeNode* root)
{
    stack<TreeNode*> stk;
    TreeNode *p = root;
    TreeNode *pre = p; 
    while(p || !stk.empty())
    {
        if(p)
        {
            
            stk.push(p);
            p = p->left;
        }
        else
        {
            p = stk.top();
            cout<<p->val<<" "; //访问
            p = p->right;
            stk.pop();
        }
    }
}
void postorder(TreeNode* root)
{
    stack<TreeNode*> stk;
    TreeNode *p = root;
    TreeNode *pre = p;
    while(p || !stk.empty())
    {
        while(p) //不能用if else了 
        {
            stk.push(p);
            p = p->left;
        }
        p = stk.top();
     
        if(p->right==NULL || p->right==pre)
        {
            cout<<p->val<<" "; //访问cur
            stk.pop();
            pre = p;
            p = NULL;
        }
        else
        {
            p = p->right;
        }
    }
}


int main()
{
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    /*
      1
     2 3
    4 5
    */
    TreeNode *p = root->left;
    p->left = new TreeNode(4);
    p->right = new TreeNode(5);
    TreeNode *q = root->right;
    q->left = new TreeNode(6);
    q->right = new TreeNode(7);
    cout<<"后序"<<endl;
    postorder(root);
    cout<<endl;
    cout<<"前序"<<endl;
    preorder(root);
    cout<<endl;
    cout<<"中序"<<endl;
    inorder(root);
    return 0;
    
}