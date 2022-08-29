#include<string>
#include<iostream>
int main(){
    std::string str = "ab2c3d7Rc34E6";

    size_t pos = str.find_first_of("c3d7",0);
    size_t pos1 = str.rfind("c3");
    std::cout<< "find_first_of(""2c"",0) pos:"<<pos<<std::endl;
    std::cout<< "rfind(""2c"",0) pos2:"<<pos1<<std::endl;


}