#ifndef BITREE_HPP
#define BITREE_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <utility>
#include <string>
#include <type_traits>

template <typename T, NodeType NT> class BST;

enum class TraverseOrder{
    PreOrder,
    InOrder,
    PostOrder,
    LevelOrder
};

enum class NodeType{
    DoubleLink,
    TripleLink
};


template <typename T>
class tNode {
public:
    T data;
    tNode *left, *right;
    static constexpr bool hasParent = false;
    tNode(const T& data) : data(data), left(nullptr), right(nullptr) {}
    tNode() : left(nullptr), right(nullptr) {}
    ~tNode() = default;
};

template <typename T>
class TripleNode {
public:
    T data;
    TripleNode *left, *right, *parent;
    static constexpr bool hasParent = true;
    TripleNode(const T& data) : data(data), left(nullptr), right(nullptr), parent(nullptr) {}
    TripleNode() : left(nullptr), right(nullptr), parent(nullptr) {}
    ~TripleNode() = default;
};


template<typename T, NodeType NT>
struct NodeSelector;

template <typename T>
struct NodeSelector<T, NodeType::DoubleLink> {
    using type = tNode<T>;
};

template <typename T>
struct NodeSelector<T, NodeType::TripleLink> {
    using type = TripleNode<T>;
};



template <typename T, NodeType NT> class BiTree;
template <typename U, NodeType UNT> void print(const BiTree<U, UNT>& tree);



template <typename T, NodeType NT = NodeType::DoubleLink>
class BiTree{

    template <typename U, NodeType UNT> friend class BST;

public:
    using Node = typename NodeSelector<T, NT>::type;
    
private:
    Node *root;

    void connect_left(Node* parent_node, Node* child_node) {
        if (parent_node) {
            parent_node->left = child_node;
        }
        if constexpr (Node::hasParent) {
            if (child_node) child_node->parent = parent_node;
        }
    }

    void connect_right(Node* parent_node, Node* child_node) {
        if (parent_node) {
            parent_node->right = child_node;
        }
        if constexpr (Node::hasParent) {
            if (child_node) child_node->parent = parent_node;
        }
    }

    Node* copyTree(const Node* node) {
        if(node){
            std::stack<std::pair<const Node*,Node*>> stack;
            const Node* current = nullptr;
            Node* newNode = new Node(node->data); 
            Node* newCurrent = nullptr;
            stack.emplace(node,newNode);
            while(!stack.empty()){
                current=stack.top().first;
                newCurrent=stack.top().second;
                stack.pop();
                if(current->right){
                    connect_right(newCurrent, new Node(current->right->data));
                    stack.emplace(current->right,newCurrent->right);
                }
                if(current->left){
                    connect_left(newCurrent, new Node(current->left->data));
                    stack.emplace(current->left,newCurrent->left);
                }
            }
            return newNode;
        }
        return nullptr;
    }

    static void printHelper(const Node* node, std::string& prefix, int isleft){
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
    static Node* PreFind(const Node* node, const T& data){
        if(node){
            std::stack<const Node*> stack;
            const Node* current; 
            stack.emplace(node);
            while(!stack.empty()){
                current=stack.top();
                stack.pop();
                if(current->data == data){
                    return const_cast<Node*>(current);
                }
                if(current->right) stack.emplace(current->right);
                if(current->left) stack.emplace(current->left);
            }
        }
        return nullptr;
    }

    static Node* InFind(const Node* node, const T& data){
        if(node){
            std::stack<const Node*> stack;
            const Node* current=node;
            while(current||!stack.empty()){
                while(current){
                    stack.emplace(current);
                    current=current->left;
                }
                current=stack.top();
                stack.pop();
                if(current->data == data){
                    return const_cast<Node*>(current);
                }
                current=current->right;
            }
        }
        return nullptr;
    }

    static Node* PostFind(const Node* node, const T& data){
        if(node){
            std::stack<const Node*> stack;
            const Node* current = node;
            const Node* prev = nullptr;
            const Node* topNode = nullptr;
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
                        return const_cast<Node*>(topNode);
                    }
                }else current = topNode->right;
            }
        }
        return nullptr;
    }

    static Node* LevelFind(const Node* node, const T& data){
        if(node){
            std::queue<const Node*> queue;
            const Node* current;
            queue.emplace(node);
            while(!queue.empty()){
                current=queue.front();
                if(current->data == data){
                    return const_cast<Node*>(current);
                }
                queue.pop();
                if(current->left) queue.emplace(current->left);
                if(current->right) queue.emplace(current->right);
            }
        }
        return nullptr;
    }


    // 遍历查找2
    static std::pair<Node*, Node*> PreFind2(const Node* node, const T& data){
        if(node){
            const Node* current = nullptr; 
            const Node* parent = nullptr;
            std::stack<std::pair<const Node*, const Node*>> stack;
            stack.emplace(node, nullptr);
            while(!stack.empty()){
                auto top = stack.top();
                current = top.first;
                parent = top.second;
                stack.pop();

                if(current->data == data){
                    return {const_cast<Node*>(current), const_cast<Node*>(parent)};
                }
                if(current->right) stack.emplace(current->right, current);
                if(current->left) stack.emplace(current->left, current);
            }
        }
        return {nullptr, nullptr};
    }

    static std::pair<Node*, Node*> InFind2(const Node* node, const T& data){
        if(node){
            const Node* current = nullptr; 
            const Node* parent = nullptr;
            std::stack<std::pair<const Node*, const Node*>> stack;
            current=node;
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
                    return {const_cast<Node*>(current), const_cast<Node*>(parent)};
                }
                parent=current;
                current=current->right;
            }
        }
        return {nullptr, nullptr};
    }

    static std::pair<Node*, Node*> PostFind2(const Node* node, const T& data){
        if(node){
            const Node* current = nullptr; 
            const Node* parent = nullptr;
            const Node* topNode = nullptr;
            std::stack<std::pair<const Node*, const Node*>> stack;
            current = node;
            const Node* prev = nullptr;
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
                        return {const_cast<Node*>(topNode), const_cast<Node*>(parent)};
                    }
                }else {
                    parent = topNode;
                    current = topNode->right;
                }
            }
        }
        return {nullptr, nullptr};
    }

    static std::pair<Node*, Node*> LevelFind2(const Node* node, const T& data){
        if(node){
            const Node* current = nullptr; 
            const Node* parent = nullptr;
            std::queue<std::pair<const Node*, const Node*>> queue;
            queue.emplace(node, nullptr);
            while(!queue.empty()){
                auto top = queue.front();
                current = top.first;
                parent = top.second;

                if(current->data == data){
                    return {const_cast<Node*>(current), const_cast<Node*>(parent)};
                }
                queue.pop();
                if(current->left) queue.emplace(current->left, current);
                if(current->right) queue.emplace(current->right, current);
            }
        }
        return {nullptr, nullptr};
    }


    // 遍历

    template <typename VisitFunc = void(*)(Node*)>
    static void PreTraverse(Node* node, VisitFunc visitFunc = [](Node* node){std::cout << node->data << " ";} ) {
        if(node){
            std::stack<Node*> stack;
            Node* current; 
            stack.emplace(node);
            while(!stack.empty()){
                current=stack.top();
                stack.pop();
                if(current->right) stack.emplace(current->right);
                if(current->left) stack.emplace(current->left);
                visitFunc(current);
            }
        }
    }

    template <typename VisitFunc = void(*)(Node*)>
    static void InTraverse(Node* node, VisitFunc visitFunc = [](Node* node){std::cout << node->data << " ";} ) {
        if(node){
            std::stack<Node*> stack;
            Node* current=node;
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

    template <typename VisitFunc = void(*)(Node*)>
    static void PostTraverse(Node* node, VisitFunc visitFunc = [](Node* node){std::cout << node->data << " ";} ) {
        std::stack<Node*> stack;
        Node* current = node;
        Node* prev = nullptr;
        while(current || !stack.empty()){
            while(current){
                stack.emplace(current);
                current = current->left;
            }
            Node* topNode = stack.top();
            if(topNode->right == nullptr || topNode->right == prev){
                prev = topNode;
                stack.pop();
                visitFunc(topNode);
            }else current = topNode->right;
        }
    }

    template <typename VisitFunc = void(*)(Node*)>
    static void LevelTraverse(Node* node, VisitFunc visitFunc = [](Node* node){std::cout << node->data << " ";} ) {
        if(node){
            std::queue<Node*> queue;
            Node* current;
            queue.emplace(node);
            while(!queue.empty()){
                current=queue.front();
                queue.pop();
                if(current->left) queue.emplace(current->left);
                if(current->right) queue.emplace(current->right);
                visitFunc(current);
            }
        }
    }


    template <typename VisitFunc = void(*)(const Node*)>
    static void PreTraverse_const(const Node* node, VisitFunc visitFunc = [](const Node* node){std::cout << node->data << " ";} ) {
        if(node){
            std::stack<const Node*> stack;
            const Node* current; 
            stack.emplace(node);
            while(!stack.empty()){
                current=stack.top();
                stack.pop();
                if(current->right) stack.emplace(current->right);
                if(current->left) stack.emplace(current->left);
                visitFunc(current);
            }
        }
    }

    template <typename VisitFunc = void(*)(const Node*)>
    static void InTraverse_const(const Node* node, VisitFunc visitFunc = [](const Node* node){std::cout << node->data << " ";} ) {
        if(node){
            std::stack<const Node*> stack;
            const Node* current=node;
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

    template <typename VisitFunc = void(*)(const Node*)>
    static void PostTraverse_const(const Node* node, VisitFunc visitFunc = [](const Node* node){std::cout << node->data << " ";} ) {
        std::stack<const Node*> stack;
        const Node* current = node;
        const Node* prev = nullptr;
        while(current || !stack.empty()){
            while(current){
                stack.emplace(current);
                current = current->left;
            }
            const Node* topNode = stack.top();
            if(topNode->right == nullptr || topNode->right == prev){
                prev = topNode;
                stack.pop();
                visitFunc(topNode);
            }else current = topNode->right;
        }
    }

    template <typename VisitFunc = void(*)(const Node*)>
    static void LevelTraverse_const(const Node* node, VisitFunc visitFunc = [](const Node* node){std::cout << node->data << " ";} ) {
        if(node){
            std::queue<const Node*> queue;
            const Node* current;
            queue.emplace(node);
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

    BiTree(Node* root) : root(root) {
        if constexpr(Node::hasParent) {
            if(root) root->parent = nullptr;
        }
    }
    
    BiTree(const std::vector<T>& levelOrder,const T& emptyValue) : root(nullptr) {
        if(levelOrder.empty()) return;
        if(levelOrder[0] == emptyValue) return;
        root = new Node(levelOrder[0]);
        std::queue<Node*> queue;
        queue.emplace(root);
        size_t i=1;
        while(i<levelOrder.size() && !queue.empty()){
            Node* current = queue.front();
            queue.pop();
            if(levelOrder[i] != emptyValue){
                connect_left(current, new Node(levelOrder[i]));
                queue.emplace(current->left);
            }
            i++;
            if(i<levelOrder.size() && levelOrder[i] != emptyValue){
                connect_right(current, new Node(levelOrder[i]));
                queue.emplace(current->right);
            }
            i++;
        }
    }

    BiTree(const BiTree<T, NT>& other) : root(nullptr) {
        if (other.root) {
            root = copyTree(static_cast<const Node*>(other.root));
        }
    }

    BiTree(BiTree<T, NT>&& other) noexcept : root(nullptr) {
        root = other.root;
        other.root = nullptr;
    }

    ~BiTree(){
        clear(root);
        root = nullptr;
    }

    BiTree<T, NT>& operator=(const BiTree<T, NT>& other){
        if(this != &other){         
            BiTree<T, NT> temp(other);
            std::swap(root, temp.root);
        }
        return *this;
    }

    BiTree<T, NT>& operator=(BiTree<T, NT>&& other) noexcept{
        if(this != &other){
            std::swap(root, other.root);
        }
        return *this;
    }

    bool operator==(const BiTree<T, NT>& other) const {
        if (this == &other) return true;
        if (!root && !other.root) return true;

        if (root && other.root) {
            std::stack<std::pair<Node*,Node*>> stack;
            Node* current;
            Node* current_other;
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

    bool operator!=(const BiTree<T, NT>& other) const {return !(*this == other);}

    static void clear(Node* node) { // 遍历删除节点
        std::stack<Node*> stack;
        Node* current = node;
        Node* prev = nullptr;
        while(current || !stack.empty()){
            while(current){
                stack.emplace(current);
                current = current->left;
            }
            Node* topNode = stack.top();
            if(topNode->right == nullptr || topNode->right == prev){
                prev = topNode;
                stack.pop();
                delete topNode;
            }else current = topNode->right;
        }
    }

    Node* getRoot() const { return root; }

    void setRoot(Node* newRoot) {
        if (!newRoot) {
            this->root = nullptr;
        } else{
            this->root = newRoot; 
            if constexpr(Node::hasParent) {
                this->root->parent = nullptr;
            }
        }
    }

    template <typename VisitFunc = void(*)(Node*)>
    static void Traverse(Node* node,
                        TraverseOrder order=TraverseOrder::PreOrder,
                        VisitFunc visitFunc = [](Node* node){std::cout << node->data << " ";} 
                        ) {
        switch(order){
            case TraverseOrder::PreOrder:
                PreTraverse(node, visitFunc);
                break;
            case TraverseOrder::InOrder:
                InTraverse(node, visitFunc);
                break;
            case TraverseOrder::PostOrder:
                PostTraverse(node, visitFunc);
                break;
            case TraverseOrder::LevelOrder:
                LevelTraverse(node, visitFunc);
                break;
        }
    }

    template <typename VisitFunc = void(*)(Node*)>
    void Traverse(TraverseOrder order=TraverseOrder::PreOrder,
                VisitFunc visitFunc = [](Node* node){std::cout << node->data << " ";} 
                ) {
        Traverse(root, order, visitFunc);
    }

    template <typename VisitFunc = void(*)(const Node*)>
    static void Traverse(const Node* node,
                        TraverseOrder order=TraverseOrder::PreOrder,
                        VisitFunc visitFunc = [](const Node* node){std::cout << node->data << " ";} 
                        ) {
        switch(order){
            case TraverseOrder::PreOrder:
                PreTraverse_const(static_cast<const Node*>(node), visitFunc);
                break;
            case TraverseOrder::InOrder:
                InTraverse_const(static_cast<const Node*>(node), visitFunc);
                break;
            case TraverseOrder::PostOrder:
                PostTraverse_const(static_cast<const Node*>(node), visitFunc);
                break;
            case TraverseOrder::LevelOrder:
                LevelTraverse_const(static_cast<const Node*>(node), visitFunc);
                break;
        }
    }

    template <typename VisitFunc = void(*)(const Node*)>
    void Traverse(TraverseOrder order=TraverseOrder::PreOrder,
                VisitFunc visitFunc = [](const Node* node){std::cout << node->data << " ";} 
                ) const {
        Traverse(static_cast<const Node*>(root), order, visitFunc);
    }


    static size_t countNodes(const Node* node) {
        size_t count = 0;
        if(node){
            std::stack<const Node*> stack;
            const Node* current; 
            stack.emplace(node);
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

    size_t countNodes() const { return countNodes(static_cast<const Node*>(root)); }

    static size_t countLeaves(const Node* node) {
        size_t count = 0;
        if(node){
            std::stack<const Node*> stack;
            const Node* current; 
            stack.emplace(node);
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

    size_t countLeaves() const { return countLeaves(static_cast<const Node*>(root)); }

    static size_t getHeight(const Node* node) {
        size_t max_height = 0;
        if(node){
            const Node* current; 
            size_t height;
            std::stack<std::pair<const Node*, size_t>> stack;
            stack.emplace(node, 1);
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

    size_t getHeight() const { return getHeight(static_cast<const Node*>(root)); }

    static bool isEmpty(const Node* node) { return node == nullptr; }

    bool isEmpty() const { return root == nullptr; }

    static Node* find(const Node* node, const T& data, TraverseOrder order = TraverseOrder::PreOrder){
        switch(order){
            case TraverseOrder::PreOrder:
                return PreFind(static_cast<const Node*>(node), data);
            case TraverseOrder::InOrder:
                return InFind(static_cast<const Node*>(node), data);
            case TraverseOrder::PostOrder:
                return PostFind(static_cast<const Node*>(node), data);
            case TraverseOrder::LevelOrder:
                return LevelFind(static_cast<const Node*>(node), data);
        }
        return nullptr;
    }

    Node* find(const T& data, TraverseOrder order = TraverseOrder::PreOrder) const {
        return find(static_cast<const Node*>(root), data, order);
    }

    static std::pair<Node*, Node*> locate(const Node* node, const T& data, TraverseOrder order = TraverseOrder::PreOrder) {
        switch(order){
            case TraverseOrder::PreOrder:
                return PreFind2(static_cast<const Node*>(node), data);
            case TraverseOrder::InOrder:
                return InFind2(static_cast<const Node*>(node), data);
            case TraverseOrder::PostOrder:
                return PostFind2(static_cast<const Node*>(node), data);
            case TraverseOrder::LevelOrder:
                return LevelFind2(static_cast<const Node*>(node), data);
        }
        return {nullptr, nullptr};
    }

    std::pair<Node*, Node*> locate(const T& data, TraverseOrder order = TraverseOrder::PreOrder) const {
        return locate(static_cast<const Node*>(root), data, order);
    }

    // 查找目标节点的父节点，三叉链表时屏蔽
    template <typename N = Node, typename = std::enable_if_t<!N::hasParent>>
    static Node* parent_of(const Node* node, const Node* target) {
        if (!node || !target || node == target) return nullptr;
        std::stack<const Node*> stack;
        stack.emplace(node);

        while(!stack.empty()){
            const Node* current = stack.top();
            stack.pop();

            if (current->left == target || current->right == target) {
                return const_cast<Node*>(current);
            }
            
            if (current->right) stack.emplace(current->right);
            if (current->left) stack.emplace(current->left);
        }
        return nullptr;
    }

    template <typename N = Node, typename = std::enable_if_t<!N::hasParent>>
    Node* parent_of(const Node* target) const {
        return parent_of(static_cast<const Node*>(root), target);
    }


    template <typename U, NodeType UNT>
    friend void print(const BiTree<U, UNT>& tree);

};

template <typename U, NodeType UNT>
inline void print(const BiTree<U, UNT>& tree){
    if(tree.root){
        std::string prefix = "";
        BiTree<U,UNT>::printHelper(tree.root, prefix, 0);
    }
}


#endif