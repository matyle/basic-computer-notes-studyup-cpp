    #include<vector>
    #include<numeric>
    #include<iostream>
    using namespace std;
    int paper(vector<int>& children) {
        int n = children.size();

        vector<int> vec(n,1);

        for(int i=0;i<n-1;i++)
        {
            if(children[i]<children[i+1])
            {
                vec[i+1] = vec[i] + 1; //
            }
        }
        for(int i=n-1;i>0;--i)
        {
            if(children[i-1]>children[i])
            {
                vec[i-1] = max(vec[i-1],vec[i]+1); 
            }
        }

        if(children[0]!=children[n-1])
        {
            
        }

        return accumulate(vec.begin(),vec.end(),0);
    }

    int main()
    {
        vector<int> children = {4,1,5};
        cout<<paper(children)<<endl;
    }