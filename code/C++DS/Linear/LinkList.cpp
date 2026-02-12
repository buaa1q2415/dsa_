#include <iostream>
#include <algorithm>
#include <iterator>

template <typename T> class Link;
template <typename T> class CircleLink;

template <typename T>
class Node{
    template <typename U> friend class Link<U>;
private:
	T value;
	Node* next;

    Node():next(nullptr){}
    Node(const T& value):value(value),next(nullptr){}
    Node(const Node<T>& other):value(other.value),next(nullptr){}
    Node(Node<T>&& other) noexcept : value(std::move(other.value)), next(other.next) {
        other.next = nullptr;
    }

    ~Node()=default;

    Node<T>& operator=(const Node<T>& other){
        if(this==&other) return *this;
        value=other.value;
        return *this;
    }
    Node<T>& operator=(Node<T>&& other) noexcept{
        if(this==&other) return *this;
        value=std::move(other.value);
        next=other.next;
        other.next=nullptr;
        return *this;
    }

};

template <typename T>
class Link{
private:
    Node<T>* front,*tail;
	size_t len=0;

    void init() {
        front = new Node<T>();
        tail = front;
        len = 0;
    }
public:
    Link(){
        init();
    }

    Link(const Link<T>& other){
        front=new Node<T>();
        tail=front;
        len=other.len;
        Node<T>* p=other.front->next;
        while(p){
            tail->next = new Node<T>(p->value);
            tail=tail->next;
            p=p->next;
        }
    }

    Link(Link<T>&& other) noexcept{
        front=other.front;
        tail=other.tail;
        len=other.len;
        other.front=nullptr;
        other.tail=nullptr;
        other.len=0;
    }
    
    ~Link(){
        clear();
        delete front;
        front=nullptr;
        tail=nullptr;
    }

    void clear() {
        if (!front) return;
        Node<T>* p = front->next;
        while(p != nullptr){
            Node<T>* temp = p;
            p = p->next;
            delete temp;
        }
        front->next = nullptr;
        tail = front;
        len = 0;
    }

    size_t getSize() const {return len;}
    
    void reverse(){
        Node<T>* p=front->next;
        Node<T>* q=nullptr,*r=nullptr;
        if(!p) return;
        tail=p;
        while(p){
            r=p->next;
            p->next=q;
            q=p;
            p=r;
        }
        front->next=q;
    }

    void push_front(const T& value){
        if (!front) init();
        Node<T>* newnode=new Node<T>(value);
        newnode->next=front->next;
        front->next=newnode;
        if(tail==front) tail=newnode;
        len++;
    }

    void pop_front(){
        if(!front || front->next==nullptr) return;
        Node<T>* p=front->next;
        front->next=p->next;
        delete p;
        len--;
        if(len==0) tail=front;
    }
    
    Link& operator=(const Link<T>& other){
        if(this==&other) return *this;
        clear();
        Node<T>* p = other.front ? other.front->next : nullptr;
        while(p){
            push_back(p->value);
            p=p->next;
        }
        return *this;
    }

    Link& operator=(Link<T>&& other) noexcept{
        if(this==&other) return *this;
        clear();
        delete front;

        front=other.front;
        tail=other.tail;
        len=other.len;

        other.front=nullptr;
        other.tail=nullptr;
        other.len=0;

        return *this;
    }


    class iterator {
    private:
        Node<T>* curr;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(Node<T>* ptr = nullptr) : curr(ptr) {}

        reference operator*() const { return curr->value; }
        pointer operator->() const { return &(curr->value); }

        iterator& operator++() { // 前置 ++
            if(curr) curr = curr->next;
            return *this;
        }

        iterator operator++(int) { // 后置 ++
            iterator temp = *this;
            if(curr) curr = curr->next;
            return temp;
        }

        bool operator==(const iterator& other) const { return curr == other.curr; }
        bool operator!=(const iterator& other) const { return curr != other.curr; }
    };

    // begin 指向 dummy head 的下一个节点
    iterator begin() { return iterator(front ? front->next : nullptr); }
    iterator end() { return iterator(nullptr); }

    const_iterator begin() const { return const_iterator(front ? front->next : nullptr); }
    const_iterator end() const { return const_iterator(nullptr); }

    // 返回指向 Dummy Head 的迭代器
    iterator before_begin() { 
        return iterator(front); 
    }

    // 在 pos 之后插入 value
    iterator insert_after(iterator pos, const T& value) {
        Node<T>* current = pos.curr;
        if (!current) return end(); // 无效迭代器

        Node<T>* newNode = new Node<T>(value);
        newNode->next = current->next;
        current->next = newNode;

        // 如果是在最后一个节点后插入，更新 tail
        if (current == tail) {
            tail = newNode;
        }
        
        len++;
        return iterator(newNode);
    }

    iterator erase_after(iterator pos) {
        Node<T>* current = pos.curr;
        // 如果 pos 本身无效，或者 pos 已经是最后一个节点(后面没东西删)，直接返回
        if (!current || !current->next) return end();

        Node<T>* nodeToDelete = current->next;
        current->next = nodeToDelete->next;

        // 如果删除的是 tail，tail 指针前移
        if (nodeToDelete == tail) {
            tail = current;
        }

        delete nodeToDelete; // Node 析构不删 next，安全
        len--;
        
        // 返回被删除节点原来的下一个节点
        return iterator(current->next);
    }
};


template <typename T>
class CircleNode{
    template <typename U> friend class CircleLink<U>;
private:
    T value;
    CircleNode* next;
    CircleNode* prev;

    CircleNode():next(nullptr),prev(nullptr){}
    CircleNode(const T& value):value(value),next(nullptr),prev(nullptr){}
    CircleNode(const CircleNode<T>& other):value(other.value),next(nullptr),prev(nullptr){}
    CircleNode(CircleNode<T>&& other) noexcept : value(std::move(other.value)), next(other.next),prev(other.prev) {
        other.next = nullptr;
        other.prev = nullptr;
    }

    ~CircleNode()=default;

    CircleNode<T>& operator=(const CircleNode<T>& other){
        if(this==&other) return *this;
        value=other.value;
        return *this;
    }

    CircleNode<T>& operator=(CircleNode<T>&& other) noexcept{
        if(this==&other) return *this;
        value=std::move(other.value);
        next=other.next;
        prev=other.prev;
        other.next=nullptr;
        other.prev=nullptr;
        return *this;
    }

};

template <typename T>
class CircleLink{
private:
    CircleNode<T>* dummy;
	size_t len=0;

    void init() {
        dummy = new CircleNode<T>();
        dummy->next = dummy;
        dummy->prev = dummy;
        len = 0;
    }

public:
    CircleLink(){
        init();
    }

    CircleLink(const CircleLink<T>& other){
        dummy = new CircleNode<T>();
        dummy->next = dummy;
        dummy->prev = dummy;
        len=other.len;
        CircleNode<T>* p=other.dummy->next;
        while(p!=other.dummy){
            dummy->prev->next = new CircleNode<T>(p->value);
            dummy->prev->next->prev=dummy->prev;
            dummy->prev=dummy->prev->next;
            dummy->prev->next=dummy;
            p=p->next;
        }

    }

    CircleLink(CircleLink<T>&& other) noexcept{
        dummy=other.dummy;
        len=other.len;
        other.dummy=nullptr;
        other.len=0;
    }
    
    ~CircleLink(){
        clear();
        delete dummy;
        dummy=nullptr;
    }

    void clear() {
        if (!dummy) return;
        CircleNode<T>* p = dummy->next;
        while(p != dummy){
            CircleNode<T>* temp = p;
            p = p->next;
            delete temp;
        }
        dummy->next=dummy;
        dummy->prev=dummy;
        len=0;
    }

    size_t getSize() const {return len;}
    
    void reverse(){
        if(len<=1) return;
        CircleNode<T>* p = dummy,*q = p->next,*r = q->next;
        for (int i=0;i<len;i++){
            q->next = p;
            p->prev = q;
            q = p;
            p = r;
            r = r->next;
        }
    }

    class iterator {
    private:
        CircleNode<T>* curr;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(CircleNode<T>* ptr = nullptr) : curr(ptr) {}

        reference operator*() const { return curr->value; }
        pointer operator->() const { return &(curr->value); }

        iterator& operator++() { // 前置 ++
            if(curr) curr = curr->next;
            return *this;
        }

        iterator operator++(int) { // 后置 ++
            iterator temp = *this;
            if(curr) curr = curr->next;
            return temp;
        }

        bool operator==(const iterator& other) const { return curr == other.curr; }
        bool operator!=(const iterator& other) const { return curr != other.curr; }
    };

    iterator push_front(const T& value){
        CircleNode<T>* newnode=new CircleNode<T>(value);
        CircleNode<T>* p=dummy->next;
        dummy->next=newnode;
        newnode->next=p;
        p->prev=newnode;
        newnode->prev=dummy;
        len++;
        return iterator(newnode);
    }

    iterator push_back(const T& value){
        CircleNode<T>* newnode=new CircleNode<T>(value);
        CircleNode<T>* p=dummy->prev;
        dummy->prev=newnode;
        newnode->next=dummy;
        p->next=newnode;
        newnode->prev=p;
        len++;
        return iterator(newnode);
    }

    iterator pop_front(){
        if(dummy->next==dummy) return end();
        CircleNode<T>* p=dummy->next, *q=p->next;
        dummy->next=q;
        q->prev=dummy;
        delete p;
        len--;
        return iterator(dummy->next);
    }
    
    iterator pop_back(){
        if(dummy->prev==dummy) return end();
        CircleNode<T>* p=dummy->prev, *q=p->prev;
        dummy->prev=q;
        q->next=dummy;
        delete p;
        len--;
        return iterator(dummy->prev);
    }

    CircleLink& operator=(const CircleLink<T>& other){
        if(this==&other) return *this;
        clear();
        CircleNode<T>* p = other.dummy->next;
        while(p!=other.dummy){
            push_back(p->value);
            p=p->next;
        }
        return *this;
    }

    CircleLink& operator=(CircleLink<T>&& other) noexcept{
        if(this==&other) return *this;
        clear();
        delete dummy;

        dummy=other.dummy;
        len=other.len;

        other.dummy=nullptr;
        other.len=0;

        return *this;
    }

    // begin 指向 dummy head 的下一个节点
    iterator begin() { return iterator(dummy->next); }
    iterator end() { return iterator(dummy); }

    const_iterator begin() const { return const_iterator(dummy->next); }
    const_iterator end() const { return const_iterator(dummy); }
    
    iterator rbegin() {return std::reverse_iterator<iterator>(end());}
    iterator rend() {return std::reverse_iterator<iterator>(begin());}

    const_iterator rbegin() const {return std::reverse_iterator<const_iterator>(end());}
    const_iterator rend() const {return std::reverse_iterator<const_iterator>(begin());}

    iterator insert_after(iterator pos, const T& value) {
        Node<T>* current = pos.curr;
        if (!current) return end();

        Node<T>* newNode = new Node<T>(value);
        Node<T>* pos_next = current->next;
        newNode->next = pos_next;
        pos_next->prev = newNode;
        current->next = newNode;
        newNode->prev = current;
        
        len++;
        return iterator(newNode);
    }

    iterator insert_before(iterator pos, const T& value) {
        Node<T>* current = pos.curr;
        if (!current) return end();

        Node<T>* newNode = new Node<T>(value);
        Node<T>* pos_prev = current->prev;
        newNode->next = current;
        current->prev = newNode;
        pos_prev->next = newNode;
        newNode->prev = pos_prev;
        
        len++;
        return iterator(newNode);
    }

    iterator erase_after(iterator pos) {
        Node<T>* current = pos.curr;
        // 如果 pos 本身无效，或者 pos 已经是最后一个节点(后面没东西删)，直接返回
        if (!current || !current->next) return end();

        Node<T>* nodeToDelete = current->next;
        current->next = nodeToDelete->next;

        // 如果删除的是 tail，tail 指针前移
        if (nodeToDelete == tail) {
            tail = current;
        }

        delete nodeToDelete; // Node 析构不删 next，安全
        len--;
        
        // 返回被删除节点原来的下一个节点
        return iterator(current->next);
    }
};



