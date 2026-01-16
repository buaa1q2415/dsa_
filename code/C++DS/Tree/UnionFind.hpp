#ifndef UnionFind_HPP
#define UnionFind_HPP

#include <iostream>
#include <vector>
#include <functional>
#include <unordered_map>

/**
 * @file UnionFind.hpp
 * @brief 提供了高性能的并查集（Disjoint Set Union）数据结构实现。
 *
 * 这个文件包含两个版本的并查集：
 * 1. 泛型模板类 `UnionFind<T, Hasher>`:
 *    - 支持任何可哈希的类型 `T` (例如 std::string, 自定义结构体等)。
 *    - 内部使用哈希表进行元素到索引的映射。
 *    - 为了高性能，建议在构造时或使用前预估元素数量并调用 reserve()。
 *
 * 2. 针对`int`的特化版本 `UnionFind<int>`:
 *    - 专为整数 `0` 到 `N-1` 优化，性能极高。
 *    - 不涉及任何哈希操作，所有操作都是基于数组的直接访问。
 *    - 是解决基于整数索引问题的首选。
 *
 */


// =======================================================================
//                           API 接口文档
// =======================================================================

/*
-------------------------------------------------------------------------
 I. 泛型版本: UnionFind<T, Hasher>
-------------------------------------------------------------------------
用于处理任意可哈希类型的元素。

---[ 构造与容量管理 ]---

/// @brief 构造函数。
/// @param capacity 预估的元素数量，用于预分配内存以提升性能。默认为32。
UnionFind(int capacity = 32);

/// @brief 预分配内存。
/// 在添加大量元素前调用，可以避免多次内存重分配，显著提高性能。
void reserve(int capacity);


---[ 核心操作 ]---

/// @brief 合并两个元素所在的集合。
/// 如果元素不存在，会自动添加它们。
void Union(const T& element1, const T& element2);
void Union(T&& element1, T&& element2); // 移动语义版本

/// @brief 检查两个元素是否属于同一个集合。
/// @return 如果两个元素在同一集合中，返回 true。如果任一元素不存在，返回 false。
bool isConnected(const T& element1, const T& element2);

/// @brief 查找一个元素所在集合的代表元。
/// @return 返回一个 SetHandle<T> 对象。
///         - 如果元素存在，返回的 SetHandle 有效。
///         - 如果元素不存在，返回的 SetHandle 无效 (等同于 UnionFind<T>::NotFound)。
SetHandle<T> Find(const T& element);


---[ 元素添加 ]---

/// @brief 添加一个新元素，使其自成一个集合。
/// 如果元素已存在，则此操作无效。
void Add(const T& element);
void Add(T&& element); // 移动语义版本
void Add(const std::vector<T>& elements);
void Add(const std::initializer_list<T>& elements);


---[ 辅助类型与常量 ]---

/// @class SetHandle<T>
/// Find() 函数的返回值。它是一个安全的句柄，指向集合的代表元。
///
/// 用法:
///   auto handle = uf.Find("some_element");
///   if (handle) { // 或 handle.isValid()
///       // 1. 解引用获取值
///       std::cout << *handle << std::endl;
///       // 2. 对于结构体/类，使用箭头操作符
///       // std::cout << handle->some_member << std::endl;
///   }
///   // 3. 句柄之间可以直接比较
///   if (uf.Find("a") == uf.Find("b")) { ... }

/// @var NotFound
/// 一个静态的、无效的 SetHandle<T> 常量，用于表示“未找到”。
inline static const SetHandle<T> NotFound = ...;


-------------------------------------------------------------------------
 II. `int` 特化版本: UnionFind<int>
-------------------------------------------------------------------------
为整数 `0` 到 `N-1` 进行了高度优化，性能最佳。

---[ 构造与容量管理 ]---

/// @brief 构造函数。
/// @param size 并查集将处理的整数范围为 [0, size - 1]。
UnionFind(int size);

/// @brief 调整并查集的大小。
/// 可用于扩大整数的处理范围。
void resize(int size);


---[ 核心操作 ]---

/// @brief 合并整数 x 和 y 所在的集合。
/// @param x, y 必须在 [0, size - 1] 范围内。
void Union(int x, int y);

/// @brief 检查整数 x 和 y 是否属于同一个集合。
bool isConnected(int x, int y);

/// @brief 查找整数 x 所在集合的根节点（代表元）。
/// @return 返回根节点的整数索引。
int Find(int x);


-------------------------------------------------------------------------
 III. 快速使用示例
-------------------------------------------------------------------------

// --- 示例 1: 使用泛型版本处理 std::string ---
#include "UnionFind.hpp"
#include <string>
#include <iostream>

void genericExample() {
    UnionFind<std::string> uf;
    uf.Add({"apple", "banana", "orange", "grape", "melon"});

    uf.Union("apple", "orange");
    uf.Union("banana", "grape");

    if (uf.isConnected("apple", "orange")) {
        std::cout << "Apple and orange are connected." << std::endl;
    }

    auto handle = uf.Find("grape");
    if (handle) {
        std::cout << "The representative for 'grape' is '" << *handle << "'." << std::endl;
    }

    if (uf.Find("apple") != uf.Find("banana")) {
        std::cout << "Apple and banana are in different sets." << std::endl;
    }
}


// --- 示例 2: 使用 int 特化版本处理图的连通性 ---
#include "UnionFind.hpp"
#include <iostream>

void intExample() {
    int num_nodes = 10;
    UnionFind<int> uf(num_nodes); // 处理节点 0 到 9

    // 假设有一些边
    uf.Union(0, 1);
    uf.Union(2, 3);
    uf.Union(0, 4);

    if (uf.isConnected(1, 4)) { // 应该为 true, 因为 1->0, 0->4
        std::cout << "Nodes 1 and 4 are connected." << std::endl;
    }

    int root = uf.Find(1);
    std::cout << "The root for node 1 is " << root << std::endl;
}
*/


/**并查集（Union-Find）是一种用于处理不相交集合的合并与查询问题的数据结构，常用于解决图论中的连通性问题
 * 并查集的基本思想是：每个集合选出一个元素作为代表(即根结点)，储存每个元素的父节点，向上查找根节点以判断连通性
 * 并查集的基本操作包括：
 * 1. 初始化：将每个元素自成一个集合
 * 2. 查找：找到元素所在集合的根节点（带路径压缩优化）
 * 3. 合并：将两个元素所在的集合合并（按秩合并优化）
 * 并查集通常只能直接用于整数元素，对于其他类型的元素，通常使用hash或enum把元素映射到整数
 * 并查集的时间复杂度为O(α(n))，其中α(n)为阿克曼函数的反函数，近似为O(1)
 */

template <typename T>
class SetHandle {
private:
    template <typename U, typename H> friend class UnionFind;

    std::vector<T>* itoe_ptr;
    int root_index;

    SetHandle(std::vector<T>* itoe, int index) : itoe_ptr(itoe), root_index(index) {}

public:
    ~SetHandle()=default;
    // 解引用操作符，返回代表元
    const T& operator*() const {
        if (!isValid()) throw std::runtime_error("Dereferencing an invalid SetHandle");
        return (*itoe_ptr)[root_index];
    }
        
    // 箭头操作符，用于访问代表元的成员
    const T* operator->() const {
        if (!isValid()) throw std::runtime_error("Dereferencing an invalid SetHandle");
        return &(*itoe_ptr)[root_index];
    }

    // 检查结果是否有效
    bool isValid() const {
        return itoe_ptr != nullptr && root_index != -1;
    }

    // 允许像 bool 一样使用
    explicit operator bool() const {
        return isValid();
    }

    friend bool operator==(const SetHandle& a, const SetHandle& b) {
        return a.itoe_ptr == b.itoe_ptr && a.root_index == b.root_index;
    }
    friend bool operator!=(const SetHandle& a, const SetHandle& b) {
        return !(a == b);
    }
};

template <typename T, typename Hasher = std::hash<T>>
class UnionFind{
private:
    std::vector<int> parent;
    std::vector<int> rank;
    
    std::unordered_map<T, int, Hasher> element_to_index;
    std::vector<T> index_to_element;

    int next_index;

    int getOrCreateIndex(const T& element) {
        auto [it,inserted] = element_to_index.try_emplace(element, next_index);
        if (inserted) {
            parent.emplace_back(next_index);
            rank.emplace_back(0);
            index_to_element.emplace_back(element);
            next_index++; 
        }
        return it->second; 
    }
    int getOrCreateIndex(T&& element) {
        auto [it,inserted] = element_to_index.try_emplace(std::move(element), next_index);
        if (inserted) {
            parent.emplace_back(next_index);
            rank.emplace_back(0);
            index_to_element.emplace_back(it->first);
            next_index++; 
        }
        return it->second; 
    }
    int findRootIndex(int index) {// findRoot总是处理和返回索引
        if (parent[index] == index) {
            return index;
        }
        // 路径压缩
        return parent[index] = findRootIndex(parent[index]);
    }
public:
    inline static const SetHandle<T> NotFound = SetHandle<T>(nullptr, -1);

    UnionFind(int size=32): next_index(0) {
        parent.reserve(size);
        rank.reserve(size);
        element_to_index.reserve(size);
        index_to_element.reserve(size);
    };
    ~UnionFind()=default;
    
    void reserve(int size){
        parent.reserve(size);
        rank.reserve(size);
        element_to_index.reserve(size);
        index_to_element.reserve(size);
    }

    void Union(const T& element1, const T& element2);
    void Union(T&& element1, T&& element2);
    bool isConnected(const T& element1, const T& element2);
    void Add(const T& element);
    void Add(T&& element);
    void Add(const std::vector<T>& elements);
    void Add(const std::initializer_list<T>& elements);
    SetHandle<T> Find(const T& element);

};

template<>
class UnionFind<int>{ // 提供基础版本的UnionFind，需要手动维护映射和容量，但性能更好
private:
    std::vector<int> parent;  // 存储每个元素的父节点
    std::vector<int> rank;    // 存储每个集合的秩（用于优化合并）
public:
    UnionFind(int size);//构造函数：初始化并查集，每个元素自成一个集合
    void resize(int size);
    int Find(int x);//查找操作：找到元素x所在集合的根节点(带路径压缩优化)
    void Union(int x, int y);//合并操作：将元素x和元素y所在的集合合并(按秩合并优化)
    bool isConnected(int x,int y);
};


// 泛类UnionFind函数定义

template <typename T, typename Hasher>
void UnionFind<T, Hasher>::Union(const T& element1, const T& element2) {
    int index1 = getOrCreateIndex(element1);
    int index2 = getOrCreateIndex(element2);
    
    int root1 = findRootIndex(index1);
    int root2 = findRootIndex(index2);

    if (root1 == root2) {
        return;
    }
    if (rank[root1] < rank[root2]) {
        parent[root1] = root2;
    } else if (rank[root1] > rank[root2]) {
        parent[root2] = root1;
    } else {
        parent[root1] = root2;
        rank[root2]++;
    }
}

template <typename T, typename Hasher>
void UnionFind<T, Hasher>::Union(T&& element1, T&& element2) {
    int index1 = getOrCreateIndex(std::move(element1));
    int index2 = getOrCreateIndex(std::move(element2));
    
    int root1 = findRootIndex(index1);
    int root2 = findRootIndex(index2);

    if (root1 == root2) {
        return;
    }

    if (rank[root1] < rank[root2]) {
        parent[root1] = root2;
    } else if (rank[root1] > rank[root2]) {
        parent[root2] = root1;
    } else {
        parent[root1] = root2;
        rank[root2]++;
    }
}

template <typename T, typename Hasher>
bool UnionFind<T, Hasher>::isConnected(const T& element1, const T& element2) {
    auto it1 = element_to_index.find(element1);
    auto it2 = element_to_index.find(element2);
    if(it1==element_to_index.end() || it2==element_to_index.end()) return false;
    return findRootIndex(it1->second) == findRootIndex(it2->second);
}

template <typename T, typename Hasher>
void UnionFind<T, Hasher>::Add(const T& element){
    auto [it,inserted] = element_to_index.try_emplace(element, next_index);
    if (inserted) {
        parent.emplace_back(next_index);
        rank.emplace_back(0);
        index_to_element.emplace_back(element);
        next_index++; 
    }
}

template <typename T, typename Hasher>
void UnionFind<T, Hasher>::Add(T&& element){
    auto [it,inserted] = element_to_index.try_emplace(std::move(element), next_index);
    if (inserted) {
        parent.emplace_back(next_index);
        rank.emplace_back(0);
        index_to_element.emplace_back(it->first);
        next_index++; 
    }
}

template <typename T, typename Hasher>
void UnionFind<T, Hasher>::Add(const std::vector<T>& elements){
    for(const T& element:elements){
        Add(element);
    }
}

template <typename T, typename Hasher>
void UnionFind<T, Hasher>::Add(const std::initializer_list<T>& elements){
    for(const T& element:elements){
        Add(element);
    }
}

template <typename T, typename Hasher>
SetHandle<T> UnionFind<T, Hasher>::Find(const T& element){
    auto it = element_to_index.find(element);
    if (it == element_to_index.end()) {
         return NotFound; // 返回无效结果
    }
    int root = findRootIndex(it->second);
    return SetHandle(&(this->index_to_element), root); // 返回有效结果
}


// 特化UnionFind函数定义

UnionFind<int>::UnionFind(int size){ 
    parent.resize(size);
    rank.resize(size,0);
    for(int i=0;i<size;++i) {
        parent[i]=i; //初始时，每个元素的父节点是自身
    }
}

void UnionFind<int>::resize(int size){
    int sz=parent.size();
    parent.resize(size);
    rank.resize(size);
    for(int i=sz;i<size;++i) {
        parent[i]=i;
        rank[i]=0;
    }
}

int UnionFind<int>::Find(int x){
    int root = x;
    while (parent[root] != root) {
        root = parent[root];
    }
    while (parent[x] != root) {
        int next_node = parent[x];
        parent[x] = root;
        x = next_node;
    }
    return root;
}

void UnionFind<int>::Union(int x, int y){
    int rootX = Find(x); //找到x和y的根节点
    int rootY = Find(y);
    if (rootX == rootY) return; //如果已经在同一个集合中，无需合并
    if (rank[rootX] < rank[rootY]){ //按秩合并：将秩较小的集合合并到秩较大的集合中
        parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]){
        parent[rootY] = rootX;
    } else{
        parent[rootY] = rootX;
        rank[rootX]++;// 秩相等时，合并后秩加1
    }
}

bool UnionFind<int>::isConnected(int x,int y){
    return Find(x)==Find(y);
}

#endif