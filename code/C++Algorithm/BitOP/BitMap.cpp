#include <iostream>
#include <print>
#include <format>
#include <vector>

class BitMap{
private:
    std::vector<uint64_t> map;
    int start,end;
public:
    BitMap(int left,int right):start(left),end(right){};
    BitMap(int cap):start(0),end(cap-1){};
    void Push(int idx);
    void Pop(int idx);
    void Flip(int idx);
    bool Contain(int idx);

    void Fill();
    void Clear();
    void FlipAll();

    bool IsEmpty();
    bool IsFull();

    void Resize(int newcap);

    BitMap operator&(const BitMap& other) const;
    BitMap operator|(const BitMap& other) const;
    BitMap operator^(const BitMap& other) const;
    BitMap operator~() const;
    BitMap operator-(const BitMap& other) const;
};