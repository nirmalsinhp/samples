#include <bits/stdc++.h>
#include "tsafeq.hpp"
using namespace std;
/*
- sometimes you don’t just need to protect the data, you also need to synchronize actions on separate threads.
    - busy waiting, constantly checking flag to be set by another thread, too much waste & just slows everything.
    - check for flag, sleep if not set, check again on waking up. difficult to get sleep time right.
    - condition var, some threads wait on event/condition, others can satisfy the condition & notify.
- conditional_variable works with mutex for sync & conditional_variable_any works with any type bahaving like mutex.
- std::condition_variable::wait is an optimization over a busy-wait.
- When the waiting thread reacquires the mutex and checks the condition, if it isn’t in direct response to a notification from another thread, it’s called a spurious wake. predicate should not have side effects. due to spuruious wakes.
- check thread safe queue.
- - waiting with timeout
    - wait_for :duration based - like 30 ms
    - wait_until : absolute - till specific point in time
    - The wait functions all return a status to indicate whether the wait timed out or the waited-for event occurred.
- timed_mutex & recursive_timed_mutex have try_lock_for & try_lock_until
- clocks
    - now
    - timepoint
    - type of clocks
    - tick
    - duration
    - ratio
    - duration_cast
- If a clock ticks at a uniform rate (whether or not that rate matches the period) and can’t be adjusted, the clock is said to be a steady clock.

*/



// implementation of conditional_wait
template<typename Predicate>
void minimal_wait(std::unique_lock<std::mutex>& lk, Predicate pred) {
    while (!pred()) {
        lk.unlock();
        lk.lock();
    }
}

mutex data_mx;
deque<string> DQ;
condition_variable cv_data;

void write_thread()
{
    static int writes = 0;
    while (writes < 1000)
    {
        {
            lock_guard<mutex> lk(data_mx);
            DQ.push_back(string("msg") + to_string(writes++));
        }
        cv_data.notify_one();
    }

    {
        lock_guard<mutex> lk(data_mx);
        DQ.push_back("STOP!!");
    }
    cv_data.notify_one();

}


void read_thread()
{
    string read_str;
    while (true)
    {
        unique_lock<mutex> ulk(data_mx);
        // need to use unique lock here, as condition variable locks the mute, check if condition is satisfied if not unlocks the mutex & goes waiting/blocking.
        cv_data.wait(ulk, []() {return !DQ.empty();});
        read_str = DQ.front(); DQ.pop_front();
        ulk.unlock();
        cout << read_str << endl;
        if (read_str == "STOP!!")
            break;
    }
}

TSQ<string> TQ;

void read_thread_t()
{
    string read_str;
    while (true)
    {
        // need to use unique lock here, as condition variable locks the mute, check if condition is satisfied if not unlocks the mutex & goes waiting/blocking.
        TQ.wait_and_pop(read_str);
        cout << this_thread::get_id() << " " << read_str << endl;
        if (read_str == "STOP!!")
            break;
    }
}


void write_thread_t()
{
    static int writes = 0;
    while (writes < 1000)
    {
        TQ.push(string("msg") + to_string(writes++));
    }
    TQ.push("STOP!!");
    TQ.push("STOP!!");

}

int main()
{
    thread t2(write_thread);
    thread t1(read_thread);
    t1.join();
    t2.join();
    thread ts2(write_thread_t);
    thread ts1(read_thread_t);
    thread ts3(read_thread_t);
    ts1.join();
    ts2.join();
    ts3.join();

    cout << "return " << t1.joinable() << " " << t2.joinable() << endl;
    return 0;
}