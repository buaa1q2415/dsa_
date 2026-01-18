#ifndef BST_HPP
#define BST_HPP


#include "BiTree.hpp"



template <typename T>
class BST:public BiTree<T>{
// 继承与隐藏
protected:
    using BiTree<T>::find; 
    using BiTree<T>::locate;
    using BiTree<T>::getRoot;
    using BiTree<T>::setRoot;
    using BiTree<T>::parent_of;
    using BiTree<T>::Traverse;


public:
    using BiTree<T>::countNodes;
    using BiTree<T>::countLeaves;
    using BiTree<T>::getHeight;
    using BiTree<T>::isEmpty;
    using BiTree<T>::operator==;
    using BiTree<T>::operator!=;


public:
    // 语义

    BST(const BST<T>& other) : BiTree<T>(other) {}

    BST(BST<T>&& other) noexcept : BiTree<T>(std::move(other)) {}

    BST(std::initializer_list<T> list) : BiTree<T>() {
        for (const auto& item : list) {
            this->insert(item);
        }
    }

    BST(std::vector<T>& vec) : BiTree<T>() {
        
    }

    virtual ~BST() override =default;

    BST<T>& operator=(const BST<T>& other) {
        if (this != &other) {
            BiTree<T>::operator=(other);
        }
        return *this;
    }

    BST& operator=(BST&& other) noexcept {
        if (this != &other){
            BiTree<T>::operator=(std::move(other));
        }
        return *this;
    }


public:
    bool find(const T& data) const {
        tNode<T>* current = root;
        while (current) {
            if (data == current->data) return true;
            if (data < current->data) current = current->left;
            else current = current->right;
        }
        return false;
    }

    void insert(const T& data) {
        tNode<T>* current = root;
        tNode<T>* parent = nullptr;
        while (current) {
            parent = current;
            if (data == current->data) return;
            else if (data < current->data) current = current->left;
            else current = current->right;
        }
        current = new tNode<T>(data);
        if (!parent) root = current;
        else if (data < parent->data) parent->left = current;
        else parent->right = current;
    }

    void remove(const T& data) {
        tNode<T>* current = root;
        tNode<T>* parent = nullptr;
        while (current) {
            parent = current;
            if (data == current->data) {
                if (!current->left && !current->right) {
                    if (parent->left == current) parent->left = nullptr;
                    else parent->right = nullptr;
                }
                else if (current->left && !current->right) {
                    if (parent->left == current) parent->left = current->left;
                    else parent->right = current->left;
                }
                else if (!current->left && current->right) {
                    if (parent->left == current) parent->left = current->right;
                    else parent->right = current->right;
                }
                else {
                    tNode<T>* successor = current->right;
                    while (successor->left) successor = successor->left;
                    current->data = successor->data;
                    current->right = successor->right;
                }
                delete current;
                return;
            }
            else if (data < current->data) current = current->left;
            else current = current->right;
        }
    }

    T lower_bound(const T& data) const {
        
    }

    T upper_bound(const T& data) const {
        
    }

    T getMin(){
        tNode<T>* current = root;
        while (current->left) current = current->left;
        return current->data;
    }

    T getMax(){
        tNode<T>* current = root;
        while (current->right) current = current->right;
        return current->data;
    }

    T getNext(){

    }

    T getPrev(){

    }


    template <typename VisitFunc = void(*)(const T&)>
    void Traverse(TraverseOrder order=TraverseOrder::PreOrder,
                VisitFunc visitFunc = [](const T& data){std::cout << data << " ";} 
                ) const {
        this->BiTree<T>::Traverse(order, 
                [visitFunc](const tNode<T>* node) {if (node) visitFunc(node->data); }
        );            
    }

};







#endif
