/* 快速幂
 * 计算a^b mod p
 * 时间复杂度O(logb)
 * 快速幂的原理是利用二进制拆分指数，如果指数的二进制表示为1011，那么
 * a^0b1011=(a^0b1000)*(a^0b0000)*(a^0b0010)*(a^0b0001)
 * 从右往左遍历指数的二进制位，如果第i位(i>=0)为1，则将结果乘上a^(2^i)
 */
#include <iostream>
#include <vector>

using namespace std;

long long QuickPow(long long a,long long b,long long p){
    long long res=1;
    a%=p;
    while(b){
        if(b&1) res=(res*a)%p;
        a=(a*a)%p;
        b>>=1;
    }
    return res;
}

//矩阵快速幂
vector<vector<long long>> MatrixMul(vector<vector<long long>> &A,vector<vector<long long>> &B,long long p){
    int n=A.size();
    vector<vector<long long>> C(n,vector<long long>(n,0));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                C[i][j]=(A[i][k]*B[k][j])%p;
            }
        }
    }
    return C;
}

vector<vector<long long>> MatrixQuickPow(vector<vector<long long>> &A,long long b,long long p){
    int n=A.size();
    vector<vector<long long>> res(n,vector<long long>(n,0));
    for(int i=0;i<n;i++){
        res[i][i]=1;
    }
    while(b){
        if(b&1) res=MatrixMul(res,A,p);
        A=MatrixMul(A,A,p);
        b>>=1;
    }
    return res;
}
