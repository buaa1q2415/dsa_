#include <string>
#include <vector>
#include <cstdint>
#include <string_view>
#include <type_traits>

class HashMatch{
private:
    std::vector<std::pair<uint64_t,uint64_t>> configs;

    template <typename T, typename U, typename... Args>
    void InsertHash(const std::pair<T,U>& config,const Args&... rest){
        configs.emplace_back(static_cast<std::pair<uint64_t,uint64_t>>(config));
        if constexpr (sizeof...(Args) > 0){
            InsertHash(rest...);
        }
    }

public:
    HashMatch(){
        configs.reserve(2);
        configs.emplace_back(std::make_pair(131ULL,1000000007ULL));
        configs.emplace_back(std::make_pair(257ULL,1000000009ULL));
    }

    template <typename T, typename U, typename... Args>
    HashMatch(const std::pair<T,U>& config,const Args&... rest){
        static_assert((std::is_integral<T> && std::is_integral<U>),"请确认基数和模数均为整数类型!");
        InsertHash(config,rest...);
    };

    int find(const std::string& text,const std::string& pattern) const{
        int n = text.size(),m = pattern.size();
        if (m == 0) return 0;
        if (n < m) return -1;
        if(n==m) return (text==pattern)-1;
        int num_hashes = configs.size();
        std::vector<uint64_t> pattern_hashes(num_hashes,0),text_hashes(num_hashes,0),powers(num_hashes,1);

        for (int k=0; k<num_hashes; ++k) {// 计算模式串和文本初始窗口的哈希值
            const auto [base,mod] = configs[k];

            for (int i=0; i<m-1; ++i) {
                powers[k] = (powers[k]*base) % mod;
            }
            for (int i=0; i<m; ++i) {
                pattern_hashes[k] = (pattern_hashes[k]*base + static_cast<uint64_t>(pattern[i])) % mod;
                text_hashes[k] = (text_hashes[k]*base + static_cast<uint64_t>(text[i])) % mod;
            }
        }

        // 滑动窗口
        for (int i=0;i<n-m;++i){
            bool all_match = true;
            for(int k=0; k<num_hashes; ++k) {
                if (text_hashes[k] != pattern_hashes[k]) {
                    all_match = false;
                    break;
                }
            }
            
            if (all_match && (std::string_view(text.data() + i, m)==pattern)) {// 防止哈希冲突
                return i;
            }

            for (int k=0; k<num_hashes; ++k) { // 滚动哈希更新
                const auto [base,mod] = configs[k];
                uint64_t remove = (powers[k] * static_cast<uint64_t>(text[i])) % mod;
                text_hashes[k] = (text_hashes[k] - remove + mod) % mod;
                text_hashes[k] = (text_hashes[k] * base + static_cast<uint64_t>(text[i + m])) % mod;
            }
        }
        bool all_match = true;
        for(int k=0; k<num_hashes; ++k) {
            if (text_hashes[k] != pattern_hashes[k]) {
                all_match = false;
                break;
            }
        }
        if (all_match && (std::string_view(text.data()+n-m,m)==pattern)) return n-m;
        return -1;
    }
};