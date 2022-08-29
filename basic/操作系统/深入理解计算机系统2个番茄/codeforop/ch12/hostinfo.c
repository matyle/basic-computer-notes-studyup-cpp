#include "../csapp.h"
int main(int argc,char** argv){
    char **pp; //string数组
    struct in_addr addr;
    struct hostent *hostp;// 指向主机结构的指针

    if(argc!=2){
        printf("you need ip or dominname\n");
        return 0;
    }

    if(inet_aton(argv[1],&addr)!=0){ //点分十进制转网络序ip地址
        hostp = gethostbyaddr(&addr,sizeof(addr),AF_INET);
    }
    else{
        hostp = gethostbyname(argv[1]);
    }

    printf("offical name:%s\n",hostp->h_name);
    //别名可能有多个 因此是二维数组
    for(pp=hostp->h_aliases;*pp!=NULL;pp++)
        printf("alias:%s\n",*pp);
    //ip地址结构可能有多个
    for(pp=hostp->h_addr_list;*pp!=NULL;pp++){
        addr.s_addr = *((unsigned int*) *pp);// pp是个string数组
        printf("address:%s\n",inet_ntoa(addr));
    }
    exit(0);
}