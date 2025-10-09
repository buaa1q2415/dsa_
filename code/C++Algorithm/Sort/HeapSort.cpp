#include <iostream>
#include <vector>

using namespace std;

template <typename Elemtype>
void swap(Elemtype &a, Elemtype &b){
    Elemtype temp=a;
    a=b;
    b=temp;
}

template <typename Elemtype>
int maxcmp(const Elemtype &a,const Elemtype &b){//升序
	return a>b;
}

template <typename Elemtype>
int mincmp(const Elemtype &a,const Elemtype &b){//降序
	return a<b;
}

template <typename Elemtype>
void Heapify(vector<Elemtype> &arr,int len,int index,int (*cmp)(const Elemtype &,const Elemtype &)){
	int flag=index,l=2*index+1,r=2*index+2;
	if(l<len&&cmp(arr[l],arr[flag])) flag=l;
	if(r<len&&cmp(arr[r],arr[flag])) flag=r;
	if(flag!=index){
		swap(arr[index],arr[flag]);
		Heapify(arr,len,flag,cmp);
	}
}

template <typename Elemtype>
void BuildHeap(vector<Elemtype> &arr,int len,int (*cmp)(const Elemtype &,const Elemtype &)){
	for(int i=len/2-1;i>=0;i--){
		Heapify(arr,len,i,cmp);
	}
}

template <typename Elemtype>
void HeapInsert(vector<Elemtype> &arr,int &len,const Elemtype &value,int (*cmp)(const Elemtype &,const Elemtype &)){
	arr[len]=value;
	int i=len;
	while(i>0 && cmp(arr[i],arr[(i-1)/2])){
		swap(arr[i],arr[(i-1)/2]);
		i=(i-1)/2;
	}
	len++;
}

template <typename Elemtype>
Elemtype HeapPop(vector<Elemtype> &arr,int &len,int (*cmp)(const Elemtype &,const Elemtype &)){
	Elemtype pop=arr[0];
	swap(arr[0],arr[len--]);
	Heapify(arr,len,0,cmp);
	return pop;
}

template <typename Elemtype>
void HeapSort(vector<Elemtype> &arr,int len,int (*cmp)(const Elemtype &,const Elemtype &)){
	BuildHeap(arr,len,cmp);
	for(int i=len-1;i>0;i--){
		swap(arr[0],arr[i]);
		Heapify(arr,i,0,cmp);
	}
}