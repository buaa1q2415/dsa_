#include <bits/stdc++.h>
using namespace std;

//LeetCode198 打家劫舍

/**
当前操作：偷不偷第i家
子问题：偷过前i家获得的最大收益
下一个子问题：偷过前i-1家、前i-2家获得的最大收益

如果偷第i家，那么第i-1家不能偷
dp[i]=dp[i-2]+nums[i];
如果不偷第i家，那么第i-1家可以偷
dp[i]=dp[i-1];

综上，得到状态转移方程：dp[i]=max(dp[i-2]+nums[i],dp[i-1]);

考虑到dp[i]只与dp[i-1]、dp[i-2]有关，所以可以使用滚动数组优化空间复杂度

也可以考虑定义状态为dp[i]表示选择偷第i家获得的最大收益：dp[i]=max(dp[i-2],dp[i-3],...,dp[1])+nums[i]
 */

class Solution_198 {//滚动数组
public:
    int rob(vector<int>& nums) {
        if(nums.size()==1) return nums[0];
        vector<int> dp(2);
        dp[0]=nums[0],dp[1]=max(nums[0],nums[1]);
        for(int i=2;i<nums.size();i++){
            dp[i&1]=max(dp[(i-1)&1],dp[i&1]+nums[i]);
        }
        return dp[(nums.size()-1)&1];
    }
};