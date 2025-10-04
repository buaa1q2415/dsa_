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
#include <queue>

//enum ptag{Link,Thread};

template <typename Elemtype>
class tNode{
friend class BiTree;
private:
    Elemtype data;
    tNode<Elemtype> *left,*right;
    //tNode<Elemtype> *parent;//父节点，三叉链表表示法
    //int size;//记录树的节点数
    //ptag ltag,rtag;//标志区分孩子节点和线索结点，线索二叉树
public:
    tNode(Elemtype data):data(data),left(nullptr),right(nullptr){};
    tNode():left(nullptr),right(nullptr){};
    ~tNode()=default;
};

template <typename Elemtype>
class BiTree{
private:
    tNode<Elemtype> *root;
public:
    BiTree();
    BiTree(const BiTree<Elemtype>& other);
    BiTree(BiTree<Elemtype>&& other) noexcept;
    ~BiTree();

    BiTree& operator=(const BiTree<Elemtype>& other);
    BiTree& operator=(BiTree<Elemtype>&& other) noexcept;

    void PreTraverse();
    void InTraverse();
    void PostTraverse();
    void LevelTraverse();

};

template <typename Elemtype>
void BiTree<Elemtype>::PreTraverse(){
    if(root){
        std::stack<tNode<Elemtype>*> stack;
        tNode<Elemtype>* current; 
        stack.emplace(root);
        while(!stack.empty()){
            current=stack.top();
            stack.pop();
            //visitFunc(current);
            if(current->right) stack.emplace(current->right);
            if(current->left) stack.emplace(current->left);
        }
    }
}

template <typename Elemtype>
void BiTree<Elemtype>::InTraverse(){
    if(root){
        std::stack<avlNode<Elemtype>*> stack;
        avlNode<Elemtype>* current=root;
        while(current||!stack.empty()){
            while(current){
                stack.emplace(current);
                current=current->left;
            }
            current=stack.top();
            stack.pop();
            //visitFunc(current);
            current=current->right;
        }
    }
}

//看不懂后一个的逻辑就用这个
// template <typename Elemtype>
// void BiTree<Elemtype>::PostTraverse(){
//     if(root){
//         std::stack<tNode<Elemtype>*> stack;
//         tNode<Elemtype>* current = root;
//         tNode<Elemtype>* prev = root;
//         stack.emplace(current);
//         while(!stack.empty()){
//             current=stack.top();
//             if(current->left && prev!=current->left && prev!=current->right){
//                 stack.emplace(current->left);
//             }else if(current->right && prev!=current->right){
//                 stack.emplace(current->right);
//             }else{
//                 //visitFunc(current);
//                 prev=current;
//                 stack.pop();
//             }
//         }
//     }
// }

template <typename Elemtype>
void BiTree<Elemtype>::PostTraverse(){
    std::stack<tNode<Elemtype>*> stack;
    tNode<Elemtype>* current = root;
    tNode<Elemtype>* prev = nullptr;
    while(current || !stack.empty()){
        while(current){
            stack.emplace(current);
            current = current->left;
        }
        tNode<Elemtype>* topNode = stack.top();
        if(topNode->right == nullptr || topNode->right == prev){
            //visitFunc(topNode);
            prev = topNode;
            stack.pop();
        }else current = topNode->right;
    }
}

template <typename Elemtype>
void BiTree<Elemtype>::LevelTraverse(){
    if(root){
        std::queue<tNode<Elemtype>*> queue;
        tNode<Elemtype>* current;
        queue.emplace(root);
        while(!queue.empty()){
            current=queue.front();
            queue.pop();
            //visitFunc(current);
            if(current->left) queue.emplace(current->left);
            if(current->right) queue.emplace(current->right);
        }
    }
}

