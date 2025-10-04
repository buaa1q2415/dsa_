#include <bits/stdc++.h>
using namespace std;

class hNode{
friend class HuffmanTree;
private:
    char data;
    int weight;
    hNode *left,*right;
public:
    hNode(char data,int weight):data(data),weight(weight),left(nullptr),right(nullptr){};
    hNode():left(nullptr),right(nullptr){};
    ~hNode()=default;
};

class HuffmanTree{
private:
    hNode *root;
public:
    HuffmanTree(string chars,vector<int> weights);
    HuffmanTree(string text);
    ~HuffmanTree();
};

HuffmanTree::HuffmanTree(string chars,vector<int> weights){
    priority_queue<pair<int,hNode*>,vector<pair<int,hNode*>>,greater<pair<int,hNode*>>> minHeap;
    for(size_t i=0;i<chars.size();++i){
        hNode* newNode=new hNode(chars[i],weights[i]);
        minHeap.emplace(weights[i],newNode);
    }
    while(minHeap.size()>1){
        auto left=minHeap.top();
        minHeap.pop();
        auto right=minHeap.top();
        minHeap.pop();
        hNode* parent=new hNode('\0',left.first+right.first);
        parent->left=left.second;
        parent->right=right.second;
        minHeap.emplace(parent->weight,parent);
    }
    root=minHeap.top().second;
}

HuffmanTree::HuffmanTree(string text){
    unordered_map<char,int> freqMap;
    for(char ch:text) ++freqMap[ch];
    priority_queue<pair<int,hNode*>,vector<pair<int,hNode*>>,greater<pair<int,hNode*>>> minHeap;
    for(auto& [ch,weight]:freqMap){
        hNode* newNode=new hNode(ch,weight);
        minHeap.emplace(weight,newNode);
    }
    while(minHeap.size()>1){
        auto left=minHeap.top();
        minHeap.pop();
        auto right=minHeap.top();
        minHeap.pop();
        hNode* parent=new hNode('\0',left.first+right.first);
        parent->left=left.second;
        parent->right=right.second;
        minHeap.emplace(parent->weight,parent);
    }
    root=minHeap.top().second;
}
HuffmanTree::~HuffmanTree(){
    if(root){
        std::stack<hNode*> stack;
        hNode* current = root;
        stack.emplace(current);
        while(!stack.empty()){
            current=stack.top();
            stack.pop();
            if(current->right) stack.emplace(current->right);
            if(current->left) stack.emplace(current->left);
            delete current;
        }
    }
}