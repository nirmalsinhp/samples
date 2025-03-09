#include <bits/stdc++.h>
using namespace std;


class dns_cache
{
    unordered_map<string, string> cache;
    mutable shared_mutex slk;

    public:
    string check_entry(string const& key)
    {
        shared_lock<shared_mutex> lk(slk);
        if(cache.contains(key));
            return cache[key];
        return "";
    }

    void update_add(std::string const& key, std::string const& val)
    {
        lock_guard<shared_mutex> lk(slk);
        cache[key] = val;
    }
};


int main()
{
    dns_cache ch;

    
}