#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include <stddef.h>

using namespace std;

typedef int Elemtype;

class Triple{
friend class TSMatrix;
friend TSMatrix operator*(const TSMatrix& T1,const TSMatrix& T2);
private:
    int i,j;//下标
    int e;//元素值
public:
    Triple(int i,int j,int e):i(i),j(j),e(e){};
    ~Triple(){};
    bool operator==(const Triple& other);
};

class TSMatrix{
private:
    vector<Triple> data;
    int mu,nu,tu;//矩阵的行数，列数，非零元素的个数
public:
    TSMatrix(const vector<vector<int>>& arr);
    TSMatrix(int mu,int nu,int tu);
    ~TSMatrix(){};
    vector<int> RowFirst() const;
    TSMatrix Transpose();
    bool operator==(const TSMatrix& other);
    friend TSMatrix operator*(const TSMatrix& T1,const TSMatrix& T2);
};

bool Triple::operator==(const Triple& other){
    return i==other.i&&j==other.j&&e==other.e;
}

TSMatrix::TSMatrix(const vector<vector<int>>& arr) {
    if (arr.empty()) {// 检查数组是否为空
        mu = 0,nu = 0,tu = 0;
        return;
    }
    mu = arr.size(),nu = arr[0].size(),tu = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
        for (size_t j = 0; j < arr[i].size(); ++j) {
            if (arr[i][j] != 0) {
                data.emplace_back(i, j, arr[i][j]);
                ++tu;
            }
        }
    }
}

TSMatrix::TSMatrix(int mu, int nu, int tu){
    this->mu=mu;
    this->nu=nu;
    this->tu=tu;
    data.resize(tu);
}

vector<int> TSMatrix::RowFirst() const{
    vector<int> rowfirst(mu,0),temp(2);
    for(int i=0;i<tu;++i){
        rowfirst[data[i].i]++;
    }
    temp[0]=rowfirst[0];
    rowfirst[0]=0;
    for(int i=1;i<mu;i++){
        temp[i&1]=rowfirst[i];
        rowfirst[i]=rowfirst[i-1]+temp[(i-1)&1];
    }
    return rowfirst;
}

TSMatrix TSMatrix::Transpose() {
    TSMatrix T=TSMatrix(nu,mu,tu);
    if(tu==0) return T;
    vector<int> colpot(nu,0),temp(2);
    for(int i=0;i<tu;++i){
        ++colpot[data[i].j];
    }
    temp[0]=colpot[0];
    colpot[0]=0;
    for(int i=1;i<nu;i++){
        temp[i&1]=colpot[i];
        colpot[i]=colpot[i-1]+temp[(i-1)&1];
    }
    for(int k=0;k<tu;k++){
        T.data[colpot[data[k].j]]=data[k];
        T.data[colpot[data[k].j]].i=data[k].j;
        T.data[colpot[data[k].j]].j=data[k].i;
        colpot[data[k].j]++;
    }
    return T;
}

bool TSMatrix::operator==(const TSMatrix& other){
    if(mu!=other.mu||nu!=other.nu||tu!=other.tu) return false;
    for(int i=0;i<tu;i++){
        if(!(data[i]==other.data[i])) return false;
    }
    return true;
}

TSMatrix operator*(const TSMatrix& T1,const TSMatrix& T2){
    if(T1.nu!=T2.mu){
        cout<<"multiple error"<<endl;
        return TSMatrix(0,0,0);
    }
    TSMatrix T=TSMatrix(T1.mu,T2.nu,0);
    vector<int> rowfirst1=T1.RowFirst();
    vector<int> rowfirst2=T2.RowFirst();
    if(T1.tu*T2.tu!=0){
        vector<int> temp3;
        int tp,t,col3;
        for(int row1=0;row1<T1.mu;row1++){
            temp3.resize(T2.nu,0);
            if(row1<T1.mu) tp=rowfirst1[row1+1];
            else tp=T1.tu+1;
            for(int p=rowfirst1[row1];p<tp;p++){
                int row2=T1.data[p].j;
                if(row2<T2.mu) t=rowfirst2[row2+1];
                else t=T2.tu+1;
                for(int q=rowfirst2[row2];q<t;q++){
                    col3=T2.data[q].j;
                    temp3[col3]+=T1.data[p].e*T2.data[q].e;
                }
            }
            for(col3=0;col3<T.nu;col3++){
                if(temp3[col3]){
                    T.data[T.tu].i=row1;
                    T.data[T.tu].j=col3;
                    T.data[T.tu].e=temp3[col3];
                    T.tu++;
                }
            }
        }
    }
    return T;
}