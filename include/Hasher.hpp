#ifndef HASHER_HPP
#define HASHER_HPP

#include <cstdint>
#include <string>

uint64_t polynomialRolling(const std::string &s, uint64_t base){
    uint64_t key=0;
    for (unsigned char c: s){
        key= key*base + (uint64_t)(c - '0' + 1);
    }
    return key;
}

uint64_t splitMix64(uint64_t key){
    key += 0x9e3779b97f4a7c15ULL;
    key = (key ^ (key >> 30)) * 0xbf58476d1ce4e5b9ULL;
    key = (key ^ (key >> 27)) * 0x94d049bb133111ebULL;
    key = key ^ (key >> 31);
    return key;
}

struct Hasher
{
    uint64_t base;

    Hasher(uint64_t base=131): base(base) {}

    size_t calculateKey(const std::string &s){
        uint64_t key = polynomialRolling(s,base);
        return (size_t) splitMix64(key);
    }
};

#endif