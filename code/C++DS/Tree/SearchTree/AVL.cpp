#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstddef>
#include <optional>
#include <stack>

template <typename Elemtype>
class avlNode{
friend class AVL;
private:
    Elemtype data;
    int height;
    avlNode<Elemtype> *left,*right;
public:
    avlNode(Elemtype data):data(data),left(nullptr),right(nullptr),height(1){};
    avlNode():left(nullptr),right(nullptr),height(1){};
    ~avlNode()=default;
    Elemtype GetData(){return data;};
};

template <typename Elemtype>
class AVL{
private:
    avlNode<Elemtype> *root;
    avlNode<Elemtype> RightRotate(avlNode<Elemtype> *p);
    avlNode<Elemtype> LeftRotate(avlNode<Elemtype> *p);
    avlNode<Elemtype> LeftRightRotate(avlNode<Elemtype> *p);
    avlNode<Elemtype> RightLeftRotate(avlNode<Elemtype> *p);
    int GetHeight(avlNode<Elemtype> *p){return (p==nullptr)?0:p->height;};
    int GetBF(avlNode<Elemtype> *p){return (p==nullptr)?0:GetHeight(p->left)-GetHeight(p->right);};
    avlNode<Elemtype> Rebalance(avlNode<Elemtype> *p);
public:
    bool Search(const Elemtype &data) const;
    std::optional<Elemtype> GetNext(const Elemtype &data) const;
    bool Insert(const Elemtype &data);
    bool Delete(const Elemtype &data);
    AVL(const std::vector<Elemtype>& arr);
    ~AVL();
    void InOrderOutput(std::vector<Elemtype>& arr) const;
};

template <typename Elemtype>
bool AVL<Elemtype>::Search(const Elemtype &data) const{
    avlNode<Elemtype> *p=root;
    while(p){
        if(data<p->data) p=p->left;
        else if(data>p->data) p=p->right;
        else return true;
    }
    return false;
}

template <typename Elemtype>
avlNode<Elemtype> AVL<Elemtype>::RightRotate(avlNode<Elemtype> *p){
    if(p==nullptr) return nullptr;
    avlNode<Elemtype> *q=p->left,r=q->right;
    q->right=p;
    p->left=r;
    p->height=std::max(p->left->height,p->right->height)+1;
    q->height=std::max(q->left->height,q->right->height)+1;
    return q;
}

template <typename Elemtype>
avlNode<Elemtype> AVL<Elemtype>::LeftRotate(avlNode<Elemtype> *p){
    if(p==nullptr) return nullptr;
    avlNode<Elemtype> *q=p->right,r=q->left;
    q->left=p;
    p->right=r;
    p->height=std::max(p->left->height,p->right->height)+1;
    q->height=std::max(q->left->height,q->right->height)+1;
    return q;
}

template <typename Elemtype>
avlNode<Elemtype> AVL<Elemtype>::LeftRightRotate(avlNode<Elemtype> *p){
    p->left=LeftRotate(p->left);
    return RightRotate(p);
}

template <typename Elemtype>
avlNode<Elemtype> AVL<Elemtype>::RightLeftRotate(avlNode<Elemtype> *p){
    p->right=RightRotate(p->right);
    return LeftRotate(p);
}

template <typename Elemtype>
avlNode<Elemtype> AVL<Elemtype>::Rebalance(avlNode<Elemtype> *p){
    int bf=GetBF(p);
    if(bf>1){
        if(GetBF(p->left)>0) return RightRotate(p);
        else return LeftRightRotate(p);
    }
    else if(bf<-1){
        if(GetBF(p->right)<0) return LeftRotate(p);
        else return RightLeftRotate(p);
    }
    return p;
}


template <typename Elemtype>
bool AVL<Elemtype>::Insert(const Elemtype &data){
    if(root==nullptr){
        root=new avlNode<Elemtype>(data);
        return true;
    }
    std::vector<avlNode<Elemtype>**> path;
    path.reserve(32);
    avlNode<Elemtype> **p=&root;
    while(*p){
        if(data<(*p)->data){
            path.emplace_back(p);
            p=&((*p)->left);
        }else if(data>(*p)->data){
            path.emplace_back(p);
            p=&((*p)->right);
        }else return false;
    }
    *p=new(std::nothrow) avlNode<Elemtype>(data);
    if(*p==nullptr) return false;
    for(int i=path.size()-1;i>=0;i--){
        (*path[i])->height=std::max(GetHeight((*path[i])->left),GetHeight((*path[i])->right))+1;
        if(abs(GetBF(*path[i]))>1){
            *path[i]=Rebalance(*path[i]);
        }
    }
    return true;
}

template <typename Elemtype>
std::optional<Elemtype> AVL<Elemtype>::GetNext(const Elemtype &data) const{
    avlNode<Elemtype> *p=root;
    std::vector<avlNode<Elemtype>*> path;
    path.reserve(32);
    while(p){
        if(data<p->data){
            path.emplace_back(p);
            p=p->left;
        }else if(data>p->data){
            path.emplace_back(p);
            p=p->right;
        }else{
            break;
        }
    }
    if(p&&p->right){
        p=p->right;
        while(p->left) p=p->left;
        return p->data;
    }
    int top=path.size()-1;
    while(p&&top>=0){
        if(p->data>path[top]->data) p=path[top--];
        else return path[top]->data;
    }
    return std::nullopt;
}

template <typename Elemtype>
bool AVL<Elemtype>::Delete(const Elemtype &data){
    if(root==nullptr) return false;
    std::vector<avlNode<Elemtype>**> path;
    path.reserve(32);
    avlNode<Elemtype> **p=&root;
    while(*p){
        if(data<(*p)->data){
            path.emplace_back(p);
            p=&((*p)->left);
        }else if(data>(*p)->data){
            path.emplace_back(p);
            p=&((*p)->right);
        }else break;
    }
    if(*p==nullptr) return false;
    avlNode<Elemtype> *toDelete = *p;
    if(toDelete->left && toDelete->right){
        avlNode<Elemtype> **successor = &(toDelete->right);
        path.emplace_back(p);
        while((*successor)->left){
            path.emplace_back(successor);
            successor = &((*successor)->left);
        }
        toDelete->data = (*successor)->data;
        toDelete = *successor;
        p = successor;
    }
    avlNode<Elemtype> *child = toDelete->left ? toDelete->left : toDelete->right;
    if(child == nullptr) *p = nullptr;
    else *p = child;
    delete toDelete;
    for(int i=path.size()-1; i>=0; i--){
        (*path[i])->height = std::max(GetHeight((*path[i])->left), GetHeight((*path[i])->right)) + 1;
        if(abs(GetBF(*path[i])) > 1) *path[i] = Rebalance(*path[i]);
    }
    return true;
}

template <typename Elemtype>
void AVL<Elemtype>::InOrderOutput(std::vector<Elemtype>& arr) const{
    if(root){
        std::stack<avlNode<Elemtype>*> stack;
        avlNode<Elemtype>* current=root;
        arr.reserve(GetHeight(root) * 2);
        while(current||!stack.empty()){
            while(current){
                stack.push(current);
                current=current->left;
            }
            current=stack.top();
            stack.pop();
            arr.emplace_back(current->data);
            current=current->right;
        }
    }
}

template <typename Elemtype>
AVL<Elemtype>::AVL(const std::vector<Elemtype>& arr){
    for(auto& i:arr){
        Insert(i);
    }
}

template <typename Elemtype>
AVL<Elemtype>::~AVL(){
    std::stack<avlNode<Elemtype>*> stack;
    avlNode<Elemtype>* current = root;
    avlNode<Elemtype>* prev = nullptr;
    while(current || !stack.empty()){
        while(current){
            stack.push(current);
            current = current->left;
        }
        avlNode<Elemtype>* topNode = stack.top();
        if(topNode->right == nullptr || topNode->right == prev){
            delete topNode;
            prev = topNode;
            stack.pop();
        }else current = topNode->right;
    }
    root = nullptr;//防止悬空指针
}
