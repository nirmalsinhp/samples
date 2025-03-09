#include <bits/stdc++.h>
using namespace std;

class join_thread
{
    private:
    thread t;

    public:
    join_thread() = default;

    template <typename Callable, typename... Args>
    explicit join_thread(Callable func, Args&& ... args)
        : t(thread(forward<Callable>(func), forward<Args>(args)...))
    {}

    explicit join_thread(thread tin) : t(move(tin)) { }

    join_thread(join_thread&& jt) : t(move(jt.t)) { }
    join_thread& operator=(join_thread&& jt)
    {
        if(joinable())
            join();
        t = move(jt.t);
        return *this;
    }

    join_thread& operator=(thread jt)
    {
        if(joinable())
            join();
        t = move(jt);
        return *this;
    }
    ~join_thread() noexcept
    {
        if(joinable())
            join();
    }
    void join() 
    {
        t.join();
    }

    void detach()
    {
        t.detach();
    }

    bool joinable()
    {
        return t.joinable();
    }

    thread& as_thread()
    {
        return t;
    }

    thread::id get_id()
    {
        return t.get_id();
    }

    const thread& as_thread() const
    {
        return t;
    }

};