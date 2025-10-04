#include <bits/stdc++.h>

using namespace std;

#define SatisfactionStatus true

class SlidingWindow{
public:
    auto StatisticSlidingWindow(int target,int k, vector<int>& arr) {
        int left=0,right=0;
        while(right<arr.size()){
            if(right-left+1<=k){
                // 根据情况收集答案
                right++;
            }else{
                // 根据情况收集答案
                left++;
            }
        }
    }

    auto DynamicsSlidingWindow(int target, vector<int>& arr) {
        for(int left=0,right=0;right<arr.size();right++){
            while(SatisfactionStatus){// 满足状态
                // 根据情况收集答案
                left++;
            }
            // 不满足状态，根据情况收集答案
        }
    }
};