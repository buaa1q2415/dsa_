#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstddef>
#include <optional>
using namespace std;

template <typename Elemtype>
class Stack{
private:
    vector<Elemtype> data;
    int top,size;
public:
    Stack(int size=105);
    ~Stack()=default;
    bool resize(int size);
    void Clear();
    bool IsEmpty();
    bool IsFull();
    optional<Elemtype> GetTop();
    int GetLen();
    void Show(bool reverse=false);
    void Push(Elemtype value);
    optional<Elemtype> Pop();
    optional<Elemtype> operator--(int);
};
template <typename Elemtype>
Stack<Elemtype>::Stack(int size){
    this->size=size;
    top=-1;
    data.resize(size);
}

template <typename Elemtype>
bool Stack<Elemtype>::resize(int size){
    if(size<=0) return false;
    this->size=size;
    data.resize(size);
    return true;
}

template <typename Elemtype>
void Stack<Elemtype>::Clear(){
    top=-1;
}

template <typename Elemtype>
bool Stack<Elemtype>::IsEmpty(){
    return top==-1;
}

template <typename Elemtype>
bool Stack<Elemtype>::IsFull(){
    return top==size-1;
}

template <typename Elemtype>
optional<Elemtype> Stack<Elemtype>::GetTop(){
    if(IsEmpty()) return nullopt;
    return data[top];
}

template <typename Elemtype>
int Stack<Elemtype>::GetLen(){
    return top+1;
}

template <typename Elemtype>
void Stack<Elemtype>::Show(bool reverse){
    if(!reverse){
        for(int i=top;i>=0;i--){
            cout<<data[i]<<" ";
        }
    }
    else{
        for(int i=0;i<=top;i++){
            cout<<data[i]<<" ";
        }
    }
    cout<<endl;
}

template <typename Elemtype>
void Stack<Elemtype>::Push(Elemtype value){
    if(IsFull()){
        cout<<"full stack"<<endl;
        return;
    }
    data[++top]=value;
}

template <typename Elemtype>
optional<Elemtype> Stack<Elemtype>::Pop(){
    if(IsEmpty()) return nullopt;
    return data[top--];
}

template <typename Elemtype>
optional<Elemtype> Stack<Elemtype>::operator--(int){
    if(IsEmpty()) return nullopt;
    return data[top--];
}