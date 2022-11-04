快排注意点

1. helper函数中传nums.size()-1
2. 判断条件：（
   1. k-1<p 说明k在p左边，因此递归查找左边 p是返回的
   2. k-1>p 说明k在p右边，因此递归查找左边

3. 加入random

   ```cpp
   int randIndex = rand(time) %( (r - l + 1) + l);
   swap(nums[randIndex],nums[right]);
   pivot = nums[right];
   int i = left,j=left;
   
   for(;j<right;++j)
   {
     
   }
   ```

   

