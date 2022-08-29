#include<vector>
using namespace std;
template<class T>
class Binary{
public:
    Binary(vector<T> vec,T key):vec_(vec),key_(key){}
    int normalBinSerch(const vector<T>& v,T k);
    int BSfirstkey(const vector<T>& v,T k);
private:
    vector<T> vec_;
    T key_;

};

template<class T>
int  Binary<T>::normalBinSerch(const vector<T>& v,T k){
    int low = 0,high = v.size()-1,mid=0;

    while(low<=high){
        mid = low + ((high - low) >> 1);
        if(v[mid] == k) return mid; //找到

        else if(v[mid] < k) low = mid + 1;//往后找
        else high = mid - 1;
    }
    return high;
}

int  Binary<T>::BSfirstkey(const vector<T>& v,T k){
    int low = 0,high = v.size()-1,mid=0;

    while(low<=high){
        mid = low + ((high - low) >> 1);
        if(v[mid] == k){
            if(mid==0||v[mid-1]!=k) return mid;
            else high = mid - 1;//往前找
        }

        else if(v[mid] < k) low = mid + 1;//往后找
        else high = mid - 1;
    }
    return high;
}