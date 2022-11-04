#include<stdio.h>
int main(int argc, char* argv[]){
    int i = 0; //栈上申请
    int arr[3] = {0};
    for(; i<=3; i++){
        arr[i] = 0;
        printf("hello world\n");
    }
    return 0;
}