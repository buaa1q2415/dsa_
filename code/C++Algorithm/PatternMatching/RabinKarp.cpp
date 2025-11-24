#include <string>
#include <iostream>
#include <vector>

class RollingHash{
private:
    uint64_t base;
    uint64_t mod;

public:
    

};

class HashMatch{
private:
    std::vector<std::pair<uint64_t,uint64_t>> configs;
public:
    HashMatch(const std::string& text){
        configs.reserve(2);
    };
};