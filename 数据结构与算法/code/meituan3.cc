#include <stack>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
int main()
{
    string str;
    int sum = 1;

    int s = 0;
    getline(cin, str);
    stack<char> st;
    int cnt = 0;
    int ind = 0;
    unordered_map<int, int> memo;
    for (int i = 0; i < str.size(); ++i)
    {
        char c = str[i];
        if (c == '('){
            st.push(c);
            // int ind = st.size()-1;
            // memo[ind] = 1;
        }
        else{
            ind = st.size();
            st.pop();
            //哈希表里面存在

            if (memo.size() < ind){
                memo[ind] += 1;
            }
            if (memo.count(ind))
            {
                memo[ind] = memo[ind] * memo[ind];
            }
            else
            {
                memo[ind] += 2;
            }
        }
        //结算第一个
        if (st.empty())
        {
            for (auto p : memo)
            {
                s += p.second;
            }
            sum = s * sum;
        }
    }
    cout << sum << endl;
}