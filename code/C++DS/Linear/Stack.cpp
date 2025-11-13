#include <iostream>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <stdexcept>

template <typename T>
class Stack{
private:
    std::vector<T> data;
public:
    Stack(size_t capacity=32){data.reserve(capacity);}
    ~Stack()=default;
    void Clear(){data.clear();};
    void Reserve(size_t new_capacity) {data.reserve(new_capacity);}

    bool isEmpty() const{return data.empty();};
    const T& getTop() const{
        if(isEmpty()) throw std::underflow_error("Stack::getTop(): Stack is empty!");
        return data.back();
    }
    T& getTop(){
        if (isEmpty()) throw std::underflow_error("Stack::Top(): Stack is empty!");
        return data.back();
    }
    size_t getSize() const{return data.size();};
    size_t getCapacity() const { return data.capacity(); }

    void Show(bool reverse=false) const;
    void push(const T& value){data.push_back(value);}
    void push(T&& value) {data.push_back(std::move(value));}
    template<typename... Args>
    void emplace(Args&&... args) {data.emplace_back(std::forward<Args>(args)...);}
    void pop(){
        if(isEmpty()) throw std::underflow_error("Stack::Pop() : Stack is EMPTY!");
        data.pop_back();
    }

    bool operator==(const Stack<T>& other) const{return data==other.data;};
    bool operator!=(const Stack<T>& other) const{return data!=other.data;};

friend void swap(Stack<T>& a, Stack<T>& b) noexcept;

friend std::ostream& operator<<(std::ostream& os,const Stack<T>& s);

};

template <typename T>
void Stack<T>::Show(bool reverse) const{
    if(s.isEmpty()){
        std::cout<<"[Empty Stack]"<<std::endl;
        return;
    }
    if(!reverse){
        std::cout<<"[TOP] ";
        for (auto it=s.data.rbegin();it!=s.data.rend();++it) {
            std::cout<<*it<<" ";
        }
        std::cout<<"[BOTTOM]";
    }else{
        std::cout<<"[BOTTOM] ";
        for (auto it=s.data.begin();it!=s.data.end();++it) {
            std::cout<<*it<<" ";
        }
        std::cout<<"[TOP]";
    }
    std::cout<<std::endl;
}

template <typename T>
std::ostream& operator<<(std::ostream& os,const Stack<T>& s){
    if(s.isEmpty()){
        os<<"[Empty Stack]";
    }else{
        os<<"[TOP] ";
        for (auto it=s.data.rbegin();it!=s.data.rend();++it) {
            os<<*it<<" ";
        }
        os<<"[BOTTOM]";
    }
    return os;
}

template<typename T>
void swap(Stack<T>& a, Stack<T>& b) noexcept {
    std::swap(a.data,b.data);
}