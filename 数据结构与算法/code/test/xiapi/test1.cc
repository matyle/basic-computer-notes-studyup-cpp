#include<iostream>
struct ListNode {
int val;
struct ListNode *next;
ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    /**
     * Note: 类名、方法名、参数名已经指定，请勿修改
     *
     * 
     * k个节点一组，逆转单链表
     * @param head ListNode类 头节点
     * @param k int整型 组大小
     * @return ListNode类
     */
    ListNode* revertLinkList(ListNode* head, int k) {
        // write code here
        ListNode* a = head;
        ListNode* b = head;
        for(int i=0;i<k;++i)
        {
            if(b) b=b->next;
            else{
                return b;
            }
        }
        
        ListNode* newlist = reverse(a,b);
        a->next = revertLinkList(b,k);
        return newlist;
        
    }
    ListNode* reverse(ListNode* head,ListNode* node) //前k个转换
    {
        //双指针
        ListNode* cur = head;
        ListNode* pre = nullptr;
        ListNode* nxt = head;
        while(cur!=node)
        {
            nxt = cur->next;
            cur->next = pre;
            pre = cur; // pre指向下一个结点
            cur = nxt;
        }
        return pre;
    }
};

int main()
{
    Solution s;
}