#include <iostream>
#include <vector>
#include <cstddef>
#include <optional>

template <typename Elemtype>
class Queue{
friend std::ostream& operator<<(std::ostream& os,const Queue<Elemtype>& q);
private:
    std::vector<Elemtype> data;
    int front,rear,capacity;
public:
    Queue(int capacity);
    ~Queue()=default;
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
    void Enter(Elemtype value);
    std::optional<Elemtype> Exit();
};

template <typename Elemtype>
Queue<Elemtype>::Queue(int capacity){
    this->capacity=capacity;
    front=rear=0;
    data.resize(capacity+1);
}

template <typename Elemtype>
void Queue<Elemtype>::Show(){
    for(int i=front;i!=rear;i=(i+1)%(capacity+1)){
        std::cout<<data[i]<<" ";
    }
}

template <typename Elemtype>
std::ostream& operator<<(std::ostream& os,const Queue<Elemtype>& q){
    for(int i=q.front;i!=q.rear;i=(i+1)%(q.capacity+1)){
        os<<q.data[i]<<" ";
    }
    return os;
}

template <typename Elemtype>
void Queue<Elemtype>::Enter(Elemtype value){
    if(IsFull()){
        std::cout<<"full queue"<<std::endl;
        return;
    }
    data[rear]=value;
    rear=(rear+1)%(capacity+1);
}

template <typename Elemtype>
std::optional<Elemtype> Queue<Elemtype>::Exit(){
    if(IsEmpty()){
        std::cout<<"empty queue"<<std::endl;
        return std::nullopt;
    }
    Elemtype value=data[front];
    front=(front+1)%(capacity+1);
    return value;
}
