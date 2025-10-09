/*分而治之是一种将一个复杂的问题分解为多个较小的子问题的算法
 * 每个子问题都可以独立解决，最后将子问题的解合并起来得到原问题的解
 * 能使用分治法解决的问题一般具有特征：问题的解=左子问题的解+右子问题的解+跨越子问题的解
 * 典型的分治算法有归并排序、快速排序等
 */

#include <iostream>
#include <vector>

using namespace std;

// 归并排序见C++Algorithm/Sort/MergeSort.cpp

// 统计逆序数
class CountReverse {
    vector<int> temp;
    int MergeRev(vector<int>& nums,int left,int mid,int right) {
        int i=left,j=mid+1,k=0;
        int count=0;
        while(i<=mid&&j<=right) {
            if(nums[i]<=nums[j]) {
                temp[k++]=nums[i++];
            } else {
                count+=mid-i+1;
                temp[k++]=nums[j++];
            }
        }
        while(i<=mid) temp[k++]=nums[i++];
        while(j<=right) temp[k++]=nums[j++];
        for(int p=0;p<k;p++) nums[left+p]=temp[p];
        return count;
    }

    void CountCore(vector<int>& nums,int left,int right,int& count) {
        if(left<right){
            int mid=left+(right-left)/2;
            CountCore(nums,left,mid,count);
            CountCore(nums,mid+1,right,count);
            count+=MergeRev(nums,left,mid,right);
        }
    }

public:
    int CountRev(vector<int>& nums,int left,int right) {
        temp.resize(nums.size());
        int count=0;
        CountCore(nums,left,right,count);
        temp.clear();
        return count;
    }
};

/* 数组小和问题:
 * 在一个数组中，每个数左边比当前数小的数累加起来，叫做这个数组的小和
 * 用分治法解决数组小和问题，就是计算每个数对数组小和的贡献，即它的右边有多少个数比它大
 * 然后将每个数对数组小和的贡献累加起来，就是数组小和
 */
class Solution {
    vector<int> temp;
    int MergeSmallSum(vector<int>& nums,int left,int mid,int right) {
        int i=left,j=mid+1,k=0;
        int count=0;
        while(i<=mid&&j<=right) {
            if(nums[i]<=nums[j]) {
                count+=nums[i]*(right-j+1);
                temp[k++]=nums[i++];
            } else {
                temp[k++]=nums[j++];
            }
        }
        while(i<=mid) temp[k++]=nums[i++];
        while(j<=right) temp[k++]=nums[j++];
        for(int p=0;p<k;p++) nums[left+p]=temp[p];
        return count;
    }
    void CountCore(vector<int>& nums,int left,int right,int& count) {
        if(left<right){
            int mid=left+(right-left)/2;
            CountCore(nums,left,mid,count);
            CountCore(nums,mid+1,right,count);
            count+=MergeSmallSum(nums,left,mid,right);
        }
    }
public:
    int CountSmallSum(vector<int>& nums,int left,int right) {
        temp.resize(nums.size());
        int count=0;
        CountCore(nums,left,right,count);
        temp.clear();
        return count;
    }
};
