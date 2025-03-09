#include <bits/stdc++.h>
#include <thread>
#include <condition_variable>
using namespace std;
/*
- thread safe queue using mutex & conditional variable, apart from special member functions & swap, 3 types of ops.
    1. query state of whole queue. size, empty
    2. query elements. front, back
    3. modify queue. push, pop, emplace.
    this interface has inherent race condition, using front & pop as seperate functions.
{
    auto t = q.front() -> 2  threads executing same code & 1 thread switched after exec front. 1 element will be ignored 
q.pop();
}
    - so combining pop & front in wait_pop & try_pop is required.
*/
template <typename T>
class TSQ
{
    public:
    TSQ() = default;
    TSQ(const TSQ& oth)
    {
        lock_guard<mutex> lk(oth.mx);
        dq = oth.dq;
    }

    TSQ& operator=(const TSQ& oth) = delete;
    
    size_t size() const
    {
        lock_guard<mutex> lk(mx);
        return dq.size();
    }

    bool empty() const
    {
        lock_guard<mutex> lk(mx);
        return (dq.empty());
    }

    void push(const T& val)
    {
        {
            lock_guard<mutex> lk(mx);
            dq.push(val);
        }
        cd.notify_one();
    }

    void push(T&& val)
    {
        {
            lock_guard<mutex> lk(mx);
            dq.push(move(val));
        }
        cd.notify_one();
    }

    void wait_and_pop(T& val)
    {
        unique_lock<mutex> ul(mx);
        cd.wait(ul, [this](){ return !dq.empty();});
        val = dq.front();
        dq.pop();
    }

    shared_ptr<T> wait_and_pop()
    {
        unique_lock<mutex> ul(mx);
        cd.wait(ul, [this](){ return !dq.empty();});
        auto res = make_shared<T>(dq.front());
        dq.pop();
        return res;
    }

    void try_and_pop(T& val)
    {
        lock_guard<mutex> lk(mx);
        if(dq.empty())
            return;
        val = dq.front();
        dq.pop();
    }

    shared_ptr<T> try_and_pop()
    {
        lock_guard<mutex> lg(mx);
        if(dq.empty())
            return nullptr;
        auto res = make_shared<T>(dq.front());
        dq.pop();
        return res;
    }

    private:
    mutable mutex mx;
    condition_variable cd;
    queue<T> dq;
};