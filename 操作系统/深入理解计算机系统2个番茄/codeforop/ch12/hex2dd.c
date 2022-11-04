#include<arpa/inet.h>
#include "../csapp.h"
int main(int argc, char **argv){
    char* s = argv[1];
    unsigned int num = 0;

    //转10进制 因为固定9位所以可以这么做
    //如果不固定 需要先把指针移动到最后利用指针也可以 或者循环求长度
    for(int i=9;s[i]!='x';--i){
        if(s[i]<'0') return 1;
        if(s[i]>'9'){
            unsigned int tmp = s[i] - 'a' + 10;
            num += tmp * pow(16,(9-i));
        }
        else{
            unsigned int tmp = s[i] - '0';
            num += tmp * pow(16,(9-i));
        }
        
    }
    
    //printf("ip is %u\n",ip);
    struct in_addr add;
    add.s_addr = ntohl(num);
    char *res = inet_ntoa(add);
    printf("a is %s\n",res);
    return 0;
}