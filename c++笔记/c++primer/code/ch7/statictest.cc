
class StaticTest{
public:
    static int var_st; //声明静态成员
    static int foo();
    int pvar;
private:
    int var;

};
int StaticTest::var_st = 1;
int StaticTest::foo(){
    int cnt = pvar; //错误 没有this指针不能使用类成员
    int cnt2 = var_st; //正确

}

int main(){

    StaticTest::foo();//直接调用 正确
    StaticTest st;
    st.foo();// 对象调用 正确
}