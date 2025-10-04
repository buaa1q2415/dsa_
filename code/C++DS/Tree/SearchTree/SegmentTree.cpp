#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstddef>
#include <optional>

template <typename Elemtype>
class sgNode{
private:
    Elemtype data;
    int lazy;
};

template <typename Elemtype>
class SegmentTree{
private:
    std::vector<sgNode<Elemtype>> nodes;
    int n;
public:
    SegmentTree(int n){
        this->n=n;
        tree.resize(4*n);
    }
};
