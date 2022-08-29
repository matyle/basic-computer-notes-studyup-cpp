int fib(int n){
    int i = 0;
    int val = 0;
    int nval =1;
    do{
        int t = val+nval;
        val = nval;
        nval = t;
        ++i;
    }while(i<n);
    return val;
}
int main(){
    return fib(3);
}