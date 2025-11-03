#include <iostream>
#include <string>
#include <vector>

class KMP{
private:
    void getNext(std::string& pattern, std::vector<int>& next) {
        int m=pattern.size();
        next[0]=-1;
        int i=0,j=-1;
        while(i<m-1){
            if(j==-1 || pattern[i]==pattern[j]){
                i++,j++;
                next[i]=j;
            }else{
                j=next[j];
            }
        }
    }
public:
    int Match(std::string& text, std::string& pattern) {
        int n=text.size(), m=pattern.size();
        if (n<m || m==0) return -1;
        std::vector<int> next(m,-1);
        getNext(pattern, next);
        int i=0,j=0;
        while (i<n && j<m) {
            if (j==-1 || text[i]==pattern[j]){
                i++,j++;
            }else{
                j=next[j];
            }
        }
        return (j==m)? i-j : -1;
    }

    std::vector<int> MatchAll(std::string& text, std::string& pattern) {
        std::vector<int> res;
        int n=text.size(), m=pattern.size();
        if (m==0) return {}; // 处理空模式字符串
        if (n<m && n!=0) return {}; // 如果文本短于模式且不为空
        if (n==0 && m!=0) return {}; // 如果文本为空且模式不为空
        std::vector<int> next(m,-1);
        getNext(pattern, next);
        int i=0,j=0;
        while (i<n && j<m) {
            if (j==-1 || text[i]==pattern[j]){
                i++,j++;
            }else{
                j=next[j];
            }
            if(j==m){
            res.emplace_back(i-j,i-1);
            j=next[m-1];
        }
        }
        return res;
    }
};