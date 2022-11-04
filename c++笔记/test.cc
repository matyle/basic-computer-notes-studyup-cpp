//abcdef 12345 54321
#include<iostream>
#include<vector>
#include<string>
using namespace std;
void reverse(string& str)
{
    if(str.size()==0) return;
    int len = str.size();

    int left = 0;
    int  right = len-1;

    while(left<right)
    {
        char tmp = str[left];
        str[left] = str[right];
        str[right] = tmp;
        ++left;
        --right;
    }
    return ;
}

void helper(char str[])
{
    if(str==NULL) return;
    int len = 0;
    for(int i=0;str[i]!='\0';++i)
    {
        ++len;
    }

    int left = 0;
    int right = len-1;

    while(left<right)
    {
        char tmp = str[left];
        str[left] = str[right];
        str[right] = tmp;
        ++left;
        --right;
    }

    return ;

}
void helper1(char str[])
{
    if(str==NULL) return;
    int len = 0;
    for(int i=0;str[i]!='\0';++i)
    {
        ++len;
    }

    char *left = str;
    char *right = &str[len-1];

    while(left!=right)
    {
        char tmp = *left;
        *left = *right;
        *right = tmp;
        ++left;
        --right;
    }

    return ;

}
int i = 1;
int main()
{
    string ss="12345";
    reverse(ss);
    cout<<ss<<endl;
    int n;
    cin>>n;

    if(n<0)
    {

    }
    char *str = NULL;

    str  = (char*)malloc(sizeof(char)*(n+1));
    

    for(int i=0;i<n;++i)
    {
        cin>>str[i];
    }
    str[n] ='\0';
    //helper(str);
    helper1(str);
    printf("%s\n",str);
}