#include <bits/stdc++.h>
using namespace std;

class LRUCache {
public:
    LRUCache(int capacity) : cap(capacity) {
        
    }
    
    int get(int key) {
        
        auto it = _hash.find(key);
        if(it != _hash.end())
        {
            int val = _data[it->second].second;
            updateLR(it->second);
        }

    }
    
    void put(int key, int value) {
        
    }

    void updateLR(int index)
    {
        _data.erase(_data.begin() + index);
        
    }

    void deleteLR()
    {

    }

    private:
    size_t cap;
    deque<pair<int, int>> _data;
    unordered_map<int, int> _hash;
};