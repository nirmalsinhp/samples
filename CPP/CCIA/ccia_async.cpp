#include <bits/stdc++.h>
#include "copy_counter.hpp"
using namespace std;
/*
- future : used for one-off communication/event. once future become ready, can't be reset. like unique_ptr & shared_ptr
    future : one & only instance associated with event. movable only
    shared_future : multiple futures associated with same event, get ready at same time. copyable.
    future.get is blocking.
- std:: async : async task, returns future,which holds return of function. by default, it’s up to the implementation whether std::async starts a new thread, or whether the task runs synchronously when the future is waited for. function passed to async works same as function passed to threads.
    - launch policy :
        deferred : executed when get or wait on future called.
        async : function needs to be run in it's own thread/
- std::packaged_task<> ties a future to a function or callable object. When the std:: packaged_task<> object is invoked, it calls the associated function or callable object and makes the future ready, with the return value stored as the associated data. it's template parameter is function object.
- The std::packaged_task object is a callable object, and it can be wrapped in a std::function object, passed to a std::thread as the thread function, passed to another function that requires a callable object, or even invoked directly.
- std::promise<T> provides a means of setting a value (of type T) that can later be read through an associated std::future<T> object.
- You can obtain the std::future object associated with a given std::promise by calling the get_future() member function, just like with std::packaged_task. When the value of the promise is set (using the set_value() member function), the future becomes ready and can be used to retrieve the stored value.
- can set exception in std::promise using set_exception function. if no value or exception is stored in promise before destroying, exception broken promise is stored by dtor of promise.
- If you access a single std::future object from multiple threads without additional synchronization, you have a data race and undefined behavior. This is by design: std::future models unique ownership of the asynchronous result, and the one-shot nature of get() makes such concurrent access pointless
- std::experimental::future & shared_future provides continuatuons. means you can call .then function on the future when it gets ready, continuation takes future as an argument.
- when_all & when_any to check if all futures are ready or any future is ready respectively. when we need results from multiple threads.
- latch : count_down, it does not matter which threds calls count_down, once latch is 0, processing can be move ahead, can;t be reset.
- barrier : Suppose you have a group of threads that are operating on some data. Each thread can do its processing on the data independently of the others, so no synchronization is needed during the processing, but all the threads must have completed their processing before the next data item can be processed, or before the subsequent processing can be done. std::experimental::barrier is targeted at precisely this scenario. stage wise processing, each stage can be a barrier. other threads wait till all threads are arrived at barrier.
*/

struct X
{
public:
    void foo(int i, const string& s)
    {
        cout << this_thread::get_id() << " " << i << " " << s << endl;
    }

    string string_repeat(int times, const string& s)
    {
        cout << __PRETTY_FUNCTION__ << " " << this_thread::get_id() << endl;
        string out;
        for (int i = 0; i < times; ++i)
            out += s + "-";
        return out;
    }
};

int answer_to_life(CopyCounter cc)
{
    cout << cc.stats << endl;
    return 42;
}

string to_str(int i)
{
    cout << __PRETTY_FUNCTION__ << " " << this_thread::get_id() << endl;
    return to_string(i);
}

string to_str_rpt(int i)
{
    cout << __PRETTY_FUNCTION__ << " " << this_thread::get_id() << endl;
    if (i > 1000)
        throw out_of_range("tooooo big");
    string str = to_string(i);
    string out;
    for (int j = 0; j < i; ++j)
        out += str;
    return out;
}

mutex tmut;
std::deque<std::packaged_task<string(int)> > tasks;

template <typename F>
future<string> create_tasks(F f)
{
    packaged_task<string(int)> pt(f);
    auto fut = pt.get_future();
    lock_guard<mutex> lk(tmut);
    tasks.push_back(move(pt));
    return fut;
}

int main()
{
    try
    {
        /* code */
        using namespace std::chrono_literals;
        auto one_day = 24h;
        auto half_an_hour = 30min;
        auto max_time_between_messages = 30ms;
        auto snow = chrono::system_clock::now();
        auto stnow = chrono::steady_clock::now();
        //cout << typeid(snow).name() << " " << typeid(stnow).name() << endl;
        X x;
        future<void> f1 = async(launch::deferred, &X::foo, &x, 100, "99");
        future<string> f2 = async(launch::async, &X::string_repeat, x, 1, "check");
        cout << __PRETTY_FUNCTION__ << " " << this_thread::get_id() << endl;
        CopyCounter cc;
        auto ff = async(answer_to_life, move(cc));
        cc.reset();
        f1.wait();
        cout << f2.get() << endl;
        cout << ff.get() << endl;
        auto ft1 = create_tasks(to_str);
        auto ft2 = create_tasks(to_str_rpt);

        auto fe = async(launch::async, to_str_rpt, 2000);
        while (!tasks.empty())
        {
            auto t = move(tasks.front());
            tasks.pop_front();
            t(10);
        }
        // std::experimental::future<int> find_the_answer; not working with my compiler.
        //fe.then();
        auto sf = ft2.share(); //shared_future.
        auto sf1 = sf;
        //cout << ft2.get() << endl;
        cout << ft1.get() << endl;
        cout << sf.get() << endl;
        cout << sf1.get() << endl;
        auto rpt_str = fe.get(); // future stored and re-throws an exception.
        cout << "should not print " << rpt_str << endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }


    return 0;
}