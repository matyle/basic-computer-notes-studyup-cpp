#include<string>
#include<iostream>
#include<stdio.h>
#include<climits>
// 变长参数宏
#define LOG(...) {\
        fprintf(stderr,"%s: Line %d:\t",__FILE__,__LINE__);\
        fprintf(stderr,__VA_ARGS__);\
        fprintf(stderr,"\n");\
}

#ifdef __cplusplus

#endif

using namespace std;



const char* hello()
{
    return __func__; //__func__宏

}

class A
{
public:
    A():name(__func__){}
    ~A(){}
    const char* name;

};

int main()
{
    cout<<"Standard Clib:"<<__STDC_HOSTED__<<endl;
    cout<<"Standard C:"<<__STDC__<<endl;
    //cout<<"IOS"<<__STDC_ISO_10646__<<endl; //macos无
    A a;
    cout<<a.name<<endl;
    cout<<hello()<<endl;
    int x = 3;
    //LOG("x=%d\n",x); //2-1-1.cpp: Line 39:     x=3 调试信息
    printf("min of long long: %lld\n",LLONG_MIN);
    printf("max of long long: %lld\n",LLONG_MAX);
    printf("max of unsigned long long: %llu\n",ULLONG_MAX);



}



