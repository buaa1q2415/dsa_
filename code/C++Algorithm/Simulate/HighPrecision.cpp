#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <stdexcept>
using namespace std;

class LargeNumber{
private:
    bool symbol;
    vector<int> num;
    static inline const int KARATSUBA_THRESHOLD = 32;
    LargeNumber AbsAddHelper(const LargeNumber& n1,const LargeNumber& n2) const;
    LargeNumber AbsSubHelper(const LargeNumber& n1,const LargeNumber& n2) const;
    LargeNumber AbsMulHelper(const LargeNumber& n1,const LargeNumber& n2) const;
    LargeNumber AbsDivHelper(LargeNumber n1,LargeNumber n2) const;
    LargeNumber NormalMul(const LargeNumber& n1,const LargeNumber& n2) const;
    int AbsCmp(const LargeNumber& n1,const LargeNumber& n2) const;
public:
    LargeNumber(const string& str);
    LargeNumber():num({0}),symbol(true){};
    LargeNumber(long long n);

    LargeNumber operator-() const;
    LargeNumber operator+(const LargeNumber& n) const;
    LargeNumber operator-(const LargeNumber& n) const;
    bool operator==(const LargeNumber& n) const;
    bool operator<(const LargeNumber& n) const;
    bool operator>(const LargeNumber& n) const;
    bool operator<=(const LargeNumber& n) const;
    bool operator>=(const LargeNumber& n) const;
    LargeNumber operator*(const LargeNumber& n) const;
    LargeNumber operator/(const LargeNumber& n) const;
    LargeNumber operator%(const LargeNumber& n) const;

    LargeNumber& operator=(const LargeNumber& other);
    LargeNumber& operator=(LargeNumber&& other) noexcept;
    LargeNumber(const LargeNumber& other);
    LargeNumber(LargeNumber&& other) noexcept;

friend ostream& operator<<(ostream& os,const LargeNumber& n);
friend istream& operator>>(istream& is,LargeNumber& n);
friend LargeNumber abs(const LargeNumber& n);
};

LargeNumber::LargeNumber(const string& str){
    int r=str.size()-1,l=0;
    if(str[0]=='-'){
        l++;
        symbol=false;
    }else if(str[l]=='+'){
        l++;
        symbol=true;
    }else{
        symbol=true;
    }
    int block=0;
    int mask=0;
    while(r>=l){
        block=0;
        mask=1;
        for(int i=0;i<4 && r-i>=l;i++){
            block+=(str[r-i]-'0')*mask;
            mask*=10;
        }
        num.emplace_back(block);
        r-=4;
    }
}

LargeNumber& LargeNumber::operator=(const LargeNumber& other){
    if (this == &other) {
        return *this;
    }
    symbol=other.symbol;
    num=other.num;
    return *this;
}

LargeNumber& LargeNumber::operator=(LargeNumber&& other) noexcept{
    if (this == &other) {
        return *this;
    }
    symbol=other.symbol;
    num = move(other.num);
    return *this;
}

LargeNumber::LargeNumber(const LargeNumber& other){
    symbol=other.symbol;
    num=other.num;
}

LargeNumber::LargeNumber(LargeNumber&& other) noexcept{
    symbol=other.symbol;
    num = move(other.num);
}

LargeNumber::LargeNumber(long long n){
    if(n==0LL){
        num={0};
        symbol=1;
        return;
    }
    if(n<0){
        symbol=0;
        n=-n;
    }else symbol=1;
    num.resize(5,0);
    int i=0;
    while(n>0){
        num[i]=n%10000;
        n/=10000;
        i++;
    }
    while(num.back()==0) {
        num.pop_back();
    }
}

int LargeNumber::AbsCmp(const LargeNumber& n1,const LargeNumber& n2) const{
    int l1=n1.num.size(),l2=n2.num.size();
    if(l1<l2) return -1;
    else if(l1>l2) return 1;
    else{
        while(l1>=1){
            if(n1.num[l1-1]>n2.num[l1-1]) return 1;
            else if(n1.num[l1-1]<n2.num[l1-1]) return -1;
            else l1--;
        }
        return 0;
    }
}

LargeNumber LargeNumber::AbsAddHelper(const LargeNumber& n1,const LargeNumber& n2) const{
    LargeNumber ans;
    int l1=n1.num.size(),l2=n2.num.size(),max_len=max(l1, l2);
    ans.num.resize(max_len + 1, 0);
    int val1,val2,cursum=0;
    for (size_t i = 0; i < max_len;i++) {
        val1=(i<l1)? n1.num[i] : 0;
        val2=(i<l2)? n2.num[i] : 0;
        cursum=ans.num[i]+val1+val2;
        ans.num[i] = cursum%10000;
        ans.num[i+1] = cursum/10000;
    }
    while (ans.num.size()>1 && ans.num.back()==0) {
        ans.num.pop_back();
    }
    return ans;
}

LargeNumber LargeNumber::AbsSubHelper(const LargeNumber& n1,const LargeNumber& n2) const{
    LargeNumber ans;
    int cmp=AbsCmp(n1,n2);
    if(cmp==0){
        return ans;
    }
    int val1,val2;
    const LargeNumber& bigger=(cmp==1)? n1 : n2;
    const LargeNumber& smaller=(cmp==1)? n2 : n1;
    int l1=bigger.num.size(),l2=smaller.num.size();
    ans.num.resize(l1 + 1, 0);
    for (size_t i=0;i<l1;i++) {
        val1=(i<l1)? bigger.num[i] : 0;
        val2=(i<l2)? smaller.num[i] : 0;
        ans.num[i]=val1+ans.num[i]-val2;
        if(ans.num[i]<0){
            ans.num[i]+=10000;
            ans.num[i+1]=-1;
        }
    }
    while (ans.num.size()>1 && ans.num.back()==0) {
        ans.num.pop_back();
    }
    return ans;
}

LargeNumber LargeNumber::operator-() const{
    LargeNumber ans=*this;
    if(num.size()==1 && num[0]==0){
        return ans;
    }else{
        ans.symbol=1-ans.symbol;
        return ans;
    }
}

LargeNumber LargeNumber::operator+(const LargeNumber& n) const{
    LargeNumber ans;
    int cmp=AbsCmp(*this,n);
    if(cmp==0 && symbol+n.symbol==1) return ans;
    if(cmp==0 && num.size()==1 && num[0]==0) return ans;
    if(symbol==n.symbol){
        ans=AbsAddHelper(*this,n);
        ans.symbol=symbol;
    }else{
        ans=AbsSubHelper(*this,n);
        ans.symbol=(cmp==-1)? n.symbol:symbol;
    }
}

LargeNumber LargeNumber::operator-(const LargeNumber& n) const{
    LargeNumber ans;
    LargeNumber tmp=-n;
    return *this+tmp;
}

bool LargeNumber::operator==(const LargeNumber& n) const{
    if(symbol!=n.symbol) return false;
    else return (AbsCmp(*this,n)==0);
}

bool operator==(const long long lhs, const LargeNumber& rhs) {
    return rhs == LargeNumber(lhs);
}

bool LargeNumber::operator<(const LargeNumber& n) const{
    if(symbol!=n.symbol) return (symbol==1)?false:true;
    else{
        int cmp=AbsCmp(*this,n);
        if((symbol==1 && cmp==-1) || (symbol==0 && cmp==1)) return true;
        else return false;
    }
}

bool LargeNumber::operator>(const LargeNumber& n) const{
    return n<*this;
}

bool LargeNumber::operator<=(const LargeNumber& n) const{
    return 1-(*this>n);
}

bool LargeNumber::operator>=(const LargeNumber& n) const{
    return 1-(*this<n);
}

bool operator<(const long long t,const LargeNumber& n){
    return n>t;
}

bool operator>(const long long t,const LargeNumber& n){
    return n<t;
}

bool operator<=(const long long t,const LargeNumber& n){
    return n>=t;
}

bool operator>=(const long long t,const LargeNumber& n) {
    return n<=t;
}

LargeNumber LargeNumber::NormalMul(const LargeNumber& n1,const LargeNumber& n2) const{
    LargeNumber ans;
    int l1 = n1.num.size();
    int l2 = n2.num.size();
    ans.num.resize(l1 + l2, 0);
    for (int i = 0; i < l1; ++i) {
        for (int j = 0; j < l2; ++j) {
            ans.num[i+j] += n1.num[i]*n2.num[j];
            ans.num[i+j+1] += ans.num[i+j]/10000;
            ans.num[i+j] %= 10000;
        }
    }
    while (ans.num.size() > 1 && ans.num.back() == 0) {
        ans.num.pop_back();
    }
    return ans;
}

LargeNumber LargeNumber::AbsMulHelper(const LargeNumber& n1,const LargeNumber& n2) const{
    int l1 = n1.num.size();
    int l2 = n2.num.size();
    // 递归终点
    if (l1<KARATSUBA_THRESHOLD || l2<KARATSUBA_THRESHOLD) return NormalMul(n1, n2);
    // 分割
    int k = max(l1, l2) / 2;
    LargeNumber a,b,c,d;
    b.num=vector<int>(n1.num.begin(), n1.num.begin() + min(k, l1));
    a.num=vector<int>(n1.num.begin() + min(k, l1), n1.num.end());
    d.num=vector<int>(n2.num.begin(), n2.num.begin() + min(k, l2));
    c.num=vector<int>(n2.num.begin() + min(k, l2), n2.num.end());
    // 递归计算
    LargeNumber z2 = AbsMulHelper(a, c);
    LargeNumber z0 = AbsMulHelper(b, d);
    LargeNumber z1_help = AbsMulHelper(a + b, c + d);
    LargeNumber z1 = z1_help - z2 - z0;
    // 合并结果
    z2.num.insert(z2.num.begin(), 2 * k, 0);
    z1.num.insert(z1.num.begin(), k, 0);
    LargeNumber result = z2 + z1 + z0;
    // 清理前导零
    while (result.num.size() > 1 && result.num.back() == 0) {
        result.num.pop_back();
    }
    return result;
}

LargeNumber LargeNumber::operator*(const LargeNumber& n) const{
    LargeNumber ans;
    if(n==0 || *this==0){
        return ans;
    }
    ans = AbsMulHelper(*this, n);
    ans.symbol = (symbol == n.symbol);
}

ostream& operator<<(ostream& os,const LargeNumber& n){
    if(n.symbol==0) os<<'-';
    os<<n.num.back();
    for(int i=n.num.size()-2;i>=0;i++) os<<setw(4)<<setfill('0')<<n.num[i];
    return os;
}

istream& operator>>(istream& is,LargeNumber& n){
    string s;
    is>>s;
    n=s;
    return is;
}

LargeNumber abs(const LargeNumber& n){
    LargeNumber ans=n;
    ans.symbol=1;
    return ans;
}

LargeNumber LargeNumber::AbsDivHelper(LargeNumber n1,LargeNumber n2) const{
    // Todo:补充完整
}

LargeNumber LargeNumber::operator/(const LargeNumber& n) const{
    if(n==0) throw invalid_argument("Division by zero");
    if(*this==0) return LargeNumber(0);
    else{
        LargeNumber ans=AbsDivHelper(abs(*this),abs(n));
        ans.symbol=(this->symbol==n.symbol);
        return ans;
    }
}

LargeNumber LargeNumber::operator%(const LargeNumber& n) const{
    // Todo:补充完整
}

