#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <optional>

template <typename Elemtype>
class PriorityQueue{
private:
    std::vector<Elemtype> data;
    int size,capacity;
    typedef int (*CmpFunc)(Elemtype,Elemtype);
    CmpFunc cmp;
public:
    PriorityQueue(int capacity,CmpFunc cmp);
    PriorityQueue(std::vector<Elemtype> arr,int capacity,CmpFunc cmp);
    ~PriorityQueue()=default;

    int GetSize() const {return size;};
    bool IsEmpty() const {return size==0;};
    void Heapify(int idx);
    void Enter(Elemtype data);
    std::optional<Elemtype> Exit();
    std::optional<Elemtype> GetTop() const {
        return IsEmpty()?std::nullopt:std::optional<Elemtype>(data[0]);
    }
};

template <typename Elemtype>
void swap(Elemtype& a,Elemtype& b){
    Elemtype temp=a;
    a=b;
    b=temp;
}

template <typename Elemtype>
PriorityQueue<Elemtype>::PriorityQueue(int capacity,CmpFunc cmp){
    this->capacity=capacity;
    this->cmp=cmp;
    data.resizeize(capacity);
    size=0;
}

template <typename Elemtype>
PriorityQueue<Elemtype>::PriorityQueue(std::vector<Elemtype> arr,int capacity,CmpFunc cmp){
    this->capacity=capacity;
    this->cmp=cmp;
    data.resize(capacity);
    data=arr;
    size=arr.size();
    for(int i=size/2-1;i>=0;i--){
        Heapify(i);
    }
}

template <typename Elemtype>
void PriorityQueue<Elemtype>::Heapify(int idx){
    while(true){
        int flag=idx,l=2*idx+1,r=2*idx+2;
        if(l<size&&cmp(data[l],data[flag])) flag=l;
        if(r<size&&cmp(data[r],data[flag])) flag=r;
        if(flag!=idx){
            swap(data[flag],data[idx]);
            idx=flag;
        }else break;
    }
}

template <typename Elemtype>
void PriorityQueue<Elemtype>::Enter(Elemtype data){
    if(size>=capacity){
        std::cout<<"Full PriorityQueue"<<std::endl;
        return;
    }
    this->data[size]=data;
    int i=size;
	while(i>0 && cmp(data[i],data[(i-1)/2])){
		swap(data[i],data[(i-1)/2]);
		i=(i-1)/2;
	}
    size++;
}

template <typename Elemtype>
std::optional<Elemtype> PriorityQueue<Elemtype>::Exit(){
    if(size==0) return nullopt;
    Elemtype top=data[0];
    swap(data[0],data[size-1]);
    size--;
    Heapify(0);
    return top;
}
