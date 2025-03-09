/*
- At the basic level, designing a data structure for concurrency means that multiple threads can access the data structure concurrently, either performing the same or distinct operations, and each thread will see a self-consistent view of the data structure. No data will be lost or corrupted, all invariants will be upheld, and there’ll be no problematic race conditions. This data structure is said to be thread-safe.
- mutex provides exclusive access, so it promotes serialization/ serial execution of operations. the smaller the protected region, the fewer operations are serialized, and the greater the potential for concurrency.
- ensuring access are safe and genuine concurrency enabling.
- Ensure that no thread can see a state where the invariants of the data structure have been broken by the actions of another thread.
- Take care to avoid race conditions inherent in the interface to the data structure by providing functions for complete operations rather than for operation steps.
- Pay attention to how the data structure behaves in the presence of exceptions to ensure that the invariants are not broken.
- Minimize the opportunities for deadlock when using the data structure by restricting the scope of locks and avoiding nested locks where possible.
- what kind of constraint you want to put on user of ds.
# lock based
- the design of lock-based concurrent data structures is all about ensuring that the right mutex is locked when accessing the data and that the lock is held for the minimum amount of time.
-  the interfaces of the standard containers aren’t suitable when the data structure is to be accessed from multiple threads concurrently, because there are inherent race conditions in the interface design, so they need to be cut down and revised.
- The basic issue with STL-style iterator support is that the iterator must hold some kind of reference into the internal data structure of the container. If the container can be modified from another thread, this reference must somehow remain valid, which requires that the iterator hold a lock on some part of the structure. Given that the lifetime of an STL-style iterator is completely outside the control of the container, this is a bad idea.
 */
#include <bits/stdc++.h>
#include <mutex>
#include <stack>
#include <string>
#include <condition_variable>
#include <thread>
#include <queue>
#include <algorithm>


using namespace std;

template <typename T>
class ts_stack
{
private:
    stack<T> data;
    mutex mtx;
public:
    template <typename U>
    friend void swap(ts_stack<U>& t1, ts_stack<U>& t2);
    ts_stack() = default;
    ts_stack(const ts_stack& oth)
    {
        lock_guard lk(mtx);
        data = oth.data;
    }
    ts_stack operator =(const ts_stack& oth) = delete;

    void push(T val)
    {
        lock_guard lk(mtx);
        data.push(move(val));
    }
    shared_ptr<T> pop()
    {
        lock_guard lk(mtx);
        if (empty())
            throw exception("empty");
        auto sp = make_shared<T>(data.top());
        data.pop();
        return sp;
    }

    void pop(T& val)
    {
        lock_guard lk(mtx);
        if (empty())
            throw exception("empty");
        val = data.top();
        data.pop();
    }


    bool empty()
    {
        lock_guard lk(mtx);
        return data.empty();
    }
};

template <typename T>
void swap(ts_stack<T>& s1, ts_stack<T>& s2)
{
    using std::swap;
    /* same as scoped lock.
        lock(s1.mtx, s2.mtx);
        lock_guard lk1(s1.mtx, adopt_lock);
        lock_guard lk2(s2.mtx, adopt_lock);
        */

    unique_lock<mutex> uk1(s1.mtx, defer_lock);
    unique_lock<mutex> uk2(s2.mtx, defer_lock);
    lock(s1.mtx, s2.mtx);

    //scoped_lock sl(s1.mtx, s2.mtx); // prefer this 
    std::swap(s1.data, s2.data);
}



// functions returning shared_ptr can throw exception, which can lead to thread begin awaked but not able to consume data, and no other thread is notified, in this case, one solution to this is, to create queue of shared_ptr<T> instead of T.
template <typename T>
class ts_queue
{
    mutable mutex mut;
    queue<T> _data;
    condition_variable cv;

public:
    ts_queue() = default;
    ts_queue(const ts_queue& oth)
    {
        lock_guard<mutex> lk(mut);
        _data = oth._data;
    }

    ts_queue& operator=(const ts_queue& oth) = delete;
    bool empty() const
    {
        lock_guard<mutex> lk(mut);
        return _data.empty();
    }

    void push(T val)
    {
        lock_guard<mutex> lk(mut);
        _data.push(move(val));
        cv.notify_one();
    }

    void wait_pop(T& val)
    {
        unique_lock<mutex> ul(mut);
        cv.wait(ul, [this]()
            {
                return !_data.empty();
            });
        val = move(_data.front());
        _data.pop();
    }

    shared_ptr<T> wait_pop()
    {
        unique_lock<mutex> ul(mut);
        cv.wait(ul, [this]()
            {
                return !_data.empty();
            });
        shared_ptr<T> sp(make_shared<T>(_data.front()));
        _data.pop();
        return sp;
    }
    bool try_pop(T& val)
    {
        lock_guard<mutex> lk(mut);
        if (_data.empty())
            return false;
        val = move(_data.front());
        _data.pop();
        return true;
    }

    shared_ptr<T> try_pop()
    {
        lock_guard<mutex> lk(mut);
        if (_data.empty())
            return nullptr;
        shared_ptr<T> sp(make_shared<T>(_data.front()));
        _data.pop();
        return sp;
    }
};

// using above implementation, entire queue is protected, and at any time only one thread is accessing data. which is not truly concurrent.
// unbounded, can push data as long as memory is available.
template <typename T>
class ts_queue_node
{
    struct  node
    {
        shared_ptr<T> data;
        unique_ptr<node> next;
    };
    unique_ptr<node> head;
    node* tail;
    mutable mutex mx_head;
    mutable mutex mx_tail;
    condition_variable cv;

    node* get_tail()
    {
        lock_guard<mutex> lk(mx_tail);
        return tail;
    }

    unique_ptr<node> pop_head()
    {
        /* for earlier impl without wait functions, moved out to support wait functions.
         lock_guard<mutex> lk(mx_head);
         if (head.get() == get_tail())
             return nullptr;
        */
        auto old_head = move(head);
        head = move(old_head->next);
        return old_head;
    }

    unique_lock<mutex> wait_for_data()
    {
        unique_lock<mutex> ul(mx_head);
        cv.wait(ul, [&]()
            {
                return head.get() != get_tail();
            });
        return move(ul); // unique lock can not be copied only moved.
    }

    unique_ptr<node> wait_pop_head()
    {
        unique_lock<mutex> ul(wait_for_data());
        return pop_head();
    }

    unique_ptr<node> wait_pop_head(T& val)
    {
        unique_lock<mutex> ul(wait_for_data());
        val = move(*head->data);
        return pop_head();
    }

    unique_ptr<node> try_pop_head()
    {
        lock_guard<mutex> lk(mx_head);
        if (head.get() == get_tail())
            return nullptr;
        return pop_head();
    }

    unique_ptr<node> try_pop_head(T& val)
    {
        lock_guard<mutex> lk(mx_head);
        if (head.get() == get_tail())
            return nullptr;
        val = move(*head->data);
        return pop_head();
    }
public:

    ts_queue_node() : head(new node), tail(head.get())
    {}
    ts_queue_node(const ts_queue_node& oth) = delete;
    ts_queue_node& operator=(const ts_queue_node& oth) = delete;
    shared_ptr<T> try_pop()
    {
        auto ohead = try_pop_head();
        return ohead ? ohead->data : nullptr;
    }

    shared_ptr<T> wait_pop()
    {
        const auto ohead = wait_pop_head();
        // return ohead ? ohead->data : nullptr;
        return ohead->data; // no need to check for null because we wait for data to be available, so data is always there.
    }

    bool try_pop(T& val)
    {
        auto ohead = try_pop_head(val);
        return ohead;
    }

    void wait_pop(T& val)
    {
        const auto ohead = wait_pop_head(val);
    }

    void push(T val)
    {
        shared_ptr<T> ndata(make_shared<T>(move(val)));
        unique_ptr<node> nnode(new node);
        {
            lock_guard<mutex> lk(mx_tail);
            auto* ntail = nnode.get();
            tail->data = ndata;
            tail->next = move(nnode);
            tail = ntail;
        }
        cv.notify_one();
    }
    bool empty()
    {
        lock_guard<mutex> head_lock(mx_head);
        return (head.get() == get_tail());
    }
};

/*
- concurrent lookup table
- STL associative containers are not very concurrency friendly, they heavily use iterators and working around iterators in multi-threaded systems are tricky.
- let's roll out own. 3  ways to implement lookup table.
    - Binary Tree : need to lock root, so does not help much to improve concurrency.
    - sorted array : need to hold lock on entire array to search, does not help much.
    - hash table : can keep locks per bucket, concurrency can be improved.
*/

template <typename Key, typename Value, typename Hash = hash<Key>>
class ts_lookup_table
{
private:
    class bucket_type
    {
    public:
        using bucket_value = pair<Key, Value>;
        using bucket_data = list<bucket_value>;
        typedef typename bucket_data::iterator bucket_iterator;
        //using bucket_iterator = typename bucket_data::iterator;
    private:
        bucket_data _data;
        mutable shared_mutex bmx;


        bucket_iterator find_entry_for(Key const& key) // const fails because find_if returns const iterator & can not assign to non const one.
        {
            return find_if(_data.begin(), _data.end(),
                [&](bucket_value const& item)
                {return item.first == key;});
        }
        /*
        bucket_iterator find_entry_for(const Key& key) const
        {
            return find_if(_data.begin(), _data.end(), [&](const bucket_value& item)
                {
                    return item.first == key;
                });
        }*/

    public:
        Value at_key(const Key& key, const Value& default_val) const
        {
            shared_lock<shared_mutex> slk(bmx);
            const auto fitr = find_entry_for(key);
            return (fitr != _data.end()) ? (fitr->second) : default_val;
        }

        void add_update(const Key& key, const Value& val)
        {
            unique_lock<shared_mutex> ul(bmx);
            const auto fitr = find_entry_for(key);
            if (fitr == _data.end())
                _data.push_back({ key, val });
            else
                fitr->second = val;
        }

        void remove(const Key& key)
        {
            unique_lock<shared_mutex> ul(bmx);
            const auto fitr = find_entry_for(key);
            if (fitr != _data.end())
                _data.erase(fitr);
        }
    };

    vector<unique_ptr<bucket_type>> buckets;
    Hash hasher;
    bucket_type& get_bucket(const Key& key)
    {
        const auto idx = hasher(key) % buckets.size();
        return *buckets[idx];
    }

public:
    using key_type = Key;
    using value_type = Value;
    using hash_type = Hash;

    ts_lookup_table(unsigned num_buckets = 19, const Hash& hash_ = Hash())
        : buckets(num_buckets), hasher(hash_)
    {
        for (int i = 0; i < num_buckets; ++i)
        {
            buckets[i].reset(new bucket_type());
        }
    }
    ts_lookup_table(const ts_lookup_table& oth) = delete;
    ts_lookup_table& operator=(const ts_lookup_table& oth) = delete;

    Value at_key(const Key& key, const Value& val = Value()) const
    {
        return get_bucket(key).at_key(key, val);
    }

    void add_update(const Key& key, const Value& val = Value())
    {
        return get_bucket(key).add_update(key, val);
    }

    void remove(const Key& key)
    {
        return get_bucket(key).remove(key);
    }

    unordered_map<Key, Value> get_map() const
    {
        vector<unique_lock<shared_mutex> > locks;
        for (unsigned i = 0;i < buckets.size();++i)
        {
            locks.push_back(
                unique_lock<shared_mutex>(buckets[i].bmx));
        }
        unordered_map<Key, Value> res;
        for (unsigned i = 0;i < buckets.size();++i)
        {
            //for (bucket_iterator it = buckets[i].data.begin(); // why auto works but bucket iterator fails to compile.
            for (auto it = buckets[i].data.begin();
                it != buckets[i].data.end();
                ++it)
            {
                res.insert(*it);
            }
        }
        return res;
    }

};