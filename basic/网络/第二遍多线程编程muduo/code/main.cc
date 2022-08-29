template<typename T>
void foo(const T& a);
template<typename T>
#include<stdio.h>
T bar(const T& a);
int main(){
    int i;
    foo(0);
    foo(1.0);
    bar('c');
     
    int i ;
    short s;
    float f;
    double d;
    char name[80];

    scanf("%d, %hd, %f,%lf,%s",&i,&s,&f,&d,name);
    printf("%d,%d %f %f %s",i,s,f,d,name);
}