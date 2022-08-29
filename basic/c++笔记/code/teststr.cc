#include<vector>
#include<iostream>
int main(){
    std::vector<char> charray="abc";
    for(char c:charray){
        std::cout<<c;
    }
    return 0;
}