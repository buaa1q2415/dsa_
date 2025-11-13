#include <iostream>

// 64位数O(1)倒置
uint64_t QuickReverse(uint64_t x){
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
}

//64位O(1)求n中1的数量
int QuickOnes(uint64_t n){
    n = (n & 0x5555555555555555ULL) + ((n >> 1) & 0x5555555555555555ULL);
    n = (n & 0x3333333333333333ULL) + ((n >> 2) & 0x3333333333333333ULL);
    n = (n & 0x0F0F0F0F0F0F0F0FULL) + ((n >> 4) & 0x0F0F0F0F0F0F0F0FULL);
    n = (n & 0x00FF00FF00FF00FFULL) + ((n >> 8) & 0x00FF00FF00FF00FFULL);
    n = (n & 0x0000FFFF0000FFFFULL) + ((n >> 16) & 0x0000FFFF0000FFFFULL);
    n = (n & 0x00000000FFFFFFFFULL) + ((n >> 32) & 0x00000000FFFFFFFFULL);
    return n;
};

// 求最右侧1的位置
uint64_t lowbit(uint64_t n){
    return n^(~n+1);
}
