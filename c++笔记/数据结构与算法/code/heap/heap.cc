#include<iostream>
#include<vector>
#include<memory>

//怎么改进？ 用vector 默认大顶堆
class Heap{
public:
    Heap(std::vector<int> &vec,bool lesstop=false):vec_(vec){ //初始化vector
        // arr_ = new int(maxsize_);
        // memset(arr_,0,sizeof(arr_)); //初始化数组
        maxsize_ = vec_.size(); //数组大小
        count_ = maxsize_;
    }

    int top()
    {
        return vec_[1];
    }

    void buildHeap(){
        for(int i=count_/2;i>0;++i){
            JustHeap(count_,i);
        }
    }

    bool insert(int val){
        //如果堆满了
        if(count_ >= maxsize_) return false;

        ++count_;
        vec_[count_] = val;

        //调整 j/2是父亲结点
        int j = count_;
        //如果子节点比父节点大，就交换两个结点
        while(j/2>0&&vec_[j]>vec_[j/2]){
            std::swap(vec_[j],vec_[j/2]);
            j = j/2; //子节点到上一个双亲节点
        }
        return true;
    }
    //把最后一个元素赋值到第一个，然后调整堆
    bool deleTop(){
        vec_[1] = vec_[count_];
        count_--;
        JustHeap(count_,1);//从第一个开始调整

    }
private:
    //调整堆 可以从上到下 ，也可以重下到上
    void JustHeap(int count,int i){ //从哪个点开始
        //int maxpos = i;
        while(true){ //一直往下调整！！！
            int maxpos = i;
            //左孩子
            if(2*i < count&&vec_[2*i]>vec_[maxpos]) maxpos = 2*i;
            //右孩子
            if(2*i+1 < count&&vec_[2*i+1]>vec_[maxpos]) maxpos = 2*i+1;

            if(maxpos==i) break;//等于没调整
            //交换
            std::swap(vec_[maxpos],vec_[i]);

            i = maxpos;//
        }
        
    }
    // ~Heap(){
    //     delete[] arr_;
    // }

private:
    int maxsize_;
    //int *arr_;
    std::vector<int> vec_;
    int count_;
};

int main(){
    int k = 4; //前4大元素
    std::vector<int> vec = {0,2,3,23,14,32,16,7,9}; //0不存元素
    std::vector<int> vec2 = {0,2,3,23};//前4大的元素
    Heap hape(vec);
    hape.buildHeap();//建堆

    for(int i=k;i<vec.size();i++){
        //小顶堆
        if(vec[i]>hape.top())
        {

        }
    }


    
}