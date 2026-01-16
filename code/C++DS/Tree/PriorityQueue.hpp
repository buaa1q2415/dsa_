#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP


#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstddef>
#include <stdexcept>

template <typename T,
        typename Container=std::vector<T>,
        typename Comparator = std::less<T>>
class PriorityQueue{
private:
    Container data;
    Comparator comp;
    void buildHeap(size_t size){
        for (long long i = static_cast<long long>(size/2)-1; i>= 0; --i) {
            Heapify(i);
        }
    }
public:
    PriorityQueue(size_t capacity = 16) : comp() {
        if constexpr (std::is_same_v<Container, std::vector<T>>) {
            data.reserve(capacity);
        }
    }

    PriorityQueue(std::vector<T> arr, size_t capacity = 0) : comp() {
        if constexpr (std::is_same_v<Container, std::vector<T>>) {
            data.reserve(std::max(arr.size(), capacity));
        }
        data = std::move(arr);
        buildHeap();
    }

    PriorityQueue(std::deque<T> arr, size_t capacity = 0) : comp() {
        if constexpr (std::is_same_v<Container, std::vector<T>>) {
            data.reserve(std::max(arr.size(), capacity));
        }
        data = std::move(arr);
        buildHeap();
    }
    
    PriorityQueue(std::initializer_list<T> arr, size_t capacity = 0) : comp() {
        if constexpr (std::is_same_v<Container, std::vector<T>>) {
            data.reserve(std::max(arr.size(), capacity));
        }
        data.assign(arr.begin(), arr.end());
        buildHeap();
    }

    ~PriorityQueue()=default;

    PriorityQueue(PriorityQueue&&) = default;
    PriorityQueue& operator=(PriorityQueue&&) = default;

    void reserve(size_t capacity) {
        if constexpr (std::is_same_v<Container, std::vector<T>>) {
            data.reserve(capacity);
        }
    }

    size_t getSize() const noexcept{return data.size();}

    bool isEmpty() const noexcept{return data.size()==0;}

    void Heapify(int idx){
        while(true){
            int flag=idx,l=2*idx+1,r=2*idx+2;
            if(l<data.size() && comp(data[l],data[flag])) flag=l;
            if(r<data.size() && comp(data[r],data[flag])) flag=r;
            if(flag!=idx){
                std::swap(data[flag],data[idx]);
                idx=flag;
            }else break;
        }
    }

    void push(const T& value){
        size_t i=data.size();
        data.push_back(value);
        while(i>0 && comp(data[i],data[(i-1)/2])){
            std::swap(data[i],data[(i-1)/2]);
            i=(i-1)/2;
        }
    }

    void push(T&& value){
        size_t i=data.size();
        data.push_back(std::move(value));
        while(i>0 && comp(data[i],data[(i-1)/2])){
            std::swap(data[i],data[(i-1)/2]);
            i=(i-1)/2;
        }
    }

    template<typename... Args>
    void emplace(Args&&... args){
        size_t i=data.size();
        data.emplace_back(std::forward<Args>(args)...);
        while(i>0 && comp(data[i],data[(i-1)/2])){
            std::swap(data[i],data[(i-1)/2]);
            i=(i-1)/2;
        }
    }

    void pop(){
        if(data.size()==0) throw std::underflow_error("PriorityQueue::pop(): PriorityQueue is empty!");

        std::swap(data.front(),data.back());
        data.pop_back();
        Heapify(0);
    }

    void clear() noexcept {data.clear();}

    const T& getTop() const {
        if(data.size()==0) throw std::underflow_error("PriorityQueue::getTop(): PriorityQueue is empty!");
        return data[0];
    }

    T& getTop(){
        if(data.size()==0) throw std::underflow_error("PriorityQueue::getTop(): PriorityQueue is empty!");
        return data[0];
    }

    const Container& view() const{return data;}

    friend std::ostream& operator<<(std::ostream& os,const PriorityQueue<T, Container, Comparator>& pq){
        if(pq.isEmpty()){
            os<<"[Empty PriorityQueue]";
            return os;
        }
        os<<"[TOP] ";
        for (const auto& it : pq.data) {
            os <<it<< " ";
        }
        os<<"[BACK]";
        return os;
    }

    friend void swap(PriorityQueue<T, Container,Comparator>& a, PriorityQueue<T, Container,Comparator>& b) noexcept {
        using std::swap;
        swap(a.data,b.data);
        swap(a.comp,b.comp);
    }

};

#endif
