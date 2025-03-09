#include <bits/stdc++.h>
#include "join_thread.h"
#include "ccia_concurrent_ds.h"
using namespace std;

/*
- make sure to call join in exception catch clause also.
- It’s important to bear in mind that by default, the arguments are copied into internal storage, where they can be accessed by the newly created thread of execution, and then passed to the callable object or function as rvalues as if they were temporaries.
- thread is move-only, assignment to existing attached thread throws terminte; can create vector of threads
- Don’t pass pointers and references to protected data outside the scope of the lock, whether by returning them from a function, storing them in externally visible memory, or passing them as arguments to user-supplied functions.
- data race, race condition
- each of a pair of threads needs to lock both of a pair of mutexes to perform some operation, and each thread has one mutex and is waiting for the other. Neither thread can proceed, because each is waiting for the other to release its mutex. This scenario is called deadlock.
-- To avoid deadlock
    - don’t wait for another thread if there’s a chance it’s waiting for you.
    - dont do nestd lock
    - do not exeute user defined code while holding lock
    - acquire lock in same order.
    - use lock heirarchy
- std::unique lock provides facility to lock, unlock & try_lock mutex, slightly more expensive than lock_guard as it maintains bit to check if it  owns lock, kind of RAAI for lock similar to unique ptr, can be moved to transfer ownership of mutex.
- Granularity of locking : Lock only reqyuired amount of time with coarse enought grnularity. try to do any processing of the data outside the lock. In particular, don’t do any time-consuming activities like file I/O while holding a lock. File I/O is typically hundreds (if not thousands) of times slower than reading or writing the same volume of data from memory. Unless the lock is intended to protect access to the file, performing I/O while holding the lock will delay other threads unnecessarily (because they’ll block while waiting to acquire the lock), potentially eliminating any performance gain from the use of multiple threads.
-  Protecting shared data during initialization : std::once_flag & call_once.
- mutex & once_flag can not be copied or moved.
- static initialization is thread safe. used in singleton mostly
- shared_mutex, shared_lock for reader writer lock, multiple readers can access data & writer gets an exclusive lock.
- recursive mutex can be locked multiple times, locking mutex more than one time is undefined behaviour.

## Memory model
- If there’s no enforced ordering between two accesses to a single memory location from separate threads, one or both of those accesses is not atomic, and if one or both is a write, then this is a data race and causes undefined behavior.
- Every object in a C++ program has a modification order composed of all the writes to that object from all threads in the program, starting with the object’s initialization. all threads should see same sequence of values
*/



/*
std::unique_lock<std::mutex> get_lock()
{
    extern std::mutex some_mutex;
    std::unique_lock<std::mutex> lk(some_mutex);
//    prepare_data();
    return lk;
}*/

// Protecting shared data during initialization

std::shared_ptr<ts_stack<int>> resource_ptr;
std::mutex resource_mutex;
void foo()
{
    std::unique_lock<std::mutex> lk(resource_mutex);
    if (!resource_ptr)
    {
        resource_ptr.reset(new ts_stack<int>());
    }
    lk.unlock();
    //resource_ptr->do_something();
}

void foo_dbl_checked_locking()
{
    // still a data race, undefined behaviour
    if (!resource_ptr)
    {
        std::lock_guard<std::mutex> lk(resource_mutex);
        if (!resource_ptr)
        {
            resource_ptr.reset(new ts_stack<int>());
        }
        //lk.unlock();
    }
    //resource_ptr->do_something();
}

std::once_flag resource_flag;
void foo_once_flag()
{
    // still a data race, undefined behaviour
    call_once(resource_flag, [&resource_ptr](){ 
        resource_ptr.reset(new ts_stack<int>());
    });
    //resource_ptr->do_something();
    // can be used similar way in class for member initialization
}

class myObj
{
public:
    void func(double d)
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }
};

class thread_guard
{
public:
    thread_guard(thread& in) : t(in) {};
    ~thread_guard()
    {
        cout << __PRETTY_FUNCTION__ << endl;
        if (t.joinable())
            t.join();
    }
    thread_guard(const thread_guard&) = delete;
    thread_guard& operator=(const thread_guard&) = delete;
private:
    thread& t;
};

void hello_string(string ss)
{
    cout << "hello " << ss << endl;
}

int hello(int& i)
{
    cout << "Hello CC World " << i << '\n';
    i++;
    return i;
}

class functor
{
public:
    functor(int& in) : i(in) {};
    void operator()(int times)
    {
        cout << this_thread::get_id() << endl;
        sleep(5);
        for (int j = 0; j < times; ++j)
            cout << i << " ";
        cout << endl;
    }

private:
    int& i;
};

template <typename T>
void move_func(unique_ptr<T> up)
{
    cout << __PRETTY_FUNCTION__ << " ";
    cout << *up << endl;
}

void vthreads()
{
    std::vector<std::thread> threads;
    for (unsigned i = 0;i < 20;++i)
    {
        threads.emplace_back(move_func<unsigned>, make_unique<unsigned>(i));
    }
    for (auto& entry : threads)
        entry.join();
}

// reader-writer lock


int main()
{
    ts_stack<string> ts1;
    ts_stack<string> ts2;
    swap(ts1, ts2);
    foo_once_flag();
    resource_ptr->push(11);
    // cout << "Hello World!" << endl;
    // thread accepts any callable type like free fucntion, functor, lambda etc.
    const auto max_threads = thread::hardware_concurrency();
    cout << " Max : " << max_threads << endl;
    vthreads();
    thread ts(hello_string, "BHUT");
    int i = 1;
    thread t(hello, ref(i)); /// can not call function with default args, need to provide it
    /*
    thread tdd; tdd.detach(); calling detach/join on thread object without actual attached thread c=throws an exception
    */
    thread tl([]()
        { cout << "Lambda " << endl;
    //throw runtime_error("error");
        });
    myObj o;
    thread to(&myObj::func, o, 20.0);
    thread_guard td(tl);
    int val = 10;
    functor f(val);
    thread tf(f, 11);
    auto upd = make_unique<double>(100.99);
    thread tm(move_func<double>, move(upd)); // move move-only types, need to provide template type for template function, not deduced.
    // tm = thread(f, 200); terminates, not allowed to assign to existing thread;
    thread tm1 = move(tm);
    // tm.join();
    join_thread jtm(move(tm1));
    //tm1.join();

    tf.detach();
    ts.join();
    to.join();
    t.join(); // if dtor of a joinable thread is called, program terminates.
    cout << t.joinable() << " " << tl.joinable() << " " << tf.joinable() << endl;
    //throw std::runtime_error("yun hi");
    //tl.join();
    //tf.join();

    return 0;
}