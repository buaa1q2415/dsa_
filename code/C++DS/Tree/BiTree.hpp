#ifndef BITREE_HPP
#define BITREE_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <utility>
#include <string>


enum class TraverseOrder{
    PreOrder,
    InOrder,
    PostOrder,
    LevelOrder
};


template <typename T>
class tNode{
public:
    T data;
    tNode<T> *left,*right;
    tNode(const T& data):data(data),left(nullptr),right(nullptr){}
    tNode():left(nullptr),right(nullptr){}
    ~tNode()=default;
};



template <typename T>
class BiTree{
protected:
    tNode<T> *root;

    static void clear(tNode<T>* node) { // 遍历删除节点
        std::stack<tNode<T>*> stack;
        tNode<T>* current = node;
        tNode<T>* prev = nullptr;
        while(current || !stack.empty()){
            while(current){
                stack.emplace(current);
                current = current->left;
            }
            tNode<T>* topNode = stack.top();
            if(topNode->right == nullptr || topNode->right == prev){
                prev = topNode;
                stack.pop();
                delete topNode;
            }else current = topNode->right;
        }
    }

    tNode<T>* copyTree(tNode<T>* node) {
        if(node){
            std::stack<std::pair<tNode<T>*,tNode<T>*>> stack;
            tNode<T>* current = nullptr;
            tNode<T>* newNode = new tNode<T>(node->data); 
            tNode<T>* newCurrent = nullptr;
            stack.emplace(node,newNode);
            while(!stack.empty()){
                current=stack.top().first;
                newCurrent=stack.top().second;
                stack.pop();
                if(current->right){
                    newCurrent->right = new tNode<T>(current->right->data);
                    stack.emplace(current->right,newCurrent->right);
                }
                if(current->left){
                    newCurrent->left = new tNode<T>(current->left->data);
                    stack.emplace(current->left,newCurrent->left);
                }
            }
            return newNode;
        }
        return nullptr;
    }

    static void printHelper(const tNode<T>* node, std::string& prefix, int isleft){
        if(node != nullptr){
            std::cout << prefix;
            if(isleft==1) std::cout<<"├──" ;
            else if(isleft==-1) std::cout<<"└──";
            std::cout << node->data << std::endl;

            std::string c;
            if(isleft==1) c="│    ";
            else if(isleft==-1) c="    ";
            else c="  ";

            prefix += c;

            printHelper(node->left,prefix,1);
            printHelper(node->right,prefix,-1);

            prefix.resize(prefix.length() - c.length());
        }
    }


    // 遍历查找
    tNode<T>* PreFind(const T& data) const{
        if(root){
            std::stack<tNode<T>*> stack;
            tNode<T>* current; 
            stack.emplace(root);
            while(!stack.empty()){
                current=stack.top();
                stack.pop();
                if(current->data == data){
                    return current;
                }
                if(current->right) stack.emplace(current->right);
                if(current->left) stack.emplace(current->left);
                
            }
        }
        return nullptr;
    }

    tNode<T>* InFind(const T& data) const{
        if(root){
            std::stack<tNode<T>*> stack;
            tNode<T>* current=root;
            while(current||!stack.empty()){
                while(current){
                    stack.emplace(current);
                    current=current->left;
                }
                current=stack.top();
                stack.pop();
                if(current->data == data){
                    return current;
                }
                current=current->right;
            }
        }
        return nullptr;
    }

    tNode<T>* PostFind(const T& data) const{
        if(root){
            std::stack<tNode<T>*> stack;
            tNode<T>* current = root;
            tNode<T>* prev = nullptr;
            tNode<T>* topNode = nullptr;
            while(current || !stack.empty()){
                while(current){
                    stack.emplace(current);
                    current = current->left;
                }
                topNode = stack.top();
                
                if(topNode->right == nullptr || topNode->right == prev){
                    prev = topNode;
                    stack.pop();
                    if(topNode->data == data){
                        return topNode;
                    }
                }else current = topNode->right;
            }
        }
        return nullptr;
    }

    tNode<T>* LevelFind(const T& data) const{
        if(root){
            std::queue<tNode<T>*> queue;
            tNode<T>* current;
            queue.emplace(root);
            while(!queue.empty()){
                current=queue.front();
                if(current->data == data){
                    return current;
                }
                queue.pop();
                if(current->left) queue.emplace(current->left);
                if(current->right) queue.emplace(current->right);
            }
        }
        return nullptr;
    }


    // 遍历查找2
    std::pair<tNode<T>*, tNode<T>*> PreFind2(const T& data) const{
        if(root){
            tNode<T>* current = nullptr; 
            tNode<T>* parent = nullptr;
            std::stack<std::pair<tNode<T>*, tNode<T>*>> stack;
            stack.emplace(root, nullptr);
            while(!stack.empty()){
                auto top = stack.top();
                current = top.first;
                parent = top.second;
                stack.pop();

                if(current->data == data){
                    return {current, parent};
                }
                if(current->right) stack.emplace(current->right, current);
                if(current->left) stack.emplace(current->left, current);
            }
        }
        return {nullptr, nullptr};
    }

    std::pair<tNode<T>*, tNode<T>*> InFind2(const T& data) const{
        if(root){
            tNode<T>* current = nullptr; 
            tNode<T>* parent = nullptr;
            std::stack<std::pair<tNode<T>*, tNode<T>*>> stack;
            current=root;
            while(current||!stack.empty()){
                while(current){
                    stack.emplace(current, parent);
                    parent=current;
                    current=current->left;
                }

                auto top = stack.top();
                current = top.first;
                parent = top.second;
                stack.pop();

                if(current->data == data){
                    return {current, parent};
                }
                parent=current;
                current=current->right;
            }
        }
        return {nullptr, nullptr};
    }

    std::pair<tNode<T>*, tNode<T>*> PostFind2(const T& data) const{
        if(root){
            tNode<T>* current = nullptr; 
            tNode<T>* parent = nullptr;
            tNode<T>* topNode = nullptr;
            std::stack<std::pair<tNode<T>*, tNode<T>*>> stack;
            current = root;
            tNode<T>* prev = nullptr;
            while(current || !stack.empty()){
                while(current){
                    stack.emplace(current, parent);
                    parent=current;
                    current = current->left;
                }
                auto top = stack.top();
                topNode = top.first;
                parent = top.second;

                if(topNode->right == nullptr || topNode->right == prev){
                    stack.pop();
                    prev = topNode;
                    if(topNode->data == data){
                        return {topNode, parent};
                    }
                }else {
                    parent = topNode;
                    current = topNode->right;
                }
            }
        }
        return {nullptr, nullptr};
    }

    std::pair<tNode<T>*, tNode<T>*> LevelFind2(const T& data) const{
        if(root){
            tNode<T>* current = nullptr; 
            tNode<T>* parent = nullptr;
            std::queue<std::pair<tNode<T>*, tNode<T>*>> queue;
            queue.emplace(root, nullptr);
            while(!queue.empty()){
                auto top = queue.front();
                current = top.first;
                parent = top.second;

                if(current->data == data){
                    return {current, parent};
                }
                queue.pop();
                if(current->left) queue.emplace(current->left, current);
                if(current->right) queue.emplace(current->right, current);
            }
        }
        return {nullptr, nullptr};
    }


    // 遍历

    template <typename VisitFunc = void(*)(tNode<T>*)>
    void PreTraverse(VisitFunc visitFunc = [](tNode<T>* node){std::cout << node->data << " ";} ) {
        if(root){
            std::stack<tNode<T>*> stack;
            tNode<T>* current; 
            stack.emplace(root);
            while(!stack.empty()){
                current=stack.top();
                stack.pop();
                if(current->right) stack.emplace(current->right);
                if(current->left) stack.emplace(current->left);
                visitFunc(current);
            }
        }
    }

    template <typename VisitFunc = void(*)(tNode<T>*)>
    void InTraverse(VisitFunc visitFunc = [](tNode<T>* node){std::cout << node->data << " ";} ) {
        if(root){
            std::stack<tNode<T>*> stack;
            tNode<T>* current=root;
            while(current||!stack.empty()){
                while(current){
                    stack.emplace(current);
                    current=current->left;
                }
                current=stack.top();
                stack.pop();
                visitFunc(current);
                current=current->right;
            }
        }
    }

    template <typename VisitFunc = void(*)(tNode<T>*)>
    void PostTraverse(VisitFunc visitFunc = [](tNode<T>* node){std::cout << node->data << " ";} ) {
        std::stack<tNode<T>*> stack;
        tNode<T>* current = root;
        tNode<T>* prev = nullptr;
        while(current || !stack.empty()){
            while(current){
                stack.emplace(current);
                current = current->left;
            }
            tNode<T>* topNode = stack.top();
            if(topNode->right == nullptr || topNode->right == prev){
                prev = topNode;
                stack.pop();
                visitFunc(topNode);
            }else current = topNode->right;
        }
    }

    template <typename VisitFunc = void(*)(tNode<T>*)>
    void LevelTraverse(VisitFunc visitFunc = [](tNode<T>* node){std::cout << node->data << " ";} ) {
        if(root){
            std::queue<tNode<T>*> queue;
            tNode<T>* current;
            queue.emplace(root);
            while(!queue.empty()){
                current=queue.front();
                queue.pop();
                if(current->left) queue.emplace(current->left);
                if(current->right) queue.emplace(current->right);
                visitFunc(current);
            }
        }
    }


    template <typename VisitFunc = void(*)(const tNode<T>*)>
    void PreTraverse_const(VisitFunc visitFunc = [](const tNode<T>* node){std::cout << node->data << " ";} ) const {
        if(root){
            std::stack<const tNode<T>*> stack;
            const tNode<T>* current; 
            stack.emplace(root);
            while(!stack.empty()){
                current=stack.top();
                stack.pop();
                if(current->right) stack.emplace(current->right);
                if(current->left) stack.emplace(current->left);
                visitFunc(current);
            }
        }
    }

    template <typename VisitFunc = void(*)(const tNode<T>*)>
    void InTraverse_const(VisitFunc visitFunc = [](const tNode<T>* node){std::cout << node->data << " ";} ) const {
        if(root){
            std::stack<const tNode<T>*> stack;
            const tNode<T>* current=root;
            while(current||!stack.empty()){
                while(current){
                    stack.emplace(current);
                    current=current->left;
                }
                current=stack.top();
                stack.pop();
                visitFunc(current);
                current=current->right;
            }
        }
    }

    template <typename VisitFunc = void(*)(const tNode<T>*)>
    void PostTraverse_const(VisitFunc visitFunc = [](const tNode<T>* node){std::cout << node->data << " ";} ) const {
        std::stack<const tNode<T>*> stack;
        const tNode<T>* current = root;
        const tNode<T>* prev = nullptr;
        while(current || !stack.empty()){
            while(current){
                stack.emplace(current);
                current = current->left;
            }
            const tNode<T>* topNode = stack.top();
            if(topNode->right == nullptr || topNode->right == prev){
                prev = topNode;
                stack.pop();
                visitFunc(topNode);
            }else current = topNode->right;
        }
    }

    template <typename VisitFunc = void(*)(const tNode<T>*)>
    void LevelTraverse_const(VisitFunc visitFunc = [](const tNode<T>* node){std::cout << node->data << " ";} ) const {
        if(root){
            std::queue<const tNode<T>*> queue;
            const tNode<T>* current;
            queue.emplace(root);
            while(!queue.empty()){
                current=queue.front();
                queue.pop();
                if(current->left) queue.emplace(current->left);
                if(current->right) queue.emplace(current->right);
                visitFunc(current);
            }
        }
    }


public:
    BiTree():root(nullptr){}

    BiTree(tNode<T>* root) : root(root) {}
    
    BiTree(const std::vector<T>& levelOrder,const T& emptyValue) : root(nullptr) {
        if(levelOrder.empty()) return;
        if(levelOrder[0] == emptyValue) return;
        root = new tNode<T>(levelOrder[0]);
        std::queue<tNode<T>*> queue;
        queue.emplace(root);
        size_t i=1;
        while(i<levelOrder.size() && !queue.empty()){
            tNode<T>* current = queue.front();
            queue.pop();
            if(levelOrder[i] != emptyValue){
                current->left = new tNode<T>(levelOrder[i]);
                queue.emplace(current->left);
            }
            i++;
            if(i<levelOrder.size() && levelOrder[i] != emptyValue){
                current->right = new tNode<T>(levelOrder[i]);
                queue.emplace(current->right);
            }
            i++;
        }
    }

    BiTree(const BiTree<T>& other) : root(nullptr) {
        if (other.root) {
            root = copyTree(other.root);
        }
    }

    BiTree(BiTree<T>&& other) noexcept : root(nullptr) {
        root = other.root;
        other.root = nullptr;
    }

    virtual ~BiTree(){
        clear(root);
        root = nullptr;
    }

    BiTree<T>& operator=(const BiTree<T>& other){
        if(this != &other){         
            BiTree<T> temp(other);
            std::swap(root, temp.root);
        }
        return *this;
    }

    BiTree<T>& operator=(BiTree<T>&& other) noexcept{
        if(this != &other){
            std::swap(root, other.root);
        }
        return *this;
    }

    bool operator==(const BiTree<T>& other) const {
        if (this == &other) return true;
        if (!root && !other.root) return true;

        if (root && other.root) {
            std::stack<std::pair<tNode<T>*,tNode<T>*>> stack;
            tNode<T>* current;
            tNode<T>* current_other;
            stack.emplace(root,other.root);
            while(!stack.empty()) {
                auto top = stack.top();
                current = top.first;
                current_other = top.second;

                stack.pop();

                if(current->data != current_other->data) return false;
                if(current->right && current_other->right) stack.emplace(current->right,current_other->right);
                else if(current->right || current_other->right) return false;
                if(current->left && current_other->left) stack.emplace(current->left,current_other->left);
                else if(current->left || current_other->left) return false;
                
            }
        }
        else return false;
        return true;
    }

    bool operator!=(const BiTree<T>& other) const {return !(*this == other);}

    tNode<T>* getRoot() const { return root; }

    void setRoot(tNode<T>* newRoot) { 
        if (this->root == newRoot) return;
        clear(this->root);
        this->root = newRoot; 
    }

    template <typename VisitFunc = void(*)(tNode<T>*)>
    void Traverse(TraverseOrder order=TraverseOrder::PreOrder,
                VisitFunc visitFunc = [](tNode<T>* node){std::cout << node->data << " ";} 
                ) {
        switch(order){
            case TraverseOrder::PreOrder:
                PreTraverse(visitFunc);
                break;
            case TraverseOrder::InOrder:
                InTraverse(visitFunc);
                break;
            case TraverseOrder::PostOrder:
                PostTraverse(visitFunc);
                break;
            case TraverseOrder::LevelOrder:
                LevelTraverse(visitFunc);
                break;
        }
    }

    template <typename VisitFunc = void(*)(const tNode<T>*)>
    void Traverse(TraverseOrder order=TraverseOrder::PreOrder,
                        VisitFunc visitFunc = [](const tNode<T>* node){std::cout << node->data << " ";} 
                        ) const {
        switch(order){
            case TraverseOrder::PreOrder:
                PreTraverse_const(visitFunc);
                break;
            case TraverseOrder::InOrder:
                InTraverse_const(visitFunc);
                break;
            case TraverseOrder::PostOrder:
                PostTraverse_const(visitFunc);
                break;
            case TraverseOrder::LevelOrder:
                LevelTraverse_const(visitFunc);
                break;
        }
    }


    size_t countNodes() const {
        size_t count = 0;
        if(root){
            std::stack<tNode<T>*> stack;
            tNode<T>* current; 
            stack.emplace(root);
            while(!stack.empty()){
                current=stack.top();
                stack.pop();
                if(current->right) stack.emplace(current->right);
                if(current->left) stack.emplace(current->left);
                ++count;
            }
        }
        return count;
    }

    size_t countLeaves() const {
        size_t count = 0;
        if(root){
            std::stack<tNode<T>*> stack;
            tNode<T>* current; 
            stack.emplace(root);
            while(!stack.empty()){
                current=stack.top();
                stack.pop();
                if(current->right) stack.emplace(current->right);
                if(current->left) stack.emplace(current->left);
                if(!current->left && !current->right) ++count;
            }
        }
        return count;
    }

    size_t getHeight() const {
        size_t max_height = 0;
        if(root){
            tNode<T>* current; 
            size_t height;
            std::stack<std::pair<tNode<T>*, size_t>> stack;
            stack.emplace(root, 1);
            while(!stack.empty()){
                auto top = stack.top();
                current = top.first;
                height = top.second;
                stack.pop();
                
                max_height = std::max(max_height, height);
                if(current->right) stack.emplace(current->right, height+1);
                if(current->left) stack.emplace(current->left, height+1);
            }
        }
        return max_height;
    }

    bool isEmpty() const { return root == nullptr; }

    tNode<T>* find(const T& data, TraverseOrder order = TraverseOrder::PreOrder) const {
        switch(order){
            case TraverseOrder::PreOrder:
                return PreFind(data);
            case TraverseOrder::InOrder:
                return InFind(data);
            case TraverseOrder::PostOrder:
                return PostFind(data);
            case TraverseOrder::LevelOrder:
                return LevelFind(data);
        }
        return nullptr;
    }

    std::pair<tNode<T>*, tNode<T>*> locate(const T& data, TraverseOrder order = TraverseOrder::PreOrder) const {
        switch(order){
            case TraverseOrder::PreOrder:
                return PreFind2(data);
            case TraverseOrder::InOrder:
                return InFind2(data);
            case TraverseOrder::PostOrder:
                return PostFind2(data);
            case TraverseOrder::LevelOrder:
                return LevelFind2(data);
        }
        return {nullptr, nullptr};
    }

    tNode<T>* parent_of(const tNode<T>* target) const {
        if (!root || !target || root == target) return nullptr;

        std::stack<tNode<T>*> stack;
        stack.emplace(root);

        while(!stack.empty()){
            tNode<T>* current = stack.top();
            stack.pop();

            if (current->left == target || current->right == target) {
                return current;
            }
            
            if (current->right) stack.emplace(current->right);
            if (current->left) stack.emplace(current->left);
        }
        return nullptr;
    }


    template <typename U>
    friend void print(const BiTree<U>& tree);

};

template <typename U>
inline void print(const BiTree<U>& tree){
    if(tree.root){
        std::string prefix = "";
        BiTree<U>::printHelper(tree.root, prefix, 0);
    }
}


#endif