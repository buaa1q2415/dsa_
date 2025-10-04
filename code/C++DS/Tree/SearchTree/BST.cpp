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
class bstNode{
friend class BST;
private:
    Elemtype data;
    bstNode<Elemtype> *left,*right;
public:
    bstNode(Elemtype data):data(data),left(nullptr),right(nullptr){};
    bstNode():left(nullptr),right(nullptr){};
    ~bstNode()=default;
    Elemtype GetData(){return data;};
};

template <typename Elemtype>
class BST{
private:
    bstNode<Elemtype> *root;
public:
    bool Search(const Elemtype &data) const;
    std::optional<Elemtype> GetNext(const Elemtype &data) const;
    bool Insert(const Elemtype &data);
    bool Delete(const Elemtype &data);
    BST(const std::vector<Elemtype>& arr);
    ~BST();
    void InOrderOutput(std::vector<Elemtype>& arr) const;
};

template <typename Elemtype>
bool BST<Elemtype>::Search(const Elemtype &data) const{
    bstNode<Elemtype> *p=root;
    while(p){
        if(data<p->data) p=p->left;
        else if(data>p->data) p=p->right;
        else return true;
    }
    return false;
}

template <typename Elemtype>
bool BST<Elemtype>::Insert(const Elemtype &data){
    bstNode<Elemtype> **p=&root;
    while(*p){
        if(data<(*p)->data)  p=&((*p)->left);
        else if(data>(*p)->data) p=&((*p)->right);
        else return false;
    }
    *p=new bstNode<Elemtype>(data);
    return true;
}

template <typename Elemtype>
std::optional<Elemtype> BST<Elemtype>::GetNext(const Elemtype &data) const{
    bstNode<Elemtype> *p=root;
    std::vector<bstNode<Elemtype>*> path;
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
bool BST<Elemtype>::Delete(const Elemtype &data){

}

template <typename Elemtype>
void BST<Elemtype>::InOrderOutput(std::vector<Elemtype>& arr) const{
    if(root){
        std::stack<bstNode<Elemtype>*> stack;
        bstNode<Elemtype>* current=root;
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
BST<Elemtype>::BST(const std::vector<Elemtype>& arr){
    for(auto& i:arr){
        Insert(i);
    }
}

template <typename Elemtype>
BST<Elemtype>::~BST(){
    std::stack<bstNode<Elemtype>*> stack;
    bstNode<Elemtype>* current = root;
    bstNode<Elemtype>* prev = nullptr;
    while(current || !stack.empty()){
        while(current){
            stack.push(current);
            current = current->left;
        }
        bstNode<Elemtype>* topNode = stack.top();
        if(topNode->right == nullptr || topNode->right == prev){
            delete topNode;
            prev = topNode;
            stack.pop();
        }else current = topNode->right;
    }
    root = nullptr;//防止悬空指针
}