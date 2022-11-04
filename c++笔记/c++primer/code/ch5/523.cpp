#include<iostream>
#include<exception>
#include<stdexcept>
int main(){
    int a,b;
    while(std::cin>>a>>b){
        // if(b==0){
        //     throw std::runtime_error("b cannot be 0");
        // }

        try{
            if(b==0){
                throw std::runtime_error("b cannot be 0");
            }
            std::cout<<a/b<<std::endl;
            
            
            
        }catch(std::runtime_error(err)){
            std::cout<<"once again\n"<<std::endl;

        }
        
    }

    return 0; 
}