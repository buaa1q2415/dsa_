#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <utility>

using namespace std;

template <typename Elemtype>
class QuickSort{
private:
    int GetRandInt(int minval,int maxval){
        static mt19937 rng(chrono::system_clock::now().time_since_epoch().count());
        uniform_int_distribution<int> dist(minval,maxval);
        return dist(rng);
    }

    int TwoWayPartition(vector<Elemtype>& arr,int left,int right,Elemtype x,int (*cmp)(const Elemtype&,const Elemtype&)){
        int i=left,xi=left-1,lowerbound=left;
        while(i<=right){
            if(cmp(arr[i],x)<=0){
                if(i!=lowerbound) swap(arr[i],arr[lowerbound]);
                if(arr[lowerbound]==x) xi=lowerbound;
                lowerbound++,i++;
            }else{
                i++;
            }
        }
        swap(arr[xi],arr[lowerbound]);
        return lowerbound-1;
    }

    pair<int,int> ThreeWayPartition(vector<Elemtype>& arr,int left,int right,Elemtype x,int (*cmp)(const Elemtype&,const Elemtype&)){
        int i=left,lbound=left,rbound=right;
        while(i<=rbound){
            if(cmp(arr[i],x)<0){
                if(i!=lbound) swap(arr[i],arr[lbound]);
                lowerbound++,i++;
            }else if(cmp(arr[i],x)==0){
                i++;
            }else{
                if(i!=rbound){
                    swap(arr[i],arr[rbound]);
                    rbound--;
                } 
            }
        }
        pair<int,int> range(lbound,rbound);
        return range;
    }

public:
    int risecmp(const Elemtype &a, const Elemtype &b) {
        if (a < b) return -1;
        if (a > b) return 1;
        return 0;
    }

    int descmp(const Elemtype &a, const Elemtype &b) {
        if (a > b) return -1;
        if (a < b) return 1;
        return 0;
    }

    void RandQuickSort(vector<Elemtype>& arr,int left,int right,int (*cmp)(const Elemtype&,const Elemtype&)){//二路分区
        if(left>=right) return;
        int randvalue=arr[GetRandInt(left,right)];
        int mid=TwoWayPartition(arr,left,right,randvalue,cmp);
        RandQuickSort(arr,left,mid,cmp);
        RandQuickSort(arr,mid+1,right,cmp);
    }

    void BetterRandQuickSort(vector<Elemtype>& arr,int left,int right,int (*cmp)(const Elemtype&,const Elemtype&)){//三路分区
        if(left>=right) return;
        int randvalue=arr[GetRandInt(left,right)];
        pair<int,int> p=ThreeWayPartition(arr,left,right,randvalue,cmp);
        RandQuickSort(arr,left,p.first-1,cmp);
        RandQuickSort(arr,p.second+1,right,cmp);
    }
};