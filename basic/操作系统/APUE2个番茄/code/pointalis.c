#include<stdio.h>
#include<apue.h>
int foo(int *a,int *b){
    *a = 5;
    *b = 6;
    return *a+*b;
}
int main(){
    int i = 0;
    int *a = &i;
    int *b = &i;
    printf("%d",foo(a,b));

}