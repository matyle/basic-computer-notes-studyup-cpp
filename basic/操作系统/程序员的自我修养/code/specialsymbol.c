#include<stdio.h>

extern char __executable_start[];
extern char etext[], _etext[];
extern char edata[], _edata[];
extern char end[];

int main(){
    printf("executable start %X\n",__executable_start);
    printf("Text end %X\n",_etext);
    printf("Data end %X\n",_edata);
    printf("executable end %X\n",end);

    return 0;
}