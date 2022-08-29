#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
int main(){
    std::string file = "test.txt";
    std::vector<std::string> vec;
    std::ifstream in(file);
    std::ofstream out;
    out.open("out.txt",std::ofstream::app); //会新建文件
    std::string s;

    while(std::getline(std::cin,s)){
        //分割
        char *start=&s[0],*end=&s[0];
        while(*end != '\0'){
            if(*end == '.'){
                *end ='\0';
                end++; //end+1
                vec.push_back(start);
                start = end;
                continue;
            }
            end++;
        }
        vec.push_back(start);
        s.clear();
    }

    std::cout<< vec.size() << std::endl;
    for(auto &v :vec){
        //std::istringstream ss(v);
        
        std::cout<< v << std::endl;
        // out<<v;
        // out<<'\n';
    }


    // std::stringstream ss;
    // std::string tmp;
    
    // std::getline(ss,tmp,'.');

    

}