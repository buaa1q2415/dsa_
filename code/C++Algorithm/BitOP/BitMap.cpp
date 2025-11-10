#include <iostream>
#include <print>
#include <format>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <stdexcept>

class BitMap{
private:
    std::vector<uint64_t> map;
    long long start,end;
public:
    BitMap(long long left,long long right):start(left),end(right){
        if(left>=right) throw "Invalid capacity";
        map.resize((end-start)/64+1,0);
    };
    BitMap(int cap):start(0),end(cap-1){
        if(cap<=0) throw "Invalid capacity";
        map.resize((cap+63)/64,0);
    };
    BitMap(const std::vector<long long>& arr);
    void Push(int n){if(n>=start && n<=end) map[(n-start)/64]|=(1<<(n-start)%64);};
    void Pop(int n){map[(n-start)/64]&=(~(1<<(n-start)%64));};
    void Flip(int n){map[(n-start)/64]^=(1<<(n-start)%64);};
    bool Contain(int n){return (map[(n-start)/64]&(1<<(n-start)%64))!=0;};

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

    BitMap& operator&=(const BitMap& other);
    BitMap& operator|=(const BitMap& other);
    BitMap& operator^=(const BitMap& other);
    BitMap& operator-=(const BitMap& other);

friend std::ostream& operator<<(std::ostream& os,const BitMap& map);
};

BitMap::BitMap(const std::vector<long long>& arr){
    end=LLONG_MIN,start=LLONG_MAX;
    for(long long i:arr){
        if(i<start) start=i;
        if(i>end) end=i;
    }
    int cap=end-start+1;
    if(cap<=0) throw "Invalid capacity";
    map.resize((cap+63)/64,0);
    for(long long i:arr){
        map[(i-start)/64]|=(1<<(i-start)%64);
    }
}


