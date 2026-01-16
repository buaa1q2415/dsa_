#ifndef QUICKPOW_HPP
#define QUICKPOW_HPP

/* 快速幂
 * 计算a^b mod p
 * 时间复杂度O(logb)
 * 快速幂的原理是利用二进制拆分指数，如果指数的二进制表示为1011，那么
 * a^0b1011=(a^0b1000)*(a^0b0000)*(a^0b0010)*(a^0b0001)
 * 从右往左遍历指数的二进制位，如果第i位(i>=0)为1，则将结果乘上a^(2^i)
 */

#include <vector>
#include <algorithm>
#include <stdexcept>


long long QuickPow(long long a,long long b,long long p){
    if(p == 0) throw std::invalid_argument("QuickPow(): modulus p must not be 0");
    long long res=1;
    a%=p;
    while(b){
        if(b&1) res=( (__int128)res*a) % p;
        a=((__int128)a*a)%p;
        b>>=1;
    }
    return res;
}

long long QuickPow(long long a,long long b){
    long long res=1;
    while(b){
        if(b&1) res=res*a;
        a=a*a;
        b>>=1;
    }
    return res;
}

//矩阵快速幂
static inline std::vector<std::vector<long long>> MatrixMul(const std::vector<std::vector<long long>> &A,const std::vector<std::vector<long long>> &B,long long p){
    if (p== 0) throw std::invalid_argument("QuickPow(): modulus p must not be 0");
    int n=A.size();
    std::vector<std::vector<long long>> C(n,std::vector<long long>(n,0));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                C[i][j]=(C[i][j]+(__int128)A[i][k] * B[k][j])%p;
            }
        }
    }
    return C;
}

static inline std::vector<std::vector<long long>> MatrixMul(const std::vector<std::vector<long long>> &A,const std::vector<std::vector<long long>> &B){
    int n=A.size();
    std::vector<std::vector<long long>> C(n,std::vector<long long>(n,0));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                C[i][j]+=(__int128)A[i][k]*B[k][j];
            }
        }
    }
    return C;
}

inline std::vector<std::vector<long long>> QuickPow(const std::vector<std::vector<long long>> &A,long long b,long long p){
    if(p == 0) throw std::invalid_argument("QuickPow(): modulus p must not be 0");
    std::vector<std::vector<long long>> mar=A;
    int n=mar.size();
    std::vector<std::vector<long long>> res(n,std::vector<long long>(n,0));
    for(int i=0;i<n;i++){
        res[i][i]=1;
    }
    while(b){
        if(b&1) res=MatrixMul(res,mar,p);
        mar=MatrixMul(mar,mar,p);
        b>>=1;
    }
    return res;
}

inline std::vector<std::vector<long long>> QuickPow(const std::vector<std::vector<long long>> &A,long long b){
    std::vector<std::vector<long long>> mar=A;
    int n=mar.size();
    std::vector<std::vector<long long>> res(n,std::vector<long long>(n,0));
    for(int i=0;i<n;i++){
        res[i][i]=1;
    }
    while(b){
        if(b&1) res=MatrixMul(res,mar);
        mar=MatrixMul(mar,mar);
        b>>=1;
    }
    return res;
}

#endif