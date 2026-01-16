/*分而治之是一种将一个复杂的问题分解为多个较小的子问题的算法
 * 每个子问题都可以独立解决，最后将子问题的解合并起来得到原问题的解
 * 能使用分治法解决的问题一般具有特征：问题的解=左子问题的解+右子问题的解+跨越子问题的解
 * 典型的分治算法有归并排序、快速排序等
 */

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <utility>

using namespace std;

// 归并排序见C++Algorithm/Sort/BasicSort.cpp

// 快速排序见C++Algorithm/DivideConquer/QuickSort.cpp

// 统计逆序数
class CountReverse_ {
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
    int CountReverse(vector<int>& nums,int left,int right) {
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
class CountSmallSum_ {
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

/* 随机选择算法
 * 随机选择算法是指从一个数组中随机选择第k小的数
 * 随机选择算法的时间复杂度为O(n)，空间复杂度为O(1)
 * 随机选择算法的基本思想是：
 * 1. 随机选择一个数作为枢轴
 * 2. 将数组分为三部分：小于枢轴的数、等于枢轴的数、大于枢轴的数
 * 3. 如果第k小的数在小于枢轴的数中，递归在小于枢轴的数中选择第k小的数
 * 4. 如果第k小的数在等于枢轴的数中，直接返回枢轴
 * 5. 如果第k小的数在大于枢轴的数中，递归在大于枢轴的数中选择第k小的数
*/

class RandomSelect_{
private:
pair<int, int> ThreeWayPartition(vector<int>& arr, int left, int right, int pivot_value) {
        int lt = left,gt = right,i = left;
        while (i <= gt) {
            if (arr[i] < pivot_value) {
                swap(arr[i], arr[lt]);
                lt++,i++;
            } else if (arr[i] == pivot_value) {
                i++;
            } else {
                swap(arr[i], arr[gt]);
                gt--;
            }
        }
        return {lt, gt};
    }
public:
    int RandomSelect(vector<int>& arr, int left, int right, int k) {
        if (left >= right) {
            return arr[left];
        }
        int pivot_value = arr[left + rand() % (right - left + 1)];
        pair<int, int> p = ThreeWayPartition(arr, left, right, pivot_value);
        int k_idx = k+left-1;
        if(k_idx <= p.first - 1) {
            return RandomSelect(arr, left, p.first - 1, k);
        }else if (k_idx >= p.second + 1) {
            return RandomSelect(arr, p.second + 1, right, k_idx-p.second);
        }else {
            return pivot_value;
        }
    }
};
