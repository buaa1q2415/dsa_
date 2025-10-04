#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstddef>
#include <stack>
#include <queue>

using namespace std;

typedef int Elemtype;
typedef char Vertextype;
typedef int Edgetype;

class ArcNode{
private:
    int vexindex;//弧所指向顶点的位置
    ArcNode *nextarc;//指向下一条弧的指针
    //Edgetype info;//弧的权值
public:
    int GetVex(){
        return vexindex;
    }
    ArcNode* Nextarc(){
        return nextarc;
    }
};

class VNode{
friend class Graph;
private:
    Vertextype data;
    ArcNode *firstarc;
public:
};

class Graph{
private:
    vector<VNode> vertexs;
    int vexnum,arcnum;
    //int kind;//表示图的带权信息
public:
    void DFS(int v,vector<bool> &visited);
    void DFSTraverse();
    void BFSTraverse();
    void DFSPath(int v,int w);
};

void Graph::DFS(int v,vector<bool> &visited){
    visited[v]=true;
    //visitFunc(v);
    ArcNode *p=vertexs[v].firstarc;
    while(p){
        if(!visited[p->GetVex()]){
            DFS(p->GetVex(),visited);
        }
        p=p->Nextarc();
    }
}

void Graph::DFSTraverse(){
    vector<bool> visited(vexnum,false);
    stack<int> s;
    for(int i=0;i<vexnum;i++){
        if(!visited[i]){

        }
    }
}

void Graph::BFSTraverse(){
    vector<bool> visited(vexnum,false);
    queue<int> q;
    int v;
    for(int i=0;i<vexnum;i++){
        if(!visited[i]){
            q.emplace(i);
            while(!q.empty()){
                v=q.front();
                visited[v]=true;
                q.pop();
                //visitFunc(v);
                ArcNode* arc=vertexs[v].firstarc;
                while(arc){
                    if(!visited[arc->GetVex()]) q.emplace(arc->GetVex());
                    arc=arc->Nextarc();
                }
            }
        }
    }
}

void Graph::DFSPath(int v,int w){
    vector<bool> visited(vexnum,false);
    stack<int> path;
    path.emplace(v);
    visited[v]=true;
    int k;
    ArcNode* arc;
    while(!path.empty()){
        k=path.top();
        if(k==w){
            while(!path.empty()){
                cout<<path.top()<<" ";
                path.pop();
            }
            cout<<endl;
            return;
        }
        arc=vertexs[k].firstarc;
        while(arc){
            if(!visited[arc->GetVex()]){
                path.emplace(arc->GetVex());
                visited[arc->GetVex()]=true;
                break;
            }
            arc=arc->Nextarc();
        }
        if(arc==nullptr) path.pop();
    }
    cout<<"No path"<<endl;
}
