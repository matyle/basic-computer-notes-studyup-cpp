#include<iostream>
typedef struct Linklist{
    int val;
    LinkList *next;
}LinkList;

bool isPalindrome(LinkList *head){
    //双指针法，在反转链表的同时， 利用perv指针对比中点左右的值
    //如果是偶数个，每次走两步的快指针 一定最后跳到末尾元素的后面，slow的位置和pre的位置已经一个各分一半
    //如果是奇数个，则会指向末尾元素，此时slow会指向中间元素的，则要后移一位才能开始和pre比较


}

int main(){
    //创建一个链表
    
}