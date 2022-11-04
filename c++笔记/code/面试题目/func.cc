#include<pthread.h>
#include<unistd.h>
#include<time.h>
#include<iostream>
using namespace std;
int cnt = 0;
// void* func(void* arg)
// {
    
// }
time_t now = time(NULL);
void* fun(void *arg)
{
    cnt++;
    if(time(NULL)-now==1)
    {
        if(cnt==1000) cout<<"true"<<endl;
        else
        {
            cnt=0;
            cout<<"false: "<<now<<endl;
            now = time(NULL);
            
        }
    }
}
int main()
{
    pthread_t tid;
    for(int i=0;i<5000;++i)
    {
        pthread_create(&tid,NULL,func,NULL);
    }
    // while(1)
    // {
        
    //     fun();
       
    // }
    return 0;

}