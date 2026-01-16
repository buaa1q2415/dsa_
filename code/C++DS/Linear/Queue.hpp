#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include <deque>
#include <vector>
#include <list>
#include <stdexcept>
#include <type_traits>
#include <functional>
#include <utility>

/**
 * @file Queue.hpp
 * @brief 提供了高性能的队列（Queue）数据结构实现。
 *
 * 这个文件包含两个版本的队列：
 * 1. 泛型模板类 `Queue<T, Container>`:
 *    - 默认使用 std::deque<T> 作为底层容器，以实现最高性能的入队和出队操作。
 *    - API 模仿了标准库容器，提供了 push, pop, getFront, getBack 等接口。
 *    - 设计为可被继承，以实现更复杂的队列结构。
 *
 * 2. 模板特化 `Queue<T, std::vector<T>>`:
 *    - 当底层容器指定为 std::vector 时，启用此特化版本。
 *    - 内部实现为高效的**循环队列**（Circular Queue），使得 push 和 pop 操作均为摊销 O(1) 复杂度。
 *    - 支持 `reserve` 操作预分配内存。
 *
 * 3. `MonotonicQueue<T, Container, Comparator>`:
 *    - 继承自 `Queue`，实现了单调队列。
 *    - 在元素入队时，会从队尾移除不符合单调性（由比较器定义）的元素。
 *    - 常用于解决滑动窗口最值等算法问题。
 *
 */


// =======================================================================
//                           API 接口文档
// =======================================================================

/*
-------------------------------------------------------------------------
 I. 核心队列: Queue<T, Container>
-------------------------------------------------------------------------
提供了统一的队列接口，具体行为和性能取决于底层容器。

---[ 构造与析构 ]---

/// @brief 默认构造函数 (适用于所有版本)。
explicit Queue();

/// @brief [std::vector 特化版] 构造函数，可预分配容量。
/// @param capacity 内部 vector 的初始容量。默认为 16。
explicit Queue(size_t capacity = 16);

/// @brief 虚析构函数，允许安全地继承。
/// @note [std::vector 特化版] 会确保队列中所有剩余元素被正确析构，防止资源泄漏。
virtual ~Queue();


---[ 核心操作 ]---

/// @brief 在队尾添加一个元素（拷贝）。
/// @complexity 摊销 O(1)。
virtual void push(const T& value);

/// @brief 在队尾添加一个元素（移动）。
/// @complexity 摊销 O(1)。
virtual void push(T&& value);

/// @brief 在队尾就地构造一个元素，避免额外拷贝或移动。
/// @complexity 摊销 O(1)。
/// @note [std::vector 特化版] 使用 placement new 实现最高效的就地构造。
template<typename... Args>
void emplace(Args&&... args);

/// @brief 移除队首元素。
/// @complexity O(1) (对于 std::deque 和 std::vector 特化版)。
/// @note 会抛出 std::underflow_error 如果队列为空。
/// @note [std::vector 特化版] 会确保被移除元素的析构函数被调用。
void pop();

/// @brief 获取队首元素的常引用。
/// @complexity O(1)。
/// @note 会抛出 std::underflow_error 如果队列为空。
const T& getFront() const;

/// @brief 获取队首元素的引用。
/// @complexity O(1)。
/// @note 会抛出 std::underflow_error 如果队列为空。
T& getFront();

/// @brief 获取队尾元素的常引用。
/// @complexity O(1)。
/// @note 会抛出 std::underflow_error 如果队列为空。
const T& getBack() const;

/// @brief 获取队尾元素的引用。
/// @complexity O(1)。
/// @note 会抛出 std::underflow_error 如果队列为空。
T& getBack();


---[ 状态查询与容量管理 ]---

/// @brief 检查队列是否为空。
/// @complexity O(1)。
bool isEmpty() const noexcept;

/// @brief 获取队列中元素的数量。
/// @complexity O(1)。
size_t getSize() const noexcept;

/// @brief 清空队列中的所有元素。
/// @complexity 对于 std::vector 特化版，复杂度与元素数量成正比（因需调用析构），但非常快。
/// @note [std::vector 特化版] 会保持已分配的内存容量，以备后续使用。
void clear() noexcept;

/// @brief [std::vector 特化版] 预留存储空间。
/// 如果 n 大于当前容量，会重新分配内存并将队列元素线性化存储。
/// @complexity O(N)，其中 N 是队列中的元素数量。
void reserve(size_t n);


---[ 辅助功能 ]---

/// @brief [std::vector 特化版] 将队列内容转换为一个线性的 std::vector。
/// @return 一个包含队列所有元素（按逻辑顺序）的新 std::vector 副本。
/// @complexity O(N)，其中 N 是队列中的元素数量。
std::vector<T> to_vector() const;

/// @brief [非特化版] 获取底层容器的常引用，用于调试或特殊操作。
const Container& view() const;


-------------------------------------------------------------------------
 II. 单调队列: MonotonicQueue<T, Container, Comparator>
-------------------------------------------------------------------------
继承自 Queue，用于在算法中维持队列元素的单调性。

---[ 构造函数 ]---

/// @param comp 一个比较器，用于定义单调性。
///             - 默认为 std::less<T>，维持一个单调递增队列（队首最小）。
///               当 `新元素 < 队尾元素` 时，队尾元素会被弹出。
///             - 若要单调递减（队首最大），请使用 std::greater<T>。
explicit MonotonicQueue(const Comparator& comp = {});


---[ 重写操作 ]---

/// @brief 入队，但在入队前会从队尾移除所有不符合单调性的元素。
/// @note 此操作可以无缝地利用底层 Queue 的任何实现（包括高效的循环队列）。
void push(const T& value) override;
void push(T&& value) override;

*/




template <typename T, typename Container = std::deque<T>>
class Queue{
protected:
    Container data;
    size_t size=0;

    void pop_back_internal() {
        data.pop_back();
        size--;
    }
public:
    explicit Queue()=default;
    virtual ~Queue()=default;

    Queue(Queue&&) = default;
    Queue& operator=(Queue&&) = default;

    void clear() noexcept{
        data.clear();
        size=0;
    }

    bool isEmpty() const noexcept{return data.empty();};
    size_t getSize() const noexcept{return size;};

    const T& getFront() const{
        if(isEmpty()) throw std::underflow_error("Queue::getFront(): Queue is empty!");
        return data.front();
    }
    T& getFront(){
        if(isEmpty()) throw std::underflow_error("Queue::getFront(): Queue is empty!");
        return data.front();
    }

    const T& getBack() const{
        if(isEmpty()) throw std::underflow_error("Queue::getBack(): Queue is empty!");
        return data.back();
    }
    T& getBack(){
        if(isEmpty()) throw std::underflow_error("Queue::getBack(): Queue is empty!");
        return data.back();
    }

    virtual void push(const T& value){
        data.push_back(value);
        size++;
    }
    virtual void push(T&& value){
        data.push_back(std::move(value));
        size++;
    }

    template<typename... Args>
    void emplace(Args&&... args){
        data.emplace_back(std::forward<Args>(args)...);
        size++;
    }

    void pop(){
        if(isEmpty()) throw std::underflow_error("Queue::pop(): Queue is empty!");
        data.pop_front();
        size--;
    }

    bool operator==(const Queue<T,Container>& other) const{return data==other.data;};
    bool operator!=(const Queue<T,Container>& other) const{return data!=other.data;};

    const Container& view() const{return data;}

friend void swap(Queue<T, Container>& a, Queue<T, Container>& b) noexcept {
    using std::swap;
    swap(a.data,b.data);
    swap(a.size,b.size);
}

friend std::ostream& operator<<(std::ostream& os,const Queue<T, Container>& q){
    if(q.isEmpty()){
        os<<"[Empty Queue]";
        return os;
    }
    os<<"[FRONT] ";
    for (const auto& it : q.data) {
        os <<it<< " ";
    }
    os<<"[BACK]";
    return os;
}
};


//  当容器为 std::vector 时用循环队列
template <typename T>
class Queue<T, std::vector<T>> {
protected:
    std::vector<T> data;
    size_t head = 0;
    size_t tail = 0;
    size_t size = 0;

    void pop_back_internal(){
        if (isEmpty()) return;
        tail = (tail == 0) ? (data.size() - 1) : (tail - 1);
        size--;
    }
private:
    // 内部扩容函数
    void expand_if_needed() {
        if (size == data.size()) {
            size_t current_capacity = data.size();
            size_t new_capacity = (current_capacity == 0) ? 16 : current_capacity * 2;
            std::vector<T> new_data(new_capacity);
            
            // 复制旧数据到新数据区（将环形数据拉直）
            for (size_t i = 0; i < size; ++i) {
                new_data[i] = std::move(data[(head + i) % current_capacity]);
            }
            
            data = std::move(new_data);
            head = 0;
            tail = size;
        }
    }

public:
    explicit Queue(size_t capacity=16): head(0),tail(0),size(0) { data.resize(capacity);};
    virtual ~Queue() {clear()};

    Queue(Queue&&) = default;
    Queue& operator=(Queue&&) = default;

    void clear() noexcept {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            for (size_t i = 0; i < size; ++i) {
                data[(head + i) % data.size()].~T();
            }
        }
        head = 0;
        tail = 0;
        size = 0;
    }

    bool isEmpty() const noexcept { return size == 0; }
    size_t getSize() const noexcept { return size; }
    
    void reserve(size_t n) {
    if (n > data.size()) {
        data = std::move(to_vector());
        data.resize(n); 
        head = 0;
        tail = size;
    }
}

    const T& getFront() const {
        if (isEmpty()) throw std::underflow_error("Queue::getFront(): Queue is empty!");
        return data[head];
    }
    T& getFront() {
        if (isEmpty()) throw std::underflow_error("Queue::getFront(): Queue is empty!");
        return data[head];
    }

    const T& getBack() const {
        if (isEmpty()) throw std::underflow_error("Queue::getBack(): Queue is empty!");
        return data[(tail + data.size() - 1) % data.size()];
    }
    T& getBack() {
        if (isEmpty()) throw std::underflow_error("Queue::getBack(): Queue is empty!");
        return data[(tail + data.size() - 1) % data.size()];
    }
    
    virtual void push(const T& value) {
        expand_if_needed();
        data[tail] = value;
        tail = (tail + 1) % data.size();
        size++;
    }

    virtual void push(T&& value) {
        expand_if_needed();
        data[tail] = std::move(value);
        tail = (tail + 1) % data.size();
        size++;
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        expand_if_needed();
        data[tail] = T(std::forward<Args>(args)...);
        tail = (tail + 1) % data.size();
        size++;
    }

    void pop() {
        if (isEmpty()) throw std::underflow_error("Queue::pop(): Queue is empty!");

        head = (head + 1) % data.size();
        size--;
    }
    
    bool operator==(const Queue<T, std::vector<T>>& other) const {
        if (size != other.size) return false;
        if (isEmpty()) return true;

        size_t current_capacity = data.size();
        size_t other_capacity = other.data.size();

        for (size_t i = 0; i < size; ++i) {
            if (data[(head + i) % current_capacity] != other.data[(other.head + i) % other_capacity]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Queue<T, std::vector<T>>& other) const {
        return !(*this == other);
    }
    
    std::vector<T> to_vector() const {
        std::vector<T> logical_order_data;
        if (isEmpty()) return logical_order_data;
        
        logical_order_data.reserve(size);

        if (head < tail) {
            logical_order_data.insert(logical_order_data.end(), data.begin() + head, data.begin() + tail);
        } else {
            logical_order_data.insert(logical_order_data.end(), data.begin() + head, data.end());
            logical_order_data.insert(logical_order_data.end(), data.begin(), data.begin() + tail);
        }
        return logical_order_data;
    }

    // 友元函数特化
    friend void swap(Queue<T, std::vector<T>>& a, Queue<T, std::vector<T>>& b) noexcept {
        using std::swap;
        swap(a.data, b.data);
        swap(a.head, b.head);
        swap(a.tail, b.tail);
        swap(a.size, b.size);
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Queue<T, std::vector<T>>& q) {
        if (q.isEmpty()) {
            os << "[Empty Queue]";
            return os;
        }
        os << "[FRONT] ";
        size_t current_capacity = q.data.size();
        for (size_t i = 0; i < q.size; ++i) {
            os << q.data[(q.head + i) % current_capacity] << " ";
        }
        os << "[BACK]";
        return os;
    }
};


//  单调队列实现
template <
    typename T, 
    typename Container = std::deque<T>, 
    typename Comparator = std::less<T>
>
class MonotonicQueue : public Queue<T, Container> {
private:
    Comparator comparator_;
public:
    explicit MonotonicQueue(const Comparator& comp = {}) : Queue<T, Container>(), comparator_(comp) {}
    
    void push(const T& value) override{
        // 从队尾移除所有不符合单调性的元素
        while (!this->isEmpty() && !comparator_(this->getBack(), value)) {
            this->pop_back_internal();
        }
        Queue<T, Container>::push(value);
    }

    void push(T&& value) override{
        while (!this->isEmpty() && !comparator_(this->getBack(), value)) {
            this->pop_back_internal();
        }
        Queue<T, Container>::push(std::move(value));
    }
    
    // emplace 对于单调队列必须先构造出对象才能进行比较，这里我们通过构造临时对象来简化实现。
    template<typename... Args>
    void emplace(Args&&... args){
        T temp_value(std::forward<Args>(args)...);  // 只构造一次
        while (!this->isEmpty() && !comparator_(this->getBack(), temp_value)) {
            this->pop_back_internal();
        }
        Queue::push(std::move(temp_value)); 
    }
};

#endif