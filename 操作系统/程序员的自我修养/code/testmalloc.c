#include<stdio.h>
#include<stdlib.h>
unsigned maxnum = 0;
int main(){
    unsigned blocksize[] ={1024*1024,1024,1};
    int i,count;
    for(i=0;i<3;i++){
        for(count=0;;count++){
            void *block = malloc(maxnum + blocksize[i] * count);
            if(block){
                maxnum = maxnum + blocksize[i] * count;
                free(block);
            }
            else{
                break;
            }
        }
    }
    printf("maximum malloc size: %u MB",maxnum/1000*1000);
    return 0;
}