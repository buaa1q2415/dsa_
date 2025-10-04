/**并查集（Union-Find）是一种用于处理不相交集合的合并与查询问题的数据结构，常用于解决图论中的连通性问题
 * 并查集的基本思想是：每个集合选出一个元素作为代表(即根结点)，储存每个元素的父节点，向上查找根节点以判断连通性
 * 并查集的基本操作包括：
 * 1. 初始化：将每个元素自成一个集合
 * 2. 查找：找到元素所在集合的根节点（带路径压缩优化）
 * 3. 合并：将两个元素所在的集合合并（按秩合并优化）
 * 并查集通常只能直接用于整数元素，对于其他类型的元素，通常使用hash或enum把元素映射到整数
 * 并查集的时间复杂度为O(α(n))，其中α(n)为阿克曼函数的反函数，近似为O(1)
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

class UnionFind{
private:
    vector<int> parent;  // 存储每个元素的父节点
    vector<int> rank;    // 存储每个集合的秩（用于优化合并）
public:
    UnionFind(int size);//构造函数：初始化并查集，每个元素自成一个集合
    int Find(int x);//查找操作：找到元素x所在集合的根节点(带路径压缩优化)
    bool Union(int x, int y);//合并操作：将元素x和元素y所在的集合合并(按秩合并优化)
};

UnionFind::UnionFind(int size){ 
    parent.resize(size);
    rank.resize(size,0);
    for(int i=0;i<size;++i) {
        parent[i]=i; //初始时，每个元素的父节点是自身
    }
}

int UnionFind::Find(int x){
    if (parent[x]!=x){
        parent[x]=Find(parent[x]);// 如果x不是根节点，递归查找其父节点的根节点
    }
    return parent[x];// 并将x的父节点直接指向根节点，实现路径压缩
}

bool UnionFind::Union(int x, int y){
    int rootX = Find(x); //找到x和y的根节点
    int rootY = Find(y);
    if (rootX == rootY) return false; //如果已经在同一个集合中，无需合并
    if (rank[rootX] < rank[rootY]){ //按秩合并：将秩较小的集合合并到秩较大的集合中
        parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]){
        parent[rootY] = rootX;
    } else{
        parent[rootY] = rootX;
        rank[rootX]++;// 秩相等时，合并后秩加1
    }
    return true;
}