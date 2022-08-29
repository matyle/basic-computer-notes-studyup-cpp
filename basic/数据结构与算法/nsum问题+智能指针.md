## 2数之和

1. 哈希解法，使用map存kv（值为key，下标为value）

   

2. 双指针解法（需要排序）





## 三数之和

1. 做这道题的时候，最开始返回了局部对象的引用，导致出错

2. 但是我还是不想返回对象

3. 因此使用了unique_ptr

   1. 注意，unique_ptr需要初始化

      `unique_ptr<vector<vector<int>>> res(new vector<vector<int>>()); `

难点：

- 双指针的应用
- 跳过重复元素
- 跳过第一个开始的重复元素（因为调用函数从1开始）

```CPP
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        // -4 -1 -1 0 1 2
        //确定一个 剩下的就是2数之和问题

        //nsum问题
        int n = nums.size();
        sort(nums.begin(),nums.end());
        vector<vector<int>>  res;
        
        for(int i=0;i<n;++i)
        {
            unique_ptr<vector<vector<int>>>
                tuples = move(twoSum(nums,i+1,0-nums[i])); //这里注意 
            
            for(auto tuple : *tuples)
            {
                tuple.push_back(nums[i]);
                res.push_back(tuple);
            }

            //跳过第一个数重复的情况 例如[1,1,1,2,3] 6
            while(i<n-1&&nums[i]==nums[i+1]) i++;
        
        }
        return res;

    }

    unique_ptr<vector<vector<int>>> twoSum(vector<int>& nums,int start,int target) //返回局部对象的引用！！！
    {
        int lo = start; //start最多取到
        int hi = nums.size()-1;
        unique_ptr<vector<vector<int>>> res(new vector<vector<int>>()); 
        //注意要去重
        while(lo<hi)  //[lo,hi)  两个指针不需要相等 相等就是一个数相加了
        {
            int sum = nums[lo] + nums[hi];
            int left = nums[lo],right = nums[hi];//记录当初两个值 方便去重
            if(sum<target)
            {
                while(lo<hi&&nums[lo]==left) lo++;//相同值跳过
            }
            else if(sum>target)
            {
                while(lo<hi&&nums[hi]==right) hi--;//相同值跳过
            }
            else if(sum == target)
            {
                res->push_back({left,right});
                while(lo<hi&&nums[lo]==left) lo++;
                while(lo<hi&&nums[hi]==right) hi--;
            }

        }
        return res;
    }
};
```





### nsum问题

- 发现用智能指针（移动语义）还要慢一些，递归
- 

```cpp
class Solution {
public:
    int n = 4;//四数之和
    //typedef unique_ptr<vector<vector<int>>>  VvPtr; //二维数组指针
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        sort(nums.begin(),nums.end());
        return nSumTarget(nums,0,4,target);
    }

vector<vector<int>> nSumTarget(vector<int>& nums,int start,int n,int target)
{
    vector<vector<int>> res;
    //VvPtr res(new vector<vector<int>>());

    int sz = nums.size();
    if(n<2||sz<n) return res;
    if(n==2)//两数之和 base case是两数之和
    {
        int lo = start; //start最多取到
        int hi = sz-1;
        while(lo<hi)  //[lo,hi)  两个指针不需要相等 相等就是一个数相加了
        {
            int sum = nums[lo] + nums[hi];
            int left = nums[lo],right = nums[hi];//记录当初两个值 方便去重
            if(sum<target)
            {
                while(lo<hi&&nums[lo]==left) lo++;//相同值跳过
            }
            else if(sum>target)
            {
                while(lo<hi&&nums[hi]==right) hi--;//相同值跳过
            }
            else if(sum == target)
            {
                res.push_back({left,right});
                while(lo<hi&&nums[lo]==left) lo++;
                while(lo<hi&&nums[hi]==right) hi--;
            }
        }

    }
    else
    {
    //注意要去重
        for(int i=start;i<sz;++i) //从start开始 类似于回溯
        {
            vector<vector<int>> vecs = nSumTarget(nums,i+1,n-1,target-nums[i]);
            for(auto &v : vecs)
            {
                v.push_back(nums[i]);
                res.push_back(v);
            }
            while(i<sz-1&&nums[i]==nums[i+1]) i++;
        }
        
    }
    return res;
}

};
```

