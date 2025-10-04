#include <bits/stdc++.h>
using namespace std;

//LeetCode1143 最长公共子序列

/**思路：
当前操作：选不选text1的第i个字符和text2的第j个字符
子问题：text1的前i个字符和text2的前j个字符的LCS长度
下一个子问题：text1的前i-1个字符和text2的前j-1个字符、text1的前i个字符和text2的前j-1个字符、text1的前i-1个字符和text2的前j个字符的LCS长度

考虑text1[i]==text2[j]时：
    如果dp[i][j-1]=dp[i-1][j-1]+1(即text1[i]=text2[j-1]) 或 dp[i-1][j]=dp[i-1][j-1]+1(即text1[i-1]=text2[j])，则dp[i][j]=dp[i-1][j-1]+1
    如果dp[i][j-1]=dp[i-1][j]=dp[i-1][j-1]，则dp[i][j]=dp[i-1][j-1]+1
此时dp[i][j]=dp[i-1][j-1]+1
如果text1[i]!=text2[j]，dp[i][j]=max(dp[i][j-1],dp[i-1][j]，dp[i-1][j-1])，但又考虑到dp[i][j-1]>dp[i-1][j-1],dp[i-1][j]>dp[i-1][j-1]
此时dp[i][j]=max(dp[i][j-1],dp[i-1][j])

综上，得到状态转移方程：
    dp[i][j]=dp[i-1][j-1]+1  if text1[i]==text2[j]
    dp[i][j]=max(dp[i][j-1],dp[i-1][j])  if text1[i]!=text2[j]

考虑到只用到dp[i-1][j-1]，dp[i-1][j]，dp[i][j-1]，所以可以用滚动数组优化空间复杂度(方法2)
*/

class Solution_1143 {
public:
    int longestCommonSubsequence1(string text1, string text2) {//标准dp
        int row=text1.size(),col=text2.size();
        vector<vector<int>> dp(row+1,vector<int>(col+1,0));
        for(int i=1;i<=row;i++){
            for(int j=1;j<=col;j++){
                if(text1[i-1]==text2[j-1]) dp[i][j]=dp[i-1][j-1]+1;
                else dp[i][j]=max(dp[i][j-1],dp[i-1][j]);
            }
        }
        return dp[row][col];
    }


    int longestCommonSubsequence2(string text1, string text2) {//dp+滚动数组
        int row=text1.size(),col=text2.size();
        vector<vector<int>> dp(2,vector<int>(col+1,0));
        for(int i=1;i<=row;i++){
            for(int j=1;j<=col;j++){
                if(text1[i-1]==text2[j-1]) dp[i&1][j]=dp[(i-1)&1][j-1]+1;
                else dp[i&1][j]=max(dp[i&1][j-1],dp[(i-1)&1][j]);
            }
        }
        return dp[row&1][col];
    }//空间可以进一步优化为O(min(row,col))
};

//LeetCode72 编辑距离
class Solution_72 {
public:
    int minDistance(string word1, string word2) {//已优化空间版，注意初始化
        int row=word1.size(),col=word2.size();
        vector<vector<int>> dp(2,vector<int>(col+1,0));
        for(int j=1;j<=col;j++){
            dp[0][j]=j;
        }
        for(int i=1;i<=row;i++){
            dp[i&1][0]=i;
            for(int j=1;j<=col;j++){
                if(word1[i-1]==word2[j-1]) dp[i&1][j]=dp[(i-1)&1][j-1];
                else dp[i&1][j]=min(min(dp[i&1][j-1],dp[(i-1)&1][j]),dp[(i-1)&1][j-1])+1;
            }
        }
        return dp[row&1][col];
    }
};
