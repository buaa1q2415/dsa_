#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

typedef int Elemtype;

class SqList{
private:
    vector<Elemtype> data;
public:
    SqList(int size);
    ~SqList(){};
    void Show();
    void Insert(Elemtype value,int index=-1);
    int Search(Elemtype value);
    void Del(int index=-1);
    void Clear();
    bool IsEmpty();
    bool IsFull();
    Elemtype GetData(int index);
    int GetLen();
    int GetSize();
    void Resize(int size);
    void Reverse();
    void Unique();
    friend SqList* Merge(SqList* L1,SqList* L2);
};

SqList::SqList(int size){
    data.resize(size);
}

void SqList::Show() {
    for (auto it=data.begin();it!=data.end();it++){
        cout<<*it<<" ";
    }
    cout<<endl;
}

void SqList::Insert(Elemtype value,int index=-1){
    if(index==-1){
        data.push_back(value);
        return;
    }
    if(index<0||index>=data.size()){
        cout<<"index error"<<endl;
        return;
    }
    data.insert(data.begin()+index,value);
}

int SqList::Search(Elemtype value) {
    auto it=find(data.begin(), data.end(), value);
    if (it != data.end()) {
        return distance(data.begin(), it);
    }
    return -1;
}

void SqList::Del(int index=-1){
    if(index==-1){
        data.pop_back();
        return;
    }
    if(index<0||index>=data.size()){
        cout<<"index error"<<endl;
        return;
    }
    data.erase(data.begin()+index);
}

void SqList::Clear(){
    data.clear();
}

bool SqList::IsEmpty(){
    return data.empty();
}

bool SqList::IsFull(){
    return data.size()==data.capacity();
}

Elemtype SqList::GetData(int index){
    if(index<0||index>=data.size()){
        cout<<"index error"<<endl;
        return -1;
    }
    return data[index];
}

int SqList::GetLen(){
    return data.size();
}

int SqList::GetSize(){
    return data.capacity();
}

void SqList::Resize(int size){
    data.resize(size);
}

void SqList::Reverse(){
    reverse(data.begin(),data.end());
}

void SqList::Unique(){
    sort(data.begin(),data.end());
    auto newend=unique(data.begin(),data.end());
    data.erase(newend,data.end());
}

SqList* Merge(SqList* L1,SqList* L2){
    SqList* L=new SqList(L1->GetLen()+L2->GetLen());
    merge(L1->data.begin(),L1->data.end(),L2->data.begin(),L2->data.end(),back_inserter(L->data));
    return L;
}