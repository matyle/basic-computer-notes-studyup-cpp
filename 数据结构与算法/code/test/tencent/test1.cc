#include<iostream>
struct TreeNode
{
    /* data */
    TreeNode *left;
    TreeNode *right;
    int val;
};

TreeNode* pruneLeaves (TreeNode* root) {
    if(root == NULL) return NULL;
    if(root ->left != NULL && root->left->left == NULL && root->left->right == NULL) return NULL;
    if(root ->right != NULL && root->right->left == NULL && root->right->right == NULL) return NULL;
    root->left = pruneLeaves(root->left); //root->left 指向处理过的节点
    root->right = pruneLeaves(root->right);
    return root;
}

int main()
{

}