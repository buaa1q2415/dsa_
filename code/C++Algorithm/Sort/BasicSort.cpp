#include <iostream>
#include <vector>
using namespace std;

//选择排序：每次从[i,right]中选择一个最小的元素，放到i位置，然后在[i+1,right]继续
//时间复杂度：O(n^2)
//空间复杂度：O(1)
//稳定性：不稳定
template<typename Elemtype>
void SelectSort(vector<Elemtype>& arr,int left,int right,int(* cmp)(Elemtype,Elemtype)){
    for(int i=left;i<=right;i++){
        int edge=i;
        for(int j=i+1;j<=right;j++){
            if(cmp(arr[j],arr[edge])) edge=j;
        }
        if(edge!=i) swap(arr[i],arr[edge]);
    }
}

//插入排序：[left,i]已经有序，将i+1插入向下滑动到有序位置，然后继续
//时间复杂度：O(n^2)
//空间复杂度：O(1)
//稳定性：稳定
template <typename Elemtype>
void InsertSort(vector<Elemtype>& arr,int left,int right,int(* cmp)(Elemtype,Elemtype)){
    for(int i=left+1;i<=right;i++){
        int j=i-1;
        Elemtype toinsert=arr[i];
        while(j>=left&&cmp(toinsert,arr[j])){
            arr[j+1]=arr[j];
            j--;
        }
        arr[j+1]=toinsert;
    }
}

//冒泡排序：每次从[left,right-i]中相邻位置较大的元素冒上去，放到right-i位置，然后在[left,right-i-1]继续
//时间复杂度：O(n^2)
//空间复杂度：O(1)
//稳定性：稳定
template <typename Elemtype>
void BubbleSort(vector<Elemtype>& arr,int left,int right,int(* cmp)(Elemtype,Elemtype)){
    for(int i=0;i<right-left;i++){
        int flag=0;
        for(int j=left;j<=right-i-1;j++){
            if(cmp(arr[j+1],arr[j])){
                swap(arr[j],arr[j+1]);
                flag=1;
            }
        }
        if(!flag) break;
    }
}
