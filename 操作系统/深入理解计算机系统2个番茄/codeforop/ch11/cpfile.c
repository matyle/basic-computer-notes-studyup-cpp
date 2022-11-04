#include "../csapp.h"
int main(){
    int n;
    rio_t rio;
    char buf[MAXLINE];

    Rio_readinitb(&rio,STDIN_FILENO);

    while((n=Rio_readlineb(&rio,buf,MAXLINE))!=0){
        Rio_writen(STDOUT_FILENO,buf,n);
    }

}