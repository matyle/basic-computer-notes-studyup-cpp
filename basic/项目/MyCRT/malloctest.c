//#include<stdio.h>
#include "malloc.c"
int main()
{
    int *v = (int*)malloc(3*sizeof(int));
    v[0] = 1;
    v[1] = 1;
    v[2] = 1;
    //printf("%d\n",v[0]);
}