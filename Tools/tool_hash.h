#ifndef TOOL_HASH_H
#define TOOL_HASH_H
#include <iostream>
#include<algorithm>
#include <list>
const int NULLKEY = -1;

template<class Key, class Val>
class HashBase
{
public:
    Key key;
    Val value;
    HashBase(Key k, Val v) {
        key = k;
        value = v;
    }
    HashBase() {
        key = NULLKEY;
    }
};
template<class Key, class Val>
class HashUnit: public HashBase<Key, Val>
{
public:
    std::list<HashBase<Key, Val>> hor;
    HashUnit(Key k, Val v) {
        this->key = k;
        this->value = v;
    }
    HashUnit() {
        this->key = NULLKEY;
    }
};

template<class Key, class Val>
class MyHash
{
public:
    std::list<HashUnit<Key, Val>> ma;
    MyHash();
    void Push(Key k, Val v)
    {
        ma.push_back(k, v);
        std::sort(ma.begin(), ma.end());
    }
};

#endif // TOOL_HASH_H
