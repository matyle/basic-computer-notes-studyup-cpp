//点分十进制转16
#include<arpa/inet.h>
#include "../csapp.h"
int main(int argc, char **argv){
    char* s = argv[1];
    unsigned int num = 0;
    char ip[11];
    memset(&ip,'\0',sizeof(ip));
    printf("ip is %s\n",s);
    struct in_addr add;
    inet_aton(s,&add);

    unsigned int res = ntohl(add.s_addr);
    int i = 9;
    while (res!=0)
    {
        unsigned int mod = res % 16;
        res = res / 16;
        if(mod>=0&&mod<=9){

            ip[i] = mod + '0';
            //printf("%c\n",ip[i]);
        }
        else{
            char temp = (mod-10) + 'a';
            ip[i] = temp;
        }
     
        --i;
    }
    ip[0] = '0';
    ip[1] = 'x';

    printf("a is %s\n",ip);
    return 0;
}