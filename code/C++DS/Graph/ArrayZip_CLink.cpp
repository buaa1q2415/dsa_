#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include <stddef.h>

using namespace std;

class CrossLink;

class CNode{
friend class CrossLink;
friend ostream& operator<<(ostream& os,const CrossLink& link);
friend istream& operator>>(istream& is,CrossLink& link);
private:
    int i,j;//下标
    long long e;//元素值
    CNode *right,*down;//右指针，下指针
public:
    CNode(int i,int j,long long e){
        this->i=i,this->j=j,this->e=e;
        right=nullptr,down=nullptr;
    }
    CNode(int i,int j,long long e,CNode *right,CNode *down){
        this->i=i,this->j=j,this->e=e;
        this->right=right,this->down=down;
    }
    CNode(int i,int j){
        this->i=i,this->j=j;
        this->right=nullptr,this->down=nullptr;
    }
    CNode(int i,int j,CNode *right,CNode *down){
        this->i=i,this->j=j;
        this->right=right,this->down=down;
    }
    CNode(const CNode& node,int mode=0){
        this->i=node.i,this->j=node.j,this->e=node.e;
        if(mode==0){
            this->right=node.right,this->down=node.down;
        }
        else{
            this->right=nullptr,this->down=nullptr;
        }
    }
    CNode& operator=(const CNode& node){
        this->i=node.i,this->j=node.j,this->e=node.e;
        this->right=node.right,this->down=node.down;
        return *this;
    }
    long long getElem(){return e;}
    int getRow(){return i;}
    int getCol(){return j;}
};

class CrossLink{
private:
    vector<CNode*> rhead,chead;
    int row,col; // 行数，列数
    int elem; // 非零元素个数
public:
    CrossLink(int row,int col){
        this->row=row,this->col=col,elem=0;
        rhead.resize(row,nullptr);
        chead.resize(col,nullptr);
    }
    CrossLink(vector<vector<long long>>& arr);
    ~CrossLink();

    CrossLink(const CrossLink& link);
    CrossLink(CrossLink&& link) noexcept;
    CrossLink& operator=(const CrossLink& link);
    CrossLink& operator=(CrossLink&& link) noexcept;

    CrossLink& Add(const CrossLink& link1,const CrossLink& link2);
    void Print();

    CrossLink operator+(const CrossLink& link);
friend ostream& operator<<(ostream& os,const CrossLink& link);
friend istream& operator>>(istream& is,CrossLink& link);
};

CrossLink::CrossLink(vector<vector<long long>>& arr){
    row=arr.size(),col=arr[0].size(),elem=0;
    rhead.resize(row,nullptr);
    chead.resize(col,nullptr);
    vector<CNode*> ctail(col,nullptr);
    for(int i=0;i<row;i++){
        rhead[i]=new CNode(i,0);
    }
    for(int j=0;j<col;j++){
        chead[j]=new CNode(0,j);
        ctail[j]=chead[j];
    }
    CNode* node=nullptr;
    CNode* rtail=nullptr;
    for(int i=0;i<row;i++){
        rtail=rhead[i];
        for(int j=0;j<col;j++){
            if(arr[i][j]!=0){
                elem++;
                node=new CNode(i,j,arr[i][j]);
                ctail[j]->down=node;
                ctail[j]=node;
                rtail->right=node;
                rtail=node;
            }
        }
    }
}

CrossLink CrossLink::operator+(const CrossLink& link){
    if(row!=link.row||col!=link.col){
        throw "size not equal";
    }
    CrossLink res(row,col);
    vector<CNode*> ctail(col,nullptr);
    for(int i=0;i<row;i++){
        res.rhead[i]=new CNode(i,0);
    }
    for(int j=0;j<col;j++){
        res.chead[j]=new CNode(0,j);
        ctail[j]=res.chead[j];
    }
    CNode* node=nullptr,*rtail=nullptr,*p=nullptr,*q=nullptr,*r=nullptr;
    for(int i=0;i<row;i++){
        rtail=res.rhead[i];
        p=rhead[i]->right,q=link.rhead[i]->right;
        while(p && q){
            if(p->j<q->j){
                r=new CNode(*p,1);
                rtail->right=r;
                rtail=r;
                p=p->right;
                ctail[r->j]->down=r;
                ctail[r->j]=r;
                res.elem++;
            }else if(p->j>q->j){
                r=new CNode(*q,1);
                rtail->right=r;
                rtail=r;
                q=q->right;
                ctail[r->j]->down=r;
                ctail[r->j]=r;
                res.elem++;
            }else if(p->e+q->e==0){
                p=p->right,q=q->right;
            }else{
                r=new CNode(*p,1);
                r->e=p->e+q->e;
                rtail->right=r;
                rtail=r;
                p=p->right,q=q->right;
                ctail[r->j]->down=r;
                ctail[r->j]=r;
                res.elem++;
            }
        }
        while(p){
            r=new CNode(*p,1);
            rtail->right=r;
            rtail=r;
            p=p->right;
            ctail[r->j]->down=r;
            ctail[r->j]=r;
            res.elem++;
        }
        while(q){
            r=new CNode(*q,1);
            rtail->right=r;
            rtail=r;
            q=q->right;
            ctail[r->j]->down=r;
            ctail[r->j]=r;
            res.elem++;
        }
    }
    return res;
}

CrossLink::~CrossLink(){
    CNode* p=nullptr;
    for(int i=0;i<row;i++){
        p=rhead[i];
        while(p){
            CNode* q=p;
            p=p->right;
            delete q;
        }
    }
    for(int j=0;j<col;j++){
        delete chead[j];
    }
}

void CrossLink::Print(){
    CNode* p=nullptr;
    int index;
    for(int i=0;i<row;i++){
        index=0;
        p=rhead[i]->right;
        while(index<col){
            if(p && p->j==index){
                cout<<p->e<<" ";
                p=p->right;
            }else{
                cout<<"0 ";
            }
            index++;
        }
        cout<<endl;
    }
}

ostream& operator<<(ostream& os,const CrossLink& link){
    CNode* p=nullptr;
    int index;
    for(int i=0;i<link.row;i++){
        index=0;
        p=link.rhead[i]->right;
        while(index<link.col){
            if(p && p->j==index){
                os<<p->e<<" ";
                p=p->right;
            }else{
                os<<"0 ";
            }
            index++;
        }
        os<<endl;
    }
    return os;
}

