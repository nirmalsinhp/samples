#include <bits/stdc++.h>
#include <experimental/memory>
#include <atomic>
#include <new>
using namespace std;

/*
- An atomic operation is an indivisible operation.(transaction) all or none.
- all operations on atomic vars are atomic, use is_lock_free() to check if they are lock free operations or uses internal locking mechanism to appear atomic. T::is_always_lock_free, static constexpr from c++17 for integral types.
- atomic_flag - atomic lock free boolean flag, just 2 ops, test_and_set & clear.
- atomic<> is a template used for all atomic since c++17, support named version of ops along with operators.
- can use atomic<T> for user defined types, with limited base template functions.
- memory order
- atomic ops do not support copy operations, as these involve 2 objects & ops on 2 diff objects can not be atomic.
-  In order to use std::atomic<UDT> for some user-defined type UDT,, this type must have a trivial copy-assignment operator. This means that the type must not have any virtual functions or virtual base classes and must use the compiler-generated copy-assignment operator.
- atomic_shared_ptr
- atomic var helps with happens before & sync with relations.
- The synchronizes-with relationship is something that you can get only between operations on atomic types.x
- happens before, strongly happens before, inter thread happens before.
- if operation A in one thread synchronizes with operation B in another thread, then A inter-thread happens before B
- memory orders - 6 types, 3  models
- seuentially consistent : memory_order_seq_cst
- acquire-release : memory_order_consume, memory_order_acquire, memory_order_release, memory_order_acq_rel
- relaxed ordering : memory_order_relaxed
- These distinct memory-ordering models can have varying costs on different CPU architectures.
- The default ordering is named sequentially consistent because it implies that the behavior of the program is consistent with a simple sequential view of the world. as if all ops were performed sequentially in single thread. Sequential consistency is the most straightforward and intuitive ordering, but it’s also the most expensive memory ordering because it requires global synchronization between all threads
- for memory order other than sequentially consistent there is no single order of events, . Not only do you have to account for things happening truly concurrently, but threads don’t have to agree on the order of events. In the absence of other ordering constraints, the only requirement is that all threads agree on the modification order of each individual variable.
- in memory_order_relaxed, relaxed ordering there is no gurantee for order of diff variable in diff threads, only order agreed upon is modification of individual variable.
- acquire-release operations : load/read - acquire, store/write - release, fetch-add,exchange, read-modify-write - acquire-release
    - A release operation synchronizes-with an acquire operation that reads the value written. A release operation synchronizes-with an acquire operation that reads the value written.
    - With mutexes, you learn that you must ensure that the same mutex is locked when you read a value as was locked when you wrote it, and the same applies here; your acquire and release operations have to be on the same variable to ensure an ordering.
- you should not use memory_order_consume in your code!
- If the store is tagged with memory_order_release, memory_order_acq_rel, or memory_order_seq_cst, and the load is tagged with memory_order_consume, memory_order_acquire, or memory_order_seq_cst, and each operation in the chain loads the value written by the previous operation, then the chain of operations constitutes a release sequence and the initial store synchronizes with (for memory_order_acquire or memory_order_seq_cst) or is dependency-ordered-before (for memory_order_consume) the final load. Any atomic read-modify-write operations in the chain can have any memory ordering (even memory_order_relaxed).
- Fences are global operations and affect the ordering of other atomic operations in the thread that executed the fence. memory barriers. sync without accessing any data.
- if an acquire operation sees the result of a store that takes place after a release fence, the fence synchronizes with that acquire operation; and if a load that takes place before an acquire fence sees the result of a release operation, the release operation synchronizes with the acquire fence.
*/

vector<int> vec_data;
atomic<bool> df(false);


void reader_thread()
{
    while(!df.load())
    {
        this_thread::sleep_for(10ms);
    }
    // this read always happens after write in writer thread, due to sync of read & write for atomic bool flag.
    cout << this_thread::get_id() << " data : " << vec_data.front() << endl;
}

void writer_thread()
{
    vec_data.push_back(42);
    df.store(true);
}

std::atomic<bool> bx, by;
std::atomic<int> bz;
void write_x()
{
    bx.store(true,std::memory_order_seq_cst);
}
void write_y()
{
    by.store(true,std::memory_order_seq_cst);
}
void read_x_then_y()
{
    while(!bx.load(std::memory_order_seq_cst));
    if(by.load(std::memory_order_seq_cst))
        ++bz;
}
void read_y_then_x()
{
    while(!by.load(std::memory_order_seq_cst));
    if(bx.load(std::memory_order_seq_cst))
        ++bz;
}


std::atomic<bool> ax, ay;
std::atomic<int> az;
void write_ar_x()
{
    ax.store(true,std::memory_order_release);
}
void write_ar_y()
{
    ay.store(true,std::memory_order_release);
}
void ar_read_x_then_y()
{
    while(!ax.load(std::memory_order_acquire));
    if(ay.load(std::memory_order_acquire))
        ++az;
}
void ar_read_y_then_x()
{
    while(!ay.load(std::memory_order_acquire));
    if(ax.load(std::memory_order_acquire))
        ++az;
}

int main()
{
    static_assert(atomic<bool>::is_always_lock_free, "not lock free");
    static_assert(atomic<double>::is_always_lock_free, "not lock free");
    static_assert(atomic<complex<float>>::is_always_lock_free, "not lock free");
    //static_assert(atomic<shared_ptr<int>>::is_always_lock_free, "not lock free");
    //static_assert(atomic<string>::is_always_lock_free, "not lock free");

    //std::experimental::atomic_sh as = make_shared<int>(20); // it should work, as per book, but not working.
    //alignas(std::hardware_destructive_interference_size) std::atomic<int> cat; gcc have not implemented it,
    //cout << std::hardware_destructive_interference_size << endl;
    atomic_flag f=ATOMIC_FLAG_INIT;
    f.clear();
    bool x = f.test_and_set();
    cout << x << endl;

    std::atomic<bool> b;
    x =b.load(std::memory_order_acquire);
    b.store(true);
    x = b.exchange(false,std::memory_order_acq_rel);
    cout << x << endl;
    bool fx = false;
    bool done = b.compare_exchange_weak(fx, true);
    //atomic<shared_ptr<int>> asp;
 //   cout << asp.is_lock_free() << endl;
    thread t1(reader_thread);
    thread t2(writer_thread);
    t1.join();
    t2.join();
    bx=false;
    by=false;
    bz=0;
    std::thread b1(write_x);
    std::thread b2(write_y);
    std::thread b3(read_x_then_y);
    std::thread b4(read_y_then_x);
    b1.join();
    b2.join();
    b3.join();
    b4.join();
    assert(bz.load()!=0);
    cout << bz << endl;
    //   auto cf = f;
    //cout << f << endl;
    // acq_rel
    ax=false;
    ay=false;
    az=0;
    std::thread a1(write_ar_x);
    std::thread a2(write_ar_y);
    std::thread a3(ar_read_x_then_y);
    std::thread a4(ar_read_y_then_x);
    a1.join();
    a2.join();
    a3.join();
    a4.join();
    assert(az.load()!=0);
    cout << az << endl;


    return 0;
}