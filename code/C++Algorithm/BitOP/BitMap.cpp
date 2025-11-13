#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <stdexcept>

// 引入MSVC的intrinsic头文件
#if defined(_MSC_VER)
#include <intrin.h>
#endif

class BitMap{
private:
    std::vector<uint64_t> map;
    long long lbound,rbound; // 保证capacity=rbound-lbound+1在int范围内(超int了大概率直接崩了，不想管了)
    bool overturn;
    int ones;
    static uint64_t QuickReverse(uint64_t x){
        // 交换相邻1位，形成32对"2位块"（每对内部已反转）
        x = ((x & 0x5555555555555555) << 1) | ((x & 0xAAAAAAAAAAAAAAAA) >> 1);
        // 交换相邻2位块，形成16对"4位块"（每对内部已反转）
        x = ((x & 0x3333333333333333) << 2) | ((x & 0xCCCCCCCCCCCCCCCC) >> 2);
        // 交换相邻4位块，形成8对"8位块"
        x = ((x & 0x0F0F0F0F0F0F0F0F) << 4) | ((x & 0xF0F0F0F0F0F0F0F0) >> 4);
        // 交换相邻8位块，形成4对"16位块"
        x = ((x & 0x00FF00FF00FF00FF) << 8) | ((x & 0xFF00FF00FF00FF00) >> 8);
        // 交换相邻16位块，形成2对"32位块"
        x = ((x & 0x0000FFFF0000FFFF) << 16) | ((x & 0xFFFF0000FFFF0000) >> 16);
        // 交换两个32位块，完成整体反转
        x = (x << 32) | (x >> 32);
        return x;
    }
    void QuickOnes() {
        ones=0;
        uint64_t n;
        for(int i=0;i<map.size()-1;i++){
            n=map[i];
            n = (n & 0x5555555555555555ULL) + ((n >> 1) & 0x5555555555555555ULL);
            n = (n & 0x3333333333333333ULL) + ((n >> 2) & 0x3333333333333333ULL);
            n = (n & 0x0F0F0F0F0F0F0F0FULL) + ((n >> 4) & 0x0F0F0F0F0F0F0F0FULL);
            n = (n & 0x00FF00FF00FF00FFULL) + ((n >> 8) & 0x00FF00FF00FF00FFULL);
            n = (n & 0x0000FFFF0000FFFFULL) + ((n >> 16) & 0x0000FFFF0000FFFFULL);
            n = (n & 0x00000000FFFFFFFFULL) + ((n >> 32) & 0x00000000FFFFFFFFULL);
            ones+=n;
        }
        if((rbound-lbound+1)%64==0) n=map.back();
        else n=map.back()&((1ULL<<((rbound-lbound+1)%64))-1);
        n = (n & 0x5555555555555555ULL) + ((n >> 1) & 0x5555555555555555ULL);
        n = (n & 0x3333333333333333ULL) + ((n >> 2) & 0x3333333333333333ULL);
        n = (n & 0x0F0F0F0F0F0F0F0FULL) + ((n >> 4) & 0x0F0F0F0F0F0F0F0FULL);
        n = (n & 0x00FF00FF00FF00FFULL) + ((n >> 8) & 0x00FF00FF00FF00FFULL);
        n = (n & 0x0000FFFF0000FFFFULL) + ((n >> 16) & 0x0000FFFF0000FFFFULL);
        n = (n & 0x00000000FFFFFFFFULL) + ((n >> 32) & 0x00000000FFFFFFFFULL);
        ones+=n;
    };
public:
    BitMap(long long left,long long right):lbound(left),rbound(right){
        if(left>right) throw std::length_error("Invalid capacity");
        map.resize((rbound-lbound)/64+1,0);
        overturn=false;
        ones=0;
    };
    BitMap(int cap):lbound(0),rbound(cap-1){
        if(cap<=0) throw std::length_error("Invalid capacity");
        map.resize((cap+63)/64,0);
        overturn=false;
        ones=0;
    };
    BitMap(const std::vector<long long>& arr);
    inline void Push(int n);
    inline void Pop(int n);
    inline void Flip(int n);
    inline bool Contain(int n);

    void Fill();
    void Clear();
    void Overturn(){overturn=(!overturn);};
    bool isEmpty() const{return ones==overturn*(rbound-lbound+1);};
    bool isFull() const{return ones==(!overturn)*(rbound-lbound+1);};
    int getSize() const{return (overturn)? rbound-lbound+1-ones:ones;}
friend void swap(BitMap& one,BitMap& other) noexcept;

    long long getFirst() const;
    long long getNext(long long n) const;
    long long getBack() const;

    BitMap operator&(const BitMap& other) const;
    BitMap operator|(const BitMap& other) const;
    BitMap operator^(const BitMap& other) const;
    BitMap operator~() const;
    BitMap operator-(const BitMap& other) const;

    BitMap& operator&=(const BitMap& other);
    BitMap& operator|=(const BitMap& other);
    BitMap& operator^=(const BitMap& other);
    BitMap& operator-=(const BitMap& other);

    bool operator==(const BitMap& other) const{return (lbound == other.lbound && rbound == other.rbound && getSize() == other.getSize() && (*this ^ other).isEmpty());}
    bool operator!=(const BitMap& other) const{return (lbound != other.lbound || rbound != other.rbound || getSize() != other.getSize() || !(*this ^ other).isEmpty());}

    class iterator{
    private:
        friend class BitMap; 
        iterator(const BitMap* bmp, long long value):bitmap(bmp), current_value(value){} // 只有 BitMap 自己能创建迭代器
        const BitMap* bitmap;     // 指向所属的 BitMap
        long long current_value;  // 当前指向的值
    public:
        // C++迭代器标准要求的类型别名，让STL算法知道如何与它协作
        using iterator_category =std::forward_iterator_tag;
        using value_type=long long;
        using difference_type=std::ptrdiff_t;
        using pointer=const long long*;
        using reference =const long long;

        value_type operator*() const{return current_value;}
        iterator& operator++(){
            current_value = bitmap->getNext(current_value);
            return *this;
        }
        iterator operator++(int){
            iterator tmp = *this;
            current_value = bitmap->getNext(current_value);
            return tmp;
        }

        friend bool operator==(const iterator& a, const iterator& b) {return a.bitmap == b.bitmap && a.current_value == b.current_value;}
        friend bool operator!=(const iterator& a, const iterator& b) {return !(a == b);}
    };

    iterator begin() const{return iterator(this,getFirst());}
    iterator end() const{return iterator(this,rbound+1);}
};

BitMap::BitMap(const std::vector<long long>& arr){
    rbound=LLONG_MIN,lbound=LLONG_MAX;
    for(long long i:arr){
        if(i<lbound) lbound=i;
        if(i>rbound) rbound=i;
    }
    int cap=rbound-lbound+1;
    if(cap<=0) throw std::length_error("Invalid capacity");
    overturn=false;
    ones=0;
    map.resize((cap+63)/64,0);
    for(long long i:arr) map[(i-lbound)/64]|=(1ULL<<((i-lbound)%64));
    QuickOnes();
}

void BitMap::Push(int n){
    if(n<lbound || n>rbound) return; 
    ones+=1-overturn-((map[(n-lbound)/64]>>((n-lbound)%64))&1ULL);
    if(overturn==false) map[(n-lbound)/64]|=(1ULL<<((n-lbound)%64));
    else map[(n-lbound)/64]&=(~(1ULL<<(n-lbound)%64));
}

void BitMap::Pop(int n){
    if(n<lbound || n>rbound) return; 
    ones+=overturn-((map[(n-lbound)/64]>>((n-lbound)%64))&1ULL);
    if(overturn==false) map[(n-lbound)/64]&=(~(1ULL<<((n-lbound)%64)));
    else map[(n-lbound)/64]|=(1ULL<<(n-lbound)%64);
}

void BitMap::Flip(int n){
    if(n<lbound || n>rbound) return;
    ones+=((map[(n-lbound)/64]>>((n-lbound)%64))&1ULL)? -1:1;
    map[(n-lbound)/64]^=(1ULL<<(n-lbound)%64);
}

bool BitMap::Contain(int n){
    if(n<lbound || n>rbound) return false;
    return ((map[(n-lbound)/64]&(1ULL<<((n-lbound)%64)))!=0)!=overturn;
}

void BitMap::Fill(){
    std::fill(map.begin(),map.end(),UINT64_MAX);
    ones=rbound-lbound+1;
    overturn=false;
}

void BitMap::Clear(){
    std::fill(map.begin(),map.end(),0);
    ones=0;
    overturn=false;
}

BitMap BitMap::operator&(const BitMap& other) const{
    if(lbound==other.lbound && rbound==other.rbound){
        BitMap bitmap(lbound,rbound);
        for(int i=0;i<(rbound-lbound)/64+1;i++){
            bitmap.map[i]=(map[i]^overturn)&(other.map[i]^other.overturn);
        }
        bitmap.QuickOnes();
        return bitmap;
    }
    throw std::invalid_argument("Unmatched bounds");
}

BitMap BitMap::operator|(const BitMap& other) const{
    if(lbound==other.lbound && rbound==other.rbound){
        BitMap bitmap(lbound,rbound);
        for(int i=0;i<(rbound-lbound)/64+1;i++){
            bitmap.map[i]=(map[i]^overturn)|(other.map[i]^other.overturn);
        }
        bitmap.QuickOnes();
        return bitmap;
    }
    throw std::invalid_argument("Unmatched bounds");
}

BitMap BitMap::operator^(const BitMap& other) const{
    if(lbound==other.lbound && rbound==other.rbound){
        BitMap bitmap(lbound,rbound);
        for(int i=0;i<(rbound-lbound)/64+1;i++){
            bitmap.map[i]=(map[i]^overturn)^(other.map[i]^other.overturn);
        }
        bitmap.QuickOnes();
        return bitmap;
    }
    throw std::invalid_argument("Unmatched bounds");
}

BitMap BitMap::operator~() const{
    BitMap bitmap(lbound,rbound);
    bitmap.ones=ones;
    bitmap.overturn=!overturn;
    bitmap.map=map;
    return bitmap;
}

BitMap BitMap::operator-(const BitMap& other) const{
    if(lbound==other.lbound && rbound==other.rbound){
        BitMap bitmap(lbound,rbound);
        for(int i=0;i<(rbound-lbound)/64+1;i++){
            bitmap.map[i]=(map[i]^overturn)&(~(other.map[i]^other.overturn));
        }
        bitmap.QuickOnes();
        return bitmap;
    }
    throw std::invalid_argument("Unmatched bounds");
}

BitMap& BitMap::operator&=(const BitMap& other){
    if(lbound==other.lbound && rbound==other.rbound){
        for(int i=0;i<(rbound-lbound)/64+1;i++){
            map[i]=(map[i]^overturn)&(other.map[i]^other.overturn);
        }
        overturn=false;
        QuickOnes();
    }
    throw std::invalid_argument("Unmatched bounds");
}

BitMap& BitMap::operator|=(const BitMap& other){
    if(lbound==other.lbound && rbound==other.rbound){
        for(int i=0;i<(rbound-lbound)/64+1;i++){
            map[i]=(map[i]^overturn)|(other.map[i]^other.overturn);
        }
        overturn=false;
        QuickOnes();
    }
    throw std::invalid_argument("Unmatched bounds");
}

BitMap& BitMap::operator^=(const BitMap& other){
    if(lbound==other.lbound && rbound==other.rbound){
        for(int i=0;i<(rbound-lbound)/64+1;i++){
            map[i]=(map[i]^overturn)^(other.map[i]^other.overturn);
        }
        overturn=false;
        QuickOnes();
    }
    throw std::invalid_argument("Unmatched bounds");
}

BitMap& BitMap::operator-=(const BitMap& other){
    if(lbound==other.lbound && rbound==other.rbound){
        for(int i=0;i<(rbound-lbound)/64+1;i++){
            map[i]=(map[i]^overturn)&(~(other.map[i]^other.overturn));
        }
        overturn=false;
        QuickOnes();
    }
    throw std::invalid_argument("Unmatched bounds");
}

void swap(BitMap& one,BitMap& other) noexcept{
    std::swap(one.lbound,other.lbound);
    std::swap(one.rbound,other.rbound);
    std::swap(one.map,other.map);
    std::swap(one.overturn,other.overturn);
    std::swap(one.ones,other.ones);
}

long long BitMap::getFirst() const{
    long long first=rbound+1;
    uint64_t block;
    for (int i=0; i<map.size();i++) {
        block=(overturn)? ~map[i]:map[i];
        if(block!=0) {
            unsigned long bit_index;
        #if defined(_MSC_VER) // MSVC
            _BitScanForward64(&bit_index, block);
        #else // GCC/Clang
            bit_index = __builtin_ctzll(block);
        #endif
            first=lbound+(i*64)+bit_index;
            return (first>=rbound+1)? rbound+1:first;
        }
    }
    return rbound+1;
}

long long BitMap::getNext(long long n) const{
    if(n>=rbound) return rbound+1;
    if(n<lbound) return getFirst();
    long long offset=n-lbound,next;
    uint64_t block;
    if((offset+1)%64!=0){
        block=(overturn)? ~map[offset/64]:map[offset/64];
        block=block&(~((1ULL<<(offset+1)%64)-1));
        if(block!=0) {
            unsigned long bit_index;
        #if defined(_MSC_VER) // MSVC
            _BitScanForward64(&bit_index, block);
        #else // GCC/Clang
            bit_index = __builtin_ctzll(block);
        #endif
            next=lbound+((offset/64)*64)+bit_index;
            return (next>=rbound+1)? rbound+1:next;
        }
    }
    for (int i=offset/64+1;i<map.size();i++) {
        block=(overturn)? ~map[i]:map[i];
        if(block!=0) {
            unsigned long bit_index;
        #if defined(_MSC_VER) // MSVC
            _BitScanForward64(&bit_index, block);
        #else // GCC/Clang
            bit_index = __builtin_ctzll(block);
        #endif
            next=lbound+(i*64)+bit_index;
            return (next>=rbound+1)? rbound+1:next;
        }
    }
    return rbound+1;
}

long long BitMap::getBack() const{
    uint64_t block=map.back();
    block=(overturn)? ~map.back():map.back();
    if((rbound-lbound+1)%64!=0) block&=((1ULL<<((rbound-lbound+1)%64))-1);
    block=QuickReverse(block);
    if(block!=0) {
        unsigned long bit_index;
    #if defined(_MSC_VER) // MSVC
        _BitScanForward64(&bit_index, block);
    #else // GCC/Clang
        bit_index = __builtin_ctzll(block);
    #endif
        return lbound+(map.size()-1)*64+63-bit_index;
    }
    for(int i=map.size()-2;i>=0;i--){
        block=(overturn)? ~map[i]:map[i];
        block=QuickReverse(block);
        if(block!=0){
            unsigned long bit_index;
        #if defined(_MSC_VER) // MSVC
            _BitScanForward64(&bit_index, block);
        #else // GCC/Clang
            bit_index = __builtin_ctzll(block);
        #endif
            return lbound+i*64+63-bit_index;
        }
    }
    return lbound-1;
}
