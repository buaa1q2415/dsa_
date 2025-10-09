#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <stack>
#include <queue>

typedef char Vertextype;
typedef int Edgetype;

class ArcNode{
friend class Graph;
friend class VNode;
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
    std::vector<VNode> vertexs;
    int vexnum,arcnum;
    //int kind;//表示图的带权信息
public:
    void DFSTraverse();
    void BFSTraverse();
    void DFSPath(int v,int w);
};

void Graph::DFSTraverse(){
    std::vector<int> visited(vexnum,0);
    std::stack<int> s;
    int v;
    ArcNode* p=nullptr;
    for(int i=0;i<vexnum;i++){
        if(!visited[i]){
            s.emplace(i);//栈里储存待访问的顶点
            while(!s.empty()){
                v=s.top();
                s.pop();
                if(!visited[v]){
                    visited[v]=1;
                    //visitFunc(v);
                    p=vertexs[v].firstarc;
                    while(p){
                        if(!visited[p->vexindex]){
                            s.emplace(p->vexindex);
                        }
                        p=p->Nextarc();
                    }
                }
            }
        }
    }
}

void Graph::BFSTraverse(){
    std::vector<int> visited(vexnum,0);
    std::queue<int> q;
    int v;
    for(int i=0;i<vexnum;i++){
        if(!visited[i]){
            q.emplace(i);
            while(!q.empty()){
                v=q.front();
                visited[v]=1;
                q.pop();
                //visitFunc(v);
                ArcNode* arc=vertexs[v].firstarc;
                while(arc){
                    if(!visited[arc->vexindex]) q.emplace(arc->vexindex);
                    arc=arc->nextarc;
                }
            }
        }
    }
}

void Graph::DFSPath(int v,int w){
    std::vector<bool> visited(vexnum,false);
    std::stack<int> path;//存储路径
    path.emplace(v);
    visited[v]=true;
    int k;
    ArcNode* arc;
    while(!path.empty()){
        k=path.top();
        if(k==w){
            while(!path.empty()){
                std::cout<<path.top()<<" ";
                path.pop();
            }
            std::cout<<std::endl;
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
    std::cout<<"No path"<<std::endl;
}
