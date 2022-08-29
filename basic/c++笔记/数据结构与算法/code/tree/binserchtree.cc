#include "TreeNode.h"

//构建树
TreeNode<int>* buidBinarySerchTree(){

}

int find(const TreeNode<int> &root,int key){

}

bool insertNode(TreeNode<int> *root,int key){
    //跟查找类似，一直找到某个值的左孩子或者右孩子为空
}

void deleteNode(TreeNode<int> *root,int val){
    //三种情况
    //1.要删的节点没有孩子
    //2.要删的节点只有一个孩子 把要删除的父节点的指向删除节点的指针指向孙子节点
    //3.要删的节点有2个孩子，这时需要把右边最小的节点（右边最小的节点肯定没左孩子，
    //且肯定比要删除的节点的左孩子大。因此是最合适的。然后要删除的节点值交换或者覆盖要删除
    //节点的值，然后删除右边最小的这个节点即可

    TreeNode<int>* p = root;//要删除的节点
    TreeNode<int>* pp = nullptr;//记录父节点

    while(p!=nullptr && p->val!=val){
        pp = p ; //记录父节点
        if(val>p->val) p = p->right;
        else p = p -> left;
    }
    if(!p) return;

    

    //第三种情况

    if(p->left&&p->right){ //查找右子树的最小节点
        //都不为空
        TreeNode<int> * minP = p->right; //指向右子树节点
        TreeNode<int> * minPP; //记录左子树节点

        while(minP->left){
            minPP = minP;
            minP = minP->left; //一直往左边找
        }
        p->val = minP->val;//覆盖要删除的值

        //利用后面代码删除minP 把p指向minp 相当于转换成了后面的情况要么没孩子，要么只有右孩子
        p = minP;
        pp = minPP;
    }
    //删除叶结点或者有一个孩子的节点
    TreeNode<int> *child; //子节点
    if(p->left!=nullptr) child = p->left;
    if(p->right!=nullptr) child = p->right;
    else child = nullptr;


    //删除节点

    if(pp==nullptr) root = child;//删除根节点

    else if(pp->left = p) pp->left = child;
    else pp->right = child;



}
