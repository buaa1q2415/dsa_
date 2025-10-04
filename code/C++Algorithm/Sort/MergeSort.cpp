#include <iostream>
#include <vector>

using namespace std;

template <typename Elemtype>
int risecmp(const Elemtype &a,const Elemtype &b){
    return a<=b;
}

template <typename Elemtype>
int descmp(const Elemtype &a,const Elemtype &b){
    return a>=b;
}

template <typename Elemtype>
void Merge(vector<Elemtype> &arr,int left,int mid,int right,int (*cmp)(const Elemtype &,const Elemtype &),vector<Elemtype> &temp){
    int i=left,j=mid+1,k=0;
    while(i<=mid&&j<=right){
        if(cmp(arr[i],arr[j])) temp[k++]=arr[i++];
        else temp[k++]=arr[j++];
    }
    while(i<=mid) temp[k++]=arr[i++];
    while(j<=right) temp[k++]=arr[j++];
    for(int i=0;i<k;i++) arr[left+i]=temp[i];
}

template <typename Elemtype>
void MergeSortCore(vector<Elemtype> &arr,int left,int right,int (*cmp)(const Elemtype &,const Elemtype &),vector<Elemtype> &temp){
    if(left<right){
        int mid=left+(right-left)/2;
        MergeSortCore(arr,left,mid,cmp,temp);
        MergeSortCore(arr,mid+1,right,cmp,temp);
        Merge(arr,left,mid,right,cmp,temp);
    }
}

template <typename Elemtype>
void MergeSort(vector<Elemtype> &arr,int left,int right,int (*cmp)(const Elemtype &,const Elemtype &)){
    vector<Elemtype> temp(right-left+1);
    MergeSortCore(arr,left,right,cmp,temp);
}