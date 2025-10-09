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
class Node{
    friend class Link<Elemtype>;
private:
	Elemtype data;
	struct Node* next;
public:
    Node():next(nullptr){};
    Node(Elemtype data):data(data),next(nullptr){};
    ~Node()=default;
    void SetData(Elemtype data);
    Elemtype GetData();
    void SetNext(Node<Elemtype>* next);
    Node<Elemtype>* GetNext();
};

template <typename Elemtype>
class Link{//带头尾结点，长度，当前结点的链表结构体定义(优化)
private:
    Node<Elemtype>* head,*tail;
	int len=0;
public:
    Link();
    Link(const Link<Elemtype>& other);
    Link(Link<Elemtype>&& other) noexcept;
    ~Link();

    int Len();
    void Show();
    Node<Elemtype>* GetHead();
    Node<Elemtype>* GetTail();
    void Insert(Elemtype value,int index=-1);//默认尾插
    void DelIndex(int index=-1);//默认删末尾
    void DelValue(Elemtype value);
    void Modify(Elemtype value,int index);
    int SearchValue(Elemtype value);
    optional<Elemtype> SearchIndex(int index);
    void Reverse();
    friend Link<Elemtype> Merge(Link<Elemtype>& list1,Link<Elemtype>& list2,int (*cmp)(Elemtype,Elemtype));

    Link& operator=(const Link<Elemtype>& other);
    Link& operator=(Link<Elemtype>&& other) noexcept;
};

template <typename Elemtype>
void Node<Elemtype>::SetData(Elemtype data){
    this->data=data;
}

template <typename Elemtype>
Elemtype Node<Elemtype>::GetData(){
    return this->data;
}

template <typename Elemtype>
void Node<Elemtype>::SetNext(Node<Elemtype>* next){
    this->next=next;
}

template <typename Elemtype>
Node<Elemtype>* Node<Elemtype>::GetNext(){
    return next;
}

template <typename Elemtype>
Link<Elemtype>::Link(){
    head = new Node<Elemtype>();  //定义一个空结点(不存储任何数据),链表初始化时直接创建
	tail = head;
    len=0;
}

template <typename Elemtype>
Link<Elemtype>::Link(const Link<Elemtype>& other){
    head=new Node<Elemtype>();
    tail=head;
    len=0;
    Node<Elemtype>* p=other.head->next;
    while(p){
        Insert(p->data);
        p=p->next;
    }
}


template <typename Elemtype>
Link<Elemtype>::Link(Link<Elemtype>&& other) noexcept{
    head=other.head;
    tail=other.tail;
    len=other.len;
    other.head=nullptr;
    other.tail=nullptr;
    other.len=0;
}

template <typename Elemtype>
int Link<Elemtype>::Len(){
    return len;
}

template <typename Elemtype>
Link<Elemtype>::~Link(){
    Node<Elemtype>* p = head;
    while(p != nullptr){
        head = head->next;
        delete p;
        p = head;
    }
}

template <typename Elemtype>
void Link<Elemtype>::Show(){
    Node<Elemtype>* p = head->next;
    while(p != nullptr){
        cout<<p->data<<" ";
        p = p->next;
    }
}

template <typename Elemtype>
Node<Elemtype>* Link<Elemtype>::GetHead(){
    return head;
}

template <typename Elemtype>
Node<Elemtype>* Link<Elemtype>::GetTail(){
    return tail;
}

template <typename Elemtype>
void Link<Elemtype>::Insert(Elemtype value,int index){
    if(index==-1){
        Node<Elemtype>* node=new Node<Elemtype>(value);
        tail->next=node;
        tail=node;
        len++;
        return;
    }
    if(index<0||index>len){
        cout<<"index error"<<endl;
        return;
    }
    Node<Elemtype>* p=head;
    for(int i=0;i<index&&p;i++){
        p=p->next;
    }
    Node<Elemtype>* q=p->next;
    Node<Elemtype>* node=new Node<Elemtype>(value);
    p->next=node;
    node->next=q;
    len++;
}

template <typename Elemtype>
void Link<Elemtype>::DelIndex(int index){
    if(index==-1) index=len-1;
    if(len==0){
        cout<<"empty link"<<endl;
        return;
    }
    if(index<0||index>=len){
        cout<<"index error"<<endl;
        return;
    }
    Node<Elemtype>* p=head;
    for(int i=0;i<index&&p;i++){
        p=p->next;
    }
    Node<Elemtype>* q=p->next;
    p->next=q->next;
    if(index==len-1) tail=p;
    delete q;
    len--;
}

template <typename Elemtype>
void Link<Elemtype>::DelValue(Elemtype value){
    Node<Elemtype>* p=head;
    while(p->next){
        if(p->next->data==value){
            Node<Elemtype>* q=p->next;
            p->next=q->next;
            if(q==tail) tail=p;
            delete q;
            len--;
        }
        else p=p->next;
    }
}

template <typename Elemtype>
void Link<Elemtype>::Modify(Elemtype value,int index){
    if(index<0||index>=len){
        cout<<"index error"<<endl;
        return;
    }
    Node<Elemtype>* p=head;
    for(int i=0;i<=index&&p;i++){
        p=p->next;
    }
    p->data=value;
}

template <typename Elemtype>
int Link<Elemtype>::SearchValue(Elemtype value){
    Node<Elemtype>* p=head;
    int i=0;
    while(p->next){
        p=p->next;
        if(p->data==value){
            return i;
        }
        i++;
    }
    return -1;
}

template <typename Elemtype>
optional<Elemtype> Link<Elemtype>::SearchIndex(int index){
    if(index<0||index>=len){
        return nullopt;
    }
    if(index==len-1) return tail->data;
    Node<Elemtype>* p=head;
    for(int i=0;i<=index&&p;i++){
        p=p->next;
    }
    return p->data;
}

template <typename Elemtype>
void Link<Elemtype>::Reverse(){
    Node<Elemtype>* p=head->next;
    Node<Elemtype>* q=nullptr,*r=nullptr;
    if(!p) return;
    tail=p;
    while(p){
        r=p->next;
        p->next=q;
        q=p;
        p=r;
    }
    head->next=q;
}

template <typename Elemtype>
Link<Elemtype> Merge(Link<Elemtype>& list1,Link<Elemtype>& list2,int (*cmp)(Elemtype,Elemtype)){
	Link<Elemtype> link;
	Node<Elemtype>* p=list1.head->GetNext();
	Node<Elemtype>* q=list2.head->GetNext();
	while(p&&q){
		if(cmp(p->GetData(),q->GetData())<=0){
			link.Insert(p->GetData());
			p=p->GetNext();
		}else{
			link.Insert(q->GetData());
			q=q->GetNext();
		}
	}
	while(p){
		link.Insert(p->GetData());
		p=p->GetNext();
	}
	while(q){
		link.Insert(q->GetData());
		q=q->GetNext();
	}
	return link;
}

template <typename Elemtype>
Link<Elemtype>& Link<Elemtype>::operator=(const Link<Elemtype>& other){
    if(this==&other) return *this;
    clear();
    head=new Node<Elemtype>();
    tail=head;
    len=0;
    Node<Elemtype>* p=other.head->next;
    while(p){
        Insert(p->data);
        p=p->next;
    }
    return *this;
}

template <typename Elemtype>
Link<Elemtype>& Link<Elemtype>::operator=(Link<Elemtype>&& other) noexcept{
    if(this==&other) return *this;
    head=other.head;
    tail=other.tail;
    len=other.len;
    other.head=nullptr;
    other.tail=nullptr;
    other.len=0;
    return *this;
}
