#ifndef STACK_HPP
#define STACK_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <stdexcept>
#include <type_traits>
#include <functional>
#include <utility>

/**
 * @file Stack.hpp
 * @brief 提供了高性能、功能丰富的栈（Stack）数据结构实现。
 *
 * 这个文件提供了三个核心组件：
 * 1. 泛型模板类 `Stack<T, Container>`:
 *    - 默认使用 std::deque<T> 作为底层容器，因为它在两端操作上都有很好的性能表现。
 *    - API 模仿了标准库容器，提供了 push, pop, getTop 等接口。
 *    - 该泛型版本可无缝适配 std::vector, std::list, std::deque。
 *
 * 2. 模板特化 `Stack<T, std::forward_list<T>>`:
 *    - 当底层容器指定为 std::forward_list 时，启用此特化版本。
 *    - 利用 std::forward_list 的 O(1) 头部插入/删除特性，所有栈操作均为 O(1) 复杂度。
 *
 * 3. `MonotonicStack<T, Container, Comparator>`:
 *    - 继承自 `Stack`，实现了单调栈。
 *    - 在元素入栈时，会从栈顶移除不符合单调性（由比较器定义）的元素。
 *    - 常用于解决“下一个更大/更小元素”等算法问题。
 */


// =======================================================================
//                           API 接口文档
// =======================================================================

/*
-------------------------------------------------------------------------
 I. 核心栈: Stack<T, Container> (泛用版)
-------------------------------------------------------------------------
默认使用 std::deque<T>，同时支持 std::vector<T> 和 std::list<T>。

---[ 构造与容量管理 ]---

/// @brief 默认构造函数。
explicit Stack() = default;

/// @brief 虚析构函数，允许安全地继承。
virtual ~Stack() = default;

/// @brief [仅当容器为 std::vector 时有效] 预留存储空间。
void reserve(size_t n);

/// @brief [仅当容器为 std::vector 时有效] 获取当前容量。
size_t getCapacity() const noexcept;


---[ 核心操作 ]---

/// @brief 在栈顶添加一个元素（入栈）。
/// @complexity 摊销 O(1)。
virtual void push(const T& value);
virtual void push(T&& value);

/// @brief 在栈顶就地构造一个元素。
/// @complexity 摊销 O(1)。
template<typename... Args>
void emplace(Args&&... args);

/// @brief 移除栈顶元素（出栈）。
/// @complexity O(1)。
/// @note 会抛出 std::underflow_error 如果栈为空。
void pop();

/// @brief 获取栈顶元素的常引用。
/// @complexity O(1)。
/// @note 会抛出 std::underflow_error 如果栈为空。
const T& getTop() const;
T& getTop();


---[ 状态查询 ]---

/// @brief 检查栈是否为空。
/// @complexity O(1)。
bool isEmpty() const noexcept;

/// @brief 获取栈中元素的数量。
/// @complexity O(1)。
size_t getSize() const noexcept;

/// @brief 清空栈。
void clear() noexcept;


-------------------------------------------------------------------------
 II. 核心栈: Stack<T, std::forward_list<T>> (特化版)
-------------------------------------------------------------------------
为 std::forward_list 优化的版本，栈顶对应链表头部。

---[ 核心操作 ]---
(push, pop, getTop, emplace 等接口与泛用版一致，但内部实现为对链表头部的操作。)

@note 此版本不支持容量管理（reserve, getCapacity）。


-------------------------------------------------------------------------
 III. 单调栈: MonotonicStack<T, Container, Comparator>
-------------------------------------------------------------------------
继承自 Stack，用于维持栈的单调性。

---[ 构造函数 ]---

/// @param comp 一个比较器，用于定义单调性。
///             - 默认为 std::less<T>，维持一个单调递增栈（栈顶最大）。
///               当 `新元素 < 栈顶元素` 时，栈顶元素会被弹出。
///             - 若要单调递减（栈顶最小），请使用 std::greater<T>。
explicit MonotonicStack(const Comparator& comp = {});


---[ 重写操作 ]---

/// @brief 入栈，但在入栈前会从栈顶移除所有不符合单调性的元素。
void push(const T& value) override;
void push(T&& value) override;

*/


template <typename T, typename Container = std::deque<T>>
class Stack{
protected:
    Container data;

public:
    explicit Stack() = default;
    virtual ~Stack() = default;

    Stack(Stack&&) = default;
    Stack& operator=(Stack&&) = default;

    void clear() noexcept{ data.clear();}

    bool isEmpty() const noexcept { return data.empty(); }
    size_t getSize() const noexcept { return data.size(); }

    void reserve(size_t n) {
        if constexpr (std::is_same_v<Container, std::vector<T>>) {
            data.reserve(n);
        }
    }

    size_t getCapacity() const noexcept {
        if constexpr (std::is_same_v<Container, std::vector<T>>) {
            return data.capacity();
        }
        return 0;
    }

    const T& getTop() const {
        if(isEmpty()) throw std::underflow_error("Stack::getTop(): Stack is empty!");
        return data.back();
    }
    T& getTop() {
        if (isEmpty()) throw std::underflow_error("Stack::getTop(): Stack is empty!");
        return data.back();
    }

    virtual void push(const T& value) { data.push_back(value);}
    virtual void push(T&& value) { data.push_back(std::move(value));}

    template<typename... Args>
    void emplace(Args&&... args) { data.emplace_back(std::forward<Args>(args)...);}

    void pop() {
        if(data.empty()) throw std::underflow_error("Stack::pop() : Stack is empty!");
        data.pop_back();
    }

    bool operator==(const Stack<T,Container>& other) const { return data == other.data; }
    bool operator!=(const Stack<T,Container>& other) const { return data != other.data; }

    const Container& view() const { return data; }

    friend void swap(Stack<T, Container>& a, Stack<T, Container>& b) noexcept {
        using std::swap;
        swap(a.data, b.data);
    }

    friend std::ostream& operator<<(std::ostream& os, const Stack<T, Container>& s) {
        if(s.isEmpty()){
            os << "[Empty Stack]";
            return os;
        }
        os << "[TOP] ";
        for(auto it = s.data.rbegin(); it != s.data.rend(); ++it) {
            os << *it << " ";
        }
        os << "[BOTTOM]";
        return os;
    }
};


// 容器为 std::forward_list

template <typename T>
class Stack<T, std::forward_list<T>> {
protected:
    std::forward_list<T> data;
    size_t size = 0;

public:
    explicit Stack() = default;
    virtual ~Stack() = default;

    Stack(Stack&&) = default;
    Stack& operator=(Stack&&) = default;
    
    void clear() noexcept {
        data.clear();
        size = 0;
    }

    bool isEmpty() const noexcept { return data.empty(); }
    size_t getSize() const noexcept { return size; }

    const T& getTop() const {
        if (isEmpty()) throw std::underflow_error("Stack::getTop(): Stack is empty!");
        return data.front();
    }
    T& getTop() {
        if (isEmpty()) throw std::underflow_error("Stack::getTop(): Stack is empty!");
        return data.front();
    }

    virtual void push(const T& value) {
        data.push_front(value);
        size++;
    }
    virtual void push(T&& value) {
        data.push_front(std::move(value));
        size++;
    }

    template<typename... Args>
    void emplace(Args&&... args) {
        data.emplace_front(std::forward<Args>(args)...);
        size++;
    }

    void pop() {
        if (isEmpty()) throw std::underflow_error("Stack::pop() : Stack is empty!");
        data.pop_front();
        size--;
    }

    bool operator==(const Stack<T, std::forward_list<T>>& other) const{
        if(size!=other.size) return false;
        auto it1=data.begin(),it2=other.data.begin();
        while(it1!=data.end()){
            if(*it1!=*it2) return false;
            it1++,it2++;
        }
        return true;
    }

    bool operator!=(const Stack<T, std::forward_list<T>>& other) const{
        return !(*this==other);
    }

    const std::forward_list<T>& view() const { return data; }


friend void swap(Stack<T, std::forward_list<T>>& a, Stack<T, std::forward_list<T>>& b) noexcept {
    using std::swap;
    a.data.swap(b.data);
    swap(a.size, b.size);
}

friend std::ostream& operator<<(std::ostream& os, const Stack<T, std::forward_list<T>>& s) {
    if(s.isEmpty()){
        os << "[Empty Stack]";
        return os;
    }
    os << "[TOP] ";
    for (const auto& it : s.data) {
        os << it << " ";
    }
    os << "[BOTTOM]";
    return os;
}
};


//  单调栈

template <
    typename T, 
    typename Container = std::deque<T>,
    typename Comparator = std::less<T>   // 默认为单调递增 (栈顶最大)
>
class MonotonicStack : public Stack<T, Container> {
private:
    Comparator comparator_;
public:
    explicit MonotonicStack(const Comparator& comp = {}) 
        : Stack<T, Container>(), comparator_(comp) {}
    
    void push(const T& value) override {
        while (!this->isEmpty() && comparator_(value, this->getTop())) {
            this->pop();
        }
        Stack<T, Container>::push(value);
    }

    void push(T&& value) override {
        while (!this->isEmpty() && comparator_(value, this->getTop())) {
            this->pop();
        }
        Stack<T, Container>::push(std::move(value));
    }
    
    template<typename... Args>
    void emplace(Args&&... args) {
        T value(std::forward<Args>(args)...);
        push(std::move(value)); // 复用 push 的逻辑
    }
};

#endif