#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <bit>
#include <functional>

/* 实现ST表（Sparse Table）数据结构，用于解决区间查询问题（如RMQ）
 * 支持自定义比较函数，默认是std::less<int64_t>，即求区间最小值
 * 支持的查询操作：查询区间[left,right]的最小值（或最大值，根据Compare函数）
 * 时间复杂度：O(nlogn)，预处理时间，O(1)查询时间
 * 空间复杂度：O(nlogn)
 */

template<class Compare = std::less<int64_t>>
class StTable{
    std::vector<std::vector<int64_t>> table;
    Compare cmp;
public:
    StTable(const std::vector<int64_t>& arr) {
        int n=arr.size();
        int h=std::bit_width(static_cast<uint64_t>(n));

        // 初始化ST表
        table.resize(h);
        for(int i=0;i<h; ++i){
            table[i].resize(n+1-(1<<i));
        }

        // 初始化ST表的第0层
        for(int i=0;i<n; ++i){
            table[0][i]=arr[i];
        }

        // dp初始化ST表的其他层
        for(int i=1;i<h; ++i){
            for(int j=0;j+(1<<i)<=n; ++j){
                table[i][j]=cmp(table[i-1][j],table[i-1][j+(1<<(i-1))])? table[i-1][j] : table[i-1][j+(1<<(i-1))];
            }
        }
    }

    ~StTable()=default;

    StTable& operator=(const StTable& other)=default;
    StTable& operator=(StTable&& other)=default;
    StTable(const StTable& other)=default;
    StTable(StTable&& other)=default;
    

    int64_t query(int left,int right) {
        // 计算log2(right-left+1)
        int h=std::bit_width(static_cast<uint64_t>(right-left+1))-1; 
        return cmp(table[h][left],table[h][right-(1<<h)+1])? table[h][left] : table[h][right-(1<<h)+1];
    }
};