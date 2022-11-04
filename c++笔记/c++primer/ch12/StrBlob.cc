#include<vector>
#include<memory>
#include<string>
typedef std::shared_ptr<std::vector<std::string>> VPtr;
typedef std::weak_ptr<std::vector<std::string>> VWPtr; //weak_ptr


class StrBlob
{
friend class StrBlobPtr; //友元类
typedef std::vector<std::string>::size_type size_type;
private:
    /* data */
    VPtr data;
    
public:
    StrBlob(/* args */);
    std::string &front() const;
    std::string &back() const;
    size_type size() const {return data->size();}
    bool empty(){return data->empty();}
    void push_back();
    void pop_back();
    ~StrBlob();
};
 

StrBlob::StrBlob(/* args */)
{
}

StrBlob::~StrBlob()
{
}

//
class StrBlobPtr{
public:
    StrBlobPtr():curr(0){}
    StrBlobPtr(StrBlob& a,std::size_t sz):data_wptr(a.data),curr(sz){}
    std::string &deinf() const;//解引用，要先检查
    StrBlobPtr& incr();//前缀递增
    ~StrBlobPtr(){}


private:
    VWPtr data_wptr;
    VPtr check(std::size_t, const std::string&) const;
    size_t curr;

};

VPtr StrBlobPtr::check(std::size_t i, const std::string& msg) const{

    auto ret = data_wptr.lock();//vector还存在？

    if(!ret){
        throw std::runtime_error("unbound StrBlobPtr");
    }
    if(i>ret->size()){
        throw std::out_of_range(msg);
    }
    return ret;
}
std::string& StrBlobPtr::deinf() const{
    auto p = check(curr, "dereference past end"); // check必须是const的
    return (*p)[curr];
}

StrBlobPtr& StrBlobPtr::incr(){
    check(curr,"past of"); // check必须是const的
    ++curr;
    return *this;
}