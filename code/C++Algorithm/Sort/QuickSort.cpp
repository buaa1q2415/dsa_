#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <utility>

using namespace std;

template <typename Elemtype>
int risecmp(const Elemtype &a, const Elemtype &b) {
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

template <typename Elemtype>
int descmp(const Elemtype &a, const Elemtype &b) {
    if (a > b) return -1;
    if (a < b) return 1;
    return 0;
}

template <typename Elemtype>
class QuickSort{
private:
    int TwoWayPartition(vector<Elemtype>& arr,int left,int right,Elemtype x,int (*cmp)(const Elemtype&,const Elemtype&)){
        int i=left,xi=left-1,lbound=left;
        while(i<=right){
            if(cmp(arr[i],x)<=0){
                if(i!=lbound){
                    swap(arr[i],arr[lbound]);
                    if(cmp(arr[lbound],x)==0) xi=lbound;
                    if(cmp(arr[i],x)==0) xi=i;
                }
                lbound++,i++;
            }else{
                i++;
            }
        }
        swap(arr[xi],arr[lbound]);
        return lbound-1;
    }

    pair<int,int> ThreeWayPartition(vector<Elemtype>& arr,int left,int right,Elemtype x,int (*cmp)(const Elemtype&,const Elemtype&)){
        int i=left,lbound=left,rbound=right;
        while(i<=rbound){
            if(cmp(arr[i],x)<0){
                if(i!=lbound) swap(arr[i],arr[lbound]);
                lbound++,i++;
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
    void RandQuickSort(vector<Elemtype>& arr,int left,int right,int (*cmp)(const Elemtype&,const Elemtype&)=risecmp){//二路分区
        if(left>=right) return;
        // int value=arr[left + rand() % (right - left + 1)];  //随机选择
        int mid=left+(right-left)/2;
        if(cmp(arr[left],arr[mid])>0) swap(arr[left],arr[mid]);
        if(cmp(arr[left],arr[right])>0) swap(arr[left],arr[right]);
        if(cmp(arr[mid],arr[right])>0) swap(arr[mid],arr[right]);
        int value=arr[mid];  //三数取中
        mid=TwoWayPartition(arr,left,right,value,cmp);
        RandQuickSort(arr,left,mid,cmp);
        RandQuickSort(arr,mid+1,right,cmp);
    }

    void BetterRandQuickSort(vector<Elemtype>& arr,int left,int right,int (*cmp)(const Elemtype&,const Elemtype&)=risecmp){//三路分区
        if(left>=right) return;
        // int value = arr[left + rand() % (right - left + 1)];  //随机选择
        int mid=left+(right-left)/2;
        if(cmp(arr[left],arr[mid])>0) swap(arr[left],arr[mid]);
        if(cmp(arr[left],arr[right])>0) swap(arr[left],arr[right]);
        if(cmp(arr[mid],arr[right])>0) swap(arr[mid],arr[right]);
        int value=arr[mid];  //三数取中
        pair<int,int> p=ThreeWayPartition(arr,left,right,value,cmp);
        BetterRandQuickSort(arr,left,p.first-1,cmp);
        BetterRandQuickSort(arr,p.second+1,right,cmp);
    }
};