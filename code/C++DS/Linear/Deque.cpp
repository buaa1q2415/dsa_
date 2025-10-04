#include <iostream>
#include <vector>
#include <cstddef>
#include <optional>

template <typename Elemtype>
class Deque{
friend std::ostream& operator<<(std::ostream& os,const Deque<Elemtype>& q);
private:
    std::vector<Elemtype> data;
    int front,rear,capacity;
public:
    Deque(int capacity);
    ~Deque()=default;
    void Clear(){front=rear=0;};
    bool IsEmpty() const {return front==rear;}; 
    bool IsFull() const {return (rear+1)%(capacity+1)==front;};
    std::optional<Elemtype> GetFront() const {
        return IsEmpty()?std::nullopt:std::optional<Elemtype>(data[front]);
    }
    std::optional<Elemtype> GetRear() const {
        return IsEmpty()?std::nullopt:std::optional<Elemtype>(data[(rear+capacity)%(capacity+1)]);
    }
    int GetSize() const {return (rear-front+capacity+1)%(capacity+1);};
    void Show();
    void EnterFront(Elemtype value);
    void EnterBack(Elemtype value);
    std::optional<Elemtype> ExitFront();
    std::optional<Elemtype> ExitBack();
};

template <typename Elemtype>
Deque<Elemtype>::Deque(int capacity){
    this->capacity=capacity;
    front=rear=0;
    data.resize(capacity+1);
}

template <typename Elemtype>
void Deque<Elemtype>::Show(){
    for(int i=front;i!=rear;i=(i+1)%(capacity+1)){
        std::cout<<data[i]<<" ";
    }
}

template <typename Elemtype>
std::ostream& operator<<(std::ostream& os,const Deque<Elemtype>& q){
    for(int i=q.front;i!=q.rear;i=(i+1)%(q.capacity+1)){
        os<<q.data[i]<<" ";
    }
    return os;
}

template <typename Elemtype>
void Deque<Elemtype>::EnterFront(Elemtype value){
    if(IsFull()){
        std::cout<<"full deque"<<std::endl;
        return;
    }
    front=(front+capacity)%(capacity+1);
    data[front]=value;
}

template <typename Elemtype>
void Deque<Elemtype>::EnterBack(Elemtype value){
    if(IsFull()){
        std::cout<<"full deque"<<std::endl;
        return;
    }
    data[rear]=value;
    rear=(rear+1)%(capacity+1);
}

template <typename Elemtype>
std::optional<Elemtype> Deque<Elemtype>::ExitFront(){
    if(IsEmpty()){
        std::cout<<"empty deque"<<std::endl;
        return std::nullopt;
    }
    Elemtype value=data[front];
    front=(front+1)%(capacity+1);
    return value;
}

template <typename Elemtype>
std::optional<Elemtype> Deque<Elemtype>::ExitBack(){
    if(IsEmpty()){
        std::cout<<"empty deque"<<std::endl;
        return std::nullopt;
    }
    rear=(rear+capacity)%(capacity+1);
    Elemtype value=data[rear];
    return value;
}
