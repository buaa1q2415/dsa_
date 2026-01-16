#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <utility>
#include <algorithm>

template <typename T>
class qSort{
private:
    template <typename Compare>
    int TwoWayPartition(std::vector<T>& arr,int left,int right,T x,Compare cmp){
        int i=left,xi=left-1,lbound=left;
        while(i<=right){
            if(cmp(arr[i],x)<=0){
                if(i!=lbound){
                    std::swap(arr[i],arr[lbound]);
                    if(cmp(arr[lbound],x)==0) xi=lbound;
                    if(cmp(arr[i],x)==0) xi=i;
                }
                lbound++,i++;
            }else{
                i++;
            }
        }
        std::swap(arr[xi],arr[lbound]);
        return lbound-1;
    }

    template <typename Compare>
    std::pair<int,int> ThreeWayPartition(std::vector<T>& arr,int left,int right,T x,Compare cmp){
        int i=left,lbound=left,rbound=right;
        while(i<=rbound){
            if(cmp(arr[i],x)<0){
                if(i!=lbound) std::swap(arr[i],arr[lbound]);
                lbound++,i++;
            }else if(cmp(arr[i],x)==0){
                i++;
            }else{
                if(i!=rbound){
                    std::swap(arr[i],arr[rbound]);
                    rbound--;
                } 
            }
        }
        std::pair<int,int> range(lbound,rbound);
        return range;
    }

public:
    template <typename Compare>
    void QuickSort(std::vector<T>& arr,int left,int right,Compare cmp){//二路分区
        if(left>=right) return;
        // int value = arr[left + rand() % (right - left + 1)]; 
        int mid=left+(right-left)/2;
        if(cmp(arr[left],arr[mid])>0) std::swap(arr[left],arr[mid]);
        if(cmp(arr[left],arr[right])>0) std::swap(arr[left],arr[right]);
        if(cmp(arr[mid],arr[right])>0) std::swap(arr[mid],arr[right]);
        int value=arr[mid];  //三数取中
        mid=TwoWayPartition(arr,left,right,value,cmp);
        QuickSort(arr,left,mid,cmp);
        QuickSort(arr,mid+1,right,cmp);
    }

    template <typename Compare>
    void BetterQuickSort(std::vector<T>& arr,int left,int right,Compare cmp){//三路分区
        if(left>=right) return;
        // int value = arr[left + rand() % (right - left + 1)]; 
        int mid=left+(right-left)/2;
        if(cmp(arr[left],arr[mid])>0) std::swap(arr[left],arr[mid]);
        if(cmp(arr[left],arr[right])>0) std::swap(arr[left],arr[right]);
        if(cmp(arr[mid],arr[right])>0) std::swap(arr[mid],arr[right]);
        T value=arr[mid];  //三数取中
        std::pair<int,int> p=ThreeWayPartition(arr,left,right,value,cmp);
        BetterQuickSort(arr,left,p.first-1,cmp);
        BetterQuickSort(arr,p.second+1,right,cmp);
    }
};