#ifndef BST_HPP
#define BST_HPP


#include "BiTree.hpp"
#include <initializer_list>
#include <iterator>

constexpr NodeType AVLLink = static_cast<NodeType>(2);
constexpr NodeType RBTLink = static_cast<NodeType>(3);

template<typename T>
struct NodeSelector<T, AVLLink> {
    using type = AVLNode<T>;
};

template<typename T>
struct NodeSelector<T, RBTLink> {
    using type = RBTNode<T>;
};


template <typename T>
class AVLNode {
    template<typename U> friend class AVLTree;
private:
    T data;
    size_t height;
    AVLNode *left, *right, *parent;
    static constexpr bool hasParent = true;
    static constexpr bool hasHeight = true;
    static constexpr bool hasColor = false;

    AVLNode(const T& data) : data(data), height(1), left(nullptr), right(nullptr), parent(nullptr) {}
    AVLNode() : height(1), left(nullptr), right(nullptr), parent(nullptr) {}
    ~AVLNode() = default;
};


template <typename T>
class RBTNode {
private:
    T data;
    bool color; // false: black, true: red
    RBTNode *left, *right, *parent;

    static constexpr bool hasParent = true;
    static constexpr bool hasHeight = false;
    static constexpr bool hasColor = true;

    RBTNode(const T& data) : data(data), color(true), left(nullptr), right(nullptr), parent(nullptr) {}
    RBTNode() : color(true), left(nullptr), right(nullptr), parent(nullptr) {}
    ~RBTNode() = default;
};




template <typename T, NodeType NT> class BST;
template <typename T> class AVLTree;
template <typename T> class RBTree;
template <typename U, NodeType UNT> void print(const BST<U, UNT>& bst);
template <typename U> void print(const AVLTree<U>& avl);
template <typename U> void print(const RBTree<U>& rbt);


template <typename T, NodeType NT = NodeType::TripleLink>
class BST {
protected:
    BiTree<T, NT> tree;

    using Node = typename NodeSelector<T, NT>::type;

protected:

    static void connect(Node* parent_node, Node* child_node) {
        if (parent_node && child_node) {
            if (child_node->data > parent_node->data){
                parent_node->right = child_node;
            } else {
                parent_node->left = child_node;
            }
        }
        child_node->parent = parent_node;
    }

    Node* BST_from_vec_helper(std::vector<T>& vec,int l,int r){
        if(l>r) return nullptr;
        int mid=l+(r-l)/2;
        Node* node = new Node(vec[mid]);
        tree.connect_left(node, BST_from_vec_helper(vec,l,mid-1));
        tree.connect_right(node, BST_from_vec_helper(vec,mid+1,r));
        return node;
    }

    Node* removeHelper(Node* node, const T& data) {
        if (!node) return nullptr;
        if (data < node->data) tree.connect_left(node, removeHelper(node->left, data));
        else if (data > node->data) tree.connect_right(node, removeHelper(node->right, data));
        else {
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            }
            Node* successor = node->right;
            while (successor->left) successor = successor->left;
            node->data = successor->data;
            tree.connect_right(node, removeHelper(node->right, successor->data));
        }
        return node;
    }

    Node* insertHelper(Node* node, const T& data) {
        Node* current = node;
        Node* parent = nullptr;
        while (current) {
            parent = current;
            if (data < current->data) current = current->left;
            else if (data > current->data) current = current->right;
            else return nullptr;
        }
        current = new Node(data);
        if (!parent) return current; // 插入为根节点
        connect(parent, current);
        return current;
    }

public:
    // 语义

    BST() : tree() {
        static_assert(Node::hasParent, "Only supports nodes with parent pointer !");
    }

    BST(const BST<T, NT>& other) : tree(other.tree) {
        static_assert(Node::hasParent, "Only supports nodes with parent pointer !");
    }

    BST(BST<T, NT>&& other) noexcept : tree(std::move(other.tree)) {
        static_assert(Node::hasParent, "Only supports nodes with parent pointer !");
    }

    BST(std::initializer_list<T> list) : tree() {
        static_assert(Node::hasParent, "Only supports nodes with parent pointer !");
        for (const auto& item : list) {
            this->insert(item);
        }
    }

    BST(const std::vector<T>& vec) : tree() {
        static_assert(Node::hasParent, "Only supports nodes with parent pointer !");
        std::vector<T> sorted_vec = vec;
        if(!std::is_sorted(sorted_vec.begin(), sorted_vec.end())){
            std::sort(sorted_vec.begin(), sorted_vec.end());
        }
        sorted_vec.erase(std::unique(sorted_vec.begin(), sorted_vec.end()), sorted_vec.end());  // 去重
        tree.root = BST_from_vec_helper(sorted_vec,0,sorted_vec.size()-1);
    }

    virtual ~BST() = default;

public:

    BST<T, NT>& operator=(const BST<T, NT>& other) {
        if (this != &other) {
            tree = other.tree;
        }
        return *this;
    }

    BST& operator=(BST&& other) noexcept {
        if (this != &other){
            tree = std::move(other.tree);
        }
        return *this;
    }

    size_t getHeight() const { return tree.getHeight(); }

    size_t countNodes() const { return tree.countNodes(); }

    size_t countLeaves() const { return tree.countLeaves(); }

    bool isEmpty() const { return tree.isEmpty(); }

    bool operator==(const BST<T, NT>& other) const { return tree == other.tree; }

    bool operator!=(const BST<T, NT>& other) const { return !(*this == other); }

    template <typename U, NodeType UNT>
    friend void print(const BST<U, UNT>& bst);



public:
    class ConstIterator {

    friend class BST<T, NT>;

    private:
        const BiTree<T, NT>* tree;
        Node* current;

        ConstIterator(const BiTree<T, NT>* tree, Node* current=nullptr) : tree(tree), current(current) {}

        static Node* moveLeft(Node* node) {
            if (!node) return nullptr;
            while (node->left) {
                node = node->left;
            }
            return node;
        }

        static Node* moveRight(Node* node) {
            if (!node) return nullptr;
            while (node->right) {
                node = node->right;
            }
            return node;
        }

    public:
        // 声明为双向迭代器
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const T*;
        using reference         = const T&;

        reference operator*() const { return current->data; }
        pointer operator->() const { return &(current->data); }

        ConstIterator& operator++() {
            if (!current) return *this;
            if (current->right) {
                current = moveLeft(current->right);
            } else {
                Node* p = current->parent;
                while (p && current == p->right) {
                    current = p;
                    p = p->parent;
                }
                current = p;
            }
            return *this;
        }

        ConstIterator& operator--() {
            if (!current){
                current = moveRight(tree->getRoot());
                return *this;
            }
            if (current->left) {
                current = moveRight(current->left);
            } else {
                Node* p = current->parent;
                while (p && current == p->left) {
                    current = p;
                    p = p->parent;
                }
                current = p;
            }
            return *this;
        }

        ConstIterator operator++(int) { ConstIterator tmp = *this; ++(*this); return tmp; }
        ConstIterator operator--(int) { ConstIterator tmp = *this; --(*this); return tmp; }

        bool operator==(const ConstIterator& other) const { return current == other.current && tree == other.tree; }
        bool operator!=(const ConstIterator& other) const { return !(*this == other); }
    };


    ConstIterator find(const T& data) const {
        Node* current = tree.root;
        while (current) {
            if (data == current->data){
                return ConstIterator(&tree, current);
            } else if (data < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return ConstIterator(&tree);
    }

    virtual void insert(const T& data) {
        Node* newNode = insertHelper(tree.root, data);
        if (newNode && !newNode->parent) {
            tree.root = newNode;
        }
    }

    virtual void remove(const T& data) {
        tree.root = removeHelper(tree.root, data);
        if (tree.root) tree.root->parent = nullptr;
    }

    ConstIterator lower_bound(const T& data) const {
        Node* current = tree.root;
        Node* lower = nullptr;
        while (current) {
            if (data == current->data){
                return ConstIterator(&tree, current);
            }
            else if (data < current->data) {
                lower = current;
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        return lower ? ConstIterator(&tree, lower) : ConstIterator(&tree);
    }

    ConstIterator upper_bound(const T& data) const {
        Node* current = tree.root;
        Node* upper = nullptr;
        while (current) {
            if (data < current->data) {
                upper = current;
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        return upper ? ConstIterator(&tree, upper) : ConstIterator(&tree);
    }

    ConstIterator getMin() const {
        return ConstIterator(&tree, ConstIterator::moveLeft(tree.root));
    }

    ConstIterator getMax() const {
        return ConstIterator(&tree, ConstIterator::moveRight(tree.root));
    }

    template <typename VisitFunc = void(*)(const T&)>
    void Traverse(TraverseOrder order=TraverseOrder::PreOrder,
                VisitFunc visitFunc = [](const T& data){std::cout << data << " ";} 
                ) const {
        tree.Traverse(order, 
                [visitFunc](const Node* node) {if (node) visitFunc(node->data); }
        );            
    }


    ConstIterator begin() const { return ConstIterator(&tree, ConstIterator::moveLeft(tree.root)); }
    ConstIterator end() const { return ConstIterator(&tree); }


    // 反向迭代器适配
    std::reverse_iterator<ConstIterator> rbegin() const { 
        return std::reverse_iterator<ConstIterator>(end()); 
    }
    std::reverse_iterator<ConstIterator> rend() const { 
        return std::reverse_iterator<ConstIterator>(begin()); 
    }

};



template <typename T>
class AVLTree : public BST<T, AVLLink> {
private:
    using Node = AVLNode<T>;
    using BST = BST<T, AVLLink>;

    using BST::tree;

public:
    using BST::getMin;
    using BST::getMax;
    using BST::lower_bound;
    using BST::upper_bound;
    using BST::begin;
    using BST::end;
    using BST::rbegin;
    using BST::rend;
    using BST::find;
    using BST::Traverse;
    using BST::countLeaves;
    using BST::countNodes;
    using BST::isEmpty;

    using BST::ConstIterator;

public:

    size_t BST::getHeight() = delete;

    size_t getHeight() const {return getHeight(tree.getRoot());}

    AVLTree& operator=(const AVLTree& other) {
        if (this != &other){
            tree = other.tree;
        }
        return *this;
    }

    AVLTree& operator=(AVLTree&& other) noexcept {
        if (this != &other){
            tree = std::move(other.tree);
        }
        return *this;
    }

    bool operator==(const AVLTree& other) const { return tree == other.tree; }

    bool operator!=(const AVLTree& other) const { return !(*this == other); }

    template <typename U>
    friend void print(const AVLTree<U>& avl);

public:

    AVLTree() : BST() {}

    AVLTree(const AVLTree& other) : BST(other) {}

    AVLTree(AVLTree&& other) noexcept : BST(std::move(other)) {}

    AVLTree(std::initializer_list<T> list) : BST(list) {}

    AVLTree(const std::vector<T>& vec) : BST(vec) {}
    
    ~AVLTree() override = default;

private:
    Node* leftRotate(Node* p) {
        if (!p) return nullptr;
        Node* q = p->right, *r = q->left;
        this->tree.connect_left(q, p);
        this->tree.connect_right(p, r);
        p->height = std::max(getHeight(p->left), getHeight(p->right)) + 1;
        q->height = std::max(getHeight(q->left), getHeight(q->right)) + 1;
        return q;
    }

    Node* rightRotate(Node* p) {
        if (!p) return nullptr;
        Node* q = p->left, *r = q->right;
        this->tree.connect_right(q, p);
        this->tree.connect_left(p, r);
        p->height = std::max(getHeight(p->left), getHeight(p->right)) + 1;
        q->height = std::max(getHeight(q->left), getHeight(q->right)) + 1;
        return q;
    }

    Node* leftRightRotate(Node* p) {
        if (!p) return nullptr;
        this->tree.connect_left(p, leftRotate(p->left));
        return rightRotate(p);
    }

    Node* rightLeftRotate(Node* p) {
        if (!p) return nullptr;
        this->tree.connect_right(p, rightRotate(p->right));
        return leftRotate(p);
    }

    static size_t getHeight(const Node* p) {
        return p ? p->height : 0;
    }

    static int getBalanceFactor(const Node* p) {
        return p ? getHeight(p->left) - getHeight(p->right) : 0;
    }

    Node* rebalance(Node* p) {
        if (!p) return nullptr;
        int bf = getBalanceFactor(p);
        if (bf > 1) {
            if (getBalanceFactor(p->left) >= 0) {
                p = rightRotate(p);
            } else {
                p = leftRightRotate(p);
            }
        } else if (bf < -1) {
            if (getBalanceFactor(p->right) <= 0) {
                p = leftRotate(p);
            } else {
                p = rightLeftRotate(p);
            }
        }
        return p;
    }

    Node* BST::removeHelper(Node* node, const T& data) = delete;

    Node* removeHelper(Node* p, const T& data) {
        if (!p) return nullptr;
        if (data < p->data) tree.connect_left(p, removeHelper(p->left, data));
        else if (data > p->data) tree.connect_right(p, removeHelper(p->right, data));
        else {
            if (!p->left || !p->right) {
                Node* temp = p->left ? p->left : p->right;
                delete p;
                return temp;
            }
            Node* successor = p->right;
            while (successor->left) successor = successor->left;
            p->data = successor->data;
            tree.connect_right(p, removeHelper(p->right, successor->data));
        }
        p->height = std::max(getHeight(p->left), getHeight(p->right)) + 1;
        return p;
    }

public:
    void insert(const T& data) override {
        // 插入节点
        Node* node = insertHelper(tree.getRoot(), data);

        if (!node) return;
        if (!node->parent) {
            this->tree.setRoot(node);
            return;
        }

        // 调整平衡
        Node* current = node, *subRoot = nullptr, *curParent = node->parent;
        while (current) {
            current->height = std::max(getHeight(current->left), getHeight(current->right)) + 1;

            subRoot = rebalance(current);
            if (curParent) {
                connect(curParent, subRoot);
            } else {
                this->tree.setRoot(subRoot);
                return;
            }

            if (subRoot != current) return; // 已发生旋转，直接退出

            current = curParent;
            curParent = current->parent;
        }
    }

    void remove(const T& data) override {
        Node* newRoot = rebalance(removeHelper(tree.getRoot(), data));
        tree.setRoot(newRoot);
        if (newRoot) newRoot->parent = nullptr;
    }


};


template <typename T>
class RBTree : public BST<T,RBTLink> {
private:
    using Node = RBTNode<T>;
    using BST = BST<T, RBTLink>;

    using BST::tree;

public:
    using BST::getMin;
    using BST::getMax;
    using BST::lower_bound;
    using BST::upper_bound;
    using BST::begin;
    using BST::end;
    using BST::rbegin;
    using BST::rend;
    using BST::find;
    using BST::Traverse;
    using BST::countLeaves;
    using BST::countNodes;
    using BST::isEmpty;
    using BST::getHeight;

    using BST::ConstIterator;

public:

    RBTree& operator=(const RBTree& other) {
        if (this != &other){
            tree = other.tree;
        }
        return *this;
    }

    RBTree& operator=(RBTree&& other) noexcept {
        if (this != &other){
            tree = std::move(other.tree);
        }
        return *this;
    }

    bool operator==(const RBTree& other) const { return tree == other.tree; }

    bool operator!=(const RBTree& other) const { return !(*this == other); }

    template <typename U>
    friend void print(const RBTree<U>& rbt);

public:

    RBTree() : BST() {}

    RBTree(const RBTree& other) : BST(other) {}

    RBTree(RBTree&& other) noexcept : BST(std::move(other)) {}

    RBTree(std::initializer_list<T> list) : BST(list) {}

    RBTree(const std::vector<T>& vec) : BST(vec) {}
    
    ~RBTree() override = default;

private:
    Node* leftRotate(Node* p) {
        if (!p) return nullptr;
        Node* q = p->right, *r = q->left;
        this->tree.connect_left(q, p);
        this->tree.connect_right(p, r);
        return q;
    }

    Node* rightRotate(Node* p) {
        if (!p) return nullptr;
        Node* q = p->left, *r = q->right;
        this->tree.connect_right(q, p);
        this->tree.connect_left(p, r);
        return q;
    }

    Node* leftRightRotate(Node* p) {
        if (!p) return nullptr;
        this->tree.connect_left(p, leftRotate(p->left));
        return rightRotate(p);
    }

    Node* rightLeftRotate(Node* p) {
        if (!p) return nullptr;
        this->tree.connect_right(p, rightRotate(p->right));
        return leftRotate(p);
    }

    static bool isRed(Node* p) { return p && p->color == true; }

    void rebalance_after_insert(Node* inserter) {
        if (!inserter) return;

        // 插入节点为根节点，颜色设为黑色即可
        if (!inserter->parent) {
            inserter->color = false;
            this->tree.setRoot(inserter);
            return;
        }

        if (!isRed(inserter->parent)) return; // 父节点为黑色，必然平衡

        // 父节点为红色，则必有祖父节点
        Node* parent = inserter->parent, *grandparent = parent->parent;
        Node* uncle = grandparent->data < parent->data ? grandparent->left : grandparent->right;
        Node* centerNode = nullptr;

        if (!isRed(uncle)) { // 叔叔节点为黑色
            Node* subRoot = grandparent->parent;
            if (parent->data < inserter->data) {
                if (grandparent->data < parent->data) {
                    centerNode = leftRotate(grandparent);
                } else {
                    centerNode = leftRightRotate(grandparent);
                }
            } else {
                if (grandparent->data < parent->data) {
                    centerNode = rightLeftRotate(grandparent);
                } else {
                    centerNode = rightRotate(grandparent);
                }
            }

            if (subRoot) { // 维护结构
                connect(subRoot, centerNode);
            } else {
                tree.setRoot(centerNode);
            }

            // 中心点和旋转点变色
            centerNode->color = false, grandparent->color = true;
            return;
        } else { // 叔叔节点为红色
            grandparent->color = true, parent->color = false, uncle->color = false;
            rebalance_after_insert(grandparent); // 以祖父节点为新的插入点
        }
    }

    void handle_double_black(Node* db_node) { // 处理双黑节点
        Node* parent = db_node->parent; // 调用的上文可保证非空
        Node* sibling = parent->data < db_node->data ? parent->left : parent->right; // 调用的上文可保证非空
        Node* nephewL = sibling->left, *nephewR = sibling->right;
        Node* grandparent = parent->parent;
        Node* centerNode = nullptr;
        if (!isRed(sibling)) {
            if (!isRed(nephewL) && !isRed(nephewR)) { // 兄弟节点的两个子节点均为黑色
                sibling->color = true;
                if (isRed(parent) || !grandparent) { // 父节点为红色或为根节点，直接设为黑色即可
                    parent->color = false;
                } else { // 否则上移双黑节点，重新平衡
                    handle_double_black(parent);
                }
            } else { // 兄弟节点的至少一个子节点为红色
                if(sibling->data < parent->data){
                    if(isRed(nephewL)){ // LL型，变色+右旋
                        nephewL->color = false, sibling->color = parent->color, parent->color = false;
                        centerNode = rightRotate(parent);
                        connect(grandparent, centerNode);
                    } else { // LR型，变色+左右旋
                        nephewR->color = parent->color, parent->color = false;
                        centerNode = leftRightRotate(parent);
                        connect(grandparent, centerNode);
                    }
                } else {
                    if(isRed(nephewR)){ // RR型，变色+左旋
                        nephewR->color = false, sibling->color = parent->color, parent->color = false;
                        centerNode = leftRotate(parent);
                        connect(grandparent, centerNode);
                    } else { // RL型，变色+右左旋
                        nephewL->color = parent->color, parent->color = false;
                        centerNode = rightLeftRotate(parent);
                        connect(grandparent, centerNode);
                    }
                }
            }
        } else {
            sibling->color = false, parent->color = true;
            centerNode = sibling;
            if (sibling->data < parent->data) {
                this->tree.connect_right(sibling, parent);
                this->tree.connect_left(parent, nephewR);
                connect(grandparent, centerNode);
            } else {
                this->tree.connect_left(sibling, parent);
                this->tree.connect_right(parent, nephewL);
                connect(grandparent, centerNode);
            }
            handle_double_black(db_node);
        }
    }

    Node* BST::removeHelper(Node* node, const T& data) = delete;

    Node* removeHelper(Node* remover) {
        if (!remover) return nullptr;

        Node* parent = remover->parent;
        if(remover->left ^ remover->right) { // 只有一个子树(此时被删除节点一定为黑色，而且子树一定是单个节点)

            // 用子节点替代被删除节点
            Node* child = remover->left ? remover->left : remover->right;
            remover->data = child->data;
            return child;
        } else if (!remover->left && !remover->right) { // 无子树
            if (!parent) {
                return nullptr;
            }
            if (isRed(remover)) { // 被删除节点为红色，直接删除即可
                return remover;
            } else {
                handle_double_black(remover); // 处理双黑，没有发生替换
                return remover;
            }
        } else { // 有双子树
            // 找后继节点替换被删除节点
            Node* successor = remover->right;
            while (successor->left) {
                successor = successor->left;
            }
            remover->data = successor->data;

            // 删除原后继节点(对应只有右子树的情况)
            return removeHelper(successor);
        }
    }

public:
    void insert(const T& data) override {
        Node* node = insertHelper(tree.getRoot(), data);
        rebalance_after_insert(node);
    }

    void remove(const T& data) override {
        Node* remover = tree.getRoot();
        while (remover) {
            if (data < remover->data) remover = remover->left;
            else if (data > remover->data) remover = remover->right;
            else break;
        }
        if (!remover) return; // 未找到

        remover = removeHelper(remover);
        if (!remover) { // 删除的是根节点
            tree.setRoot(nullptr);
            return;
        }
        Node* parent = remover->parent;

        if (parent) {
            // 这里不比较值判断左右是因为删除时用了替换操作，可能会改变remover和parent的值关系
            if (parent->right == remover) parent->right = nullptr; 
            else parent->left = nullptr;
        } else {
            tree.setRoot(nullptr);
        }
        delete remover;
    }


};


template <typename U, NodeType UNT>
inline void print(const BST<U, UNT>& bst){
    print(bst.tree);
}

template <typename U>
inline void print(const AVLTree<U>& avl){
    print(avl.tree);
}

template <typename U>
inline void print(const RBTree<U>& rbt){
    print(rbt.tree);
}



#endif