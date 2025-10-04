/**
 * 当待搜索空间（如数组、区间等）满足 “存在可判定的二分性规律” 时，即可通过二分查找高效缩小搜索范围，最终定位目标（或满足条件的结果）

 * “可判定的二分性规律” 需满足以下核心要求：对于任意划分搜索空间的中间位置（记为 mid），能通过一个明确的判定逻辑（如比较、趋势判断等），直接确定 “目标（或满足条件的结果）必然存在于左半空间，或必然存在于右半空间”—— 即每次划分后，可直接排除其中一半空间，仅对另一半空间继续搜索

 * 二分查找的本质是 “利用二分性规律实现搜索范围的高效裁剪”，有序性仅是满足该规律的常见场景之一，而非唯一场景。
 * 只要待搜索空间能通过中间位置判定并排除一半，即可应用二分查找

 * 宫水三叶：二分的本质是两段性，并非单调性。只要一段满足某个性质，另外一段不(一定)满足某个性质，就可以用二分
**/

#include <bits/stdc++.h>
using namespace std;


int BinarySearch(int target, vector<int>& arr) {
    int left=0,right=arr.size()-1;
    while(left<=right){
        int mid=left+(right-left)/2;
        if(arr[mid]==target){
            return mid;
        }else if(arr[mid]<target){
            left=mid+1;
        }else{
            right=mid-1;
        }
    }
    return -1;
}//闭区间二分查找


int LowerBound(vector<int>& nums, int target){
    int left=0,right=nums.size()-1;
    while(left<=right){
        int mid=left+(right-left)/2;
        if(nums[mid]<target){
            left=mid+1;
        }else{
            right=mid-1;
        }
    }
    return left;
}//返回第一个大于等于target的下标

int FindPeak(vector<int>& nums){
    if(nums.size()==1) return 0;
    if(nums[0]>nums[1]) return 0;
    if(nums[nums.size()-1]>nums[nums.size()-2]) return nums.size()-1;
    int left=1,right=nums.size()-2;
    while(left<right){
        int mid=left+(right-left)/2;
        if(nums[mid]<nums[mid+1]){
            left=mid+1;
        }else{
            right=mid;
        }
    }
    return left;
}//返回峰值下标，峰值元素大于其左右相邻元素