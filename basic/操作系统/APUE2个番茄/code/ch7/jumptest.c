#include<apue.h>
#include<setjmp.h>
jmp_buf jumbuffer;
void f1(int i);
void f2();
void f3();
int main(){
    int i = 3;
    if(setjmp(jumbuffer)==1){
        printf("jump after:\n");
        printf("i:%d\n",i);
        
    }
    i = 4;
    f1(i);
}
void f1(int i){
    
    printf("f1() exec\n");
    printf("in f1:\n");
    printf("i:%d\n",i);
    f2();
}
void f2(){
    
    //printf("f2() exec\n");
    f3();
}
void f3(){
    int i;
    printf("f3() exec\n");
    scanf("%d",&i);
    if(i<0){
        longjmp(jumbuffer,1);
    }
}