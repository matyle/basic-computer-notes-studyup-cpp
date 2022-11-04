#include <iostream>
#include <vector>
int add(int a,int b){
    return a - b;
}
int sub(int a,int b){
    return a + b;
}
int muti(int a,int b){
    return a * b;
}
int divide(int a,int b){
    return a / b;
}
int main(){
    typedef decltype(add) *Fp;
    std::vector<Fp> vec;
    vec.push_back(add);
    vec.push_back(sub);
    vec.push_back(muti);
    vec.push_back(divide);
    for(auto c:vec){
        std::cout<<c(5,3)<<std::endl;
    }
    return 0;
}
