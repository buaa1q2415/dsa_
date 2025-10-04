#include <bits/stdc++.h>
using namespace std;

//LeetCode 26 删除有序数组中的重复项
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int expect=1,current=1;//expect指向下一个唯一元素期望放置的位置，current指向当前操作位置
        //从下标1开始遍历，因为第0个元素总是唯一的
        while(current<nums.size()){
            if(nums[current]!=nums[current-1]){
                nums[expect]=nums[current];
                expect++;
            }
            current++;
        }
        return expect;
    }
};