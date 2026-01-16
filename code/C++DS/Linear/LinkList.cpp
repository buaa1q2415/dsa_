#include <iostream>
#include <algorithm>
#include <iterator>

template <typename T> class LinkList;

template <typename T>
class Node{
    friend class LinkList<T>;
private:
	T data;
	Node* next;
public:
    Node():next(nullptr){}
    Node(T data):data(data),next(nullptr){}
    ~Node()=default;
    Node(const Node<T>& other):data(other.data),next(nullptr){}
    Node(Node<T>&& other) noexcept : data(std::move(other.data)), next(other.next) {
        other.next = nullptr;
    }
    Node<T>& operator=(const Node<T>& other){
        if(this==&other) return *this;
        data=other.data;
        return *this;
    }
    Node<T>& operator=(Node<T>&& other) noexcept{
        if(this==&other) return *this;
        data=std::move(other.data);
        next=other.next;
        other.next=nullptr;
        return *this;
    }


    void setData(const T& data) {this->data=data;}
    const T& getData() const {return data;}
    void setNext(Node<T>* next) {this->next=next;}
    Node<T>* getNext() const {return next;}
};

template <typename T>
class LinkList{
private:
    Node<T>* front,*tail;
	size_t len=0;

    void init() {
        front = new Node<T>();
        tail = front;
        len = 0;
    }
public:
    LinkList(){
        init();
    }

    LinkList(const LinkList<T>& other){
        front=new Node<T>();
        tail=front;
        len=other.len;
        Node<T>* p=other.front->next;
        while(p){
            tail->next = new Node<T>(p->data);
            tail=tail->next;
            p=p->next;
        }
    }

    LinkList(LinkList<T>&& other) noexcept{
        front=other.front;
        tail=other.tail;
        len=other.len;
        other.front=nullptr;
        other.tail=nullptr;
        other.len=0;
    }
    
    ~LinkList(){
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
    
    Node<T>* getHead() const {return front;}

    Node<T>* getTail() const {return tail;}
    
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

    void push_front(const T& data){
        if (!front) init();
        Node<T>* newnode=new Node<T>(data);
        newnode->next=front->next;
        front->next=newnode;
        if(tail==front) tail=newnode;
        len++;
    }

    void push_back(const T& data){
        if (!front) init();
        Node<T>* newnode=new Node<T>(data);
        tail->next=newnode;
        tail=newnode;
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

    void pop_back(){
        if(!front ||front->next==nullptr) return;
        Node<T>* p=front;
        while(p->next!=tail){
            p=p->next;
        }
        delete tail;
        tail=p;
        tail->next=nullptr;
        len--;
        if(len==0) tail=front;
    }

    LinkList& operator=(const LinkList<T>& other){
        if(this==&other) return *this;
        clear();
        Node<T>* p = other.front ? other.front->next : nullptr;
        while(p){
            push_back(p->data);
            p=p->next;
        }
        return *this;
    }

    LinkList& operator=(LinkList<T>&& other) noexcept{
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

        reference operator*() const { return curr->data; }
        pointer operator->() const { return &(curr->data); }

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

    // 删除 pos 之后的那个节点
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
