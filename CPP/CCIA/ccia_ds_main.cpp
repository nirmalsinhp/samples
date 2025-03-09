#include <bits/stdc++.h>
#include "ccia_concurrent_ds.h"
using namespace std;

template <typename T>
void writer(ts_queue<T>& Q, int count)
{
    while (count--)
    {
        Q.push(to_string(count)); // temp fix for string, how it would work for all.
    }
}

template <typename T>
void reader(ts_queue<T>& Q, int count)
{
    while (count)
    {
        auto sp = Q.try_pop();
        if (sp)
        {
            count--;
            //cout << this_thread::get_id() << " " << *sp << endl;
        }
    }
}

template <typename T>
void n_writer(ts_queue_node<T>& Q, int count)
{
    while (count--)
    {
        Q.push(count);
    }
}

template <typename T>
void n_reader(ts_queue_node<T>& Q, int count)
{
    while (count)
    {
        auto sp = Q.try_pop();
        //auto sp = Q.wait_pop();
        if (sp)
        {
            count--;
            //cout << this_thread::get_id() << " " << *sp << endl;
        }
    }
}

int main()
{
    ts_queue<string> tss;
    ts_queue_node<double> tsns;
    auto t1 = chrono::steady_clock::now();
    thread wt(writer<string>, ref(tss), 1000000);
    thread rt(reader<string>, ref(tss), 600000);
    thread rt2(reader<string>, ref(tss), 400000);
    wt.join();
    rt.join();
    rt2.join();
    auto t2 = chrono::steady_clock::now();
    cout << chrono::duration_cast<chrono::microseconds>(t2-t1).count() << endl;

    auto tn1 = chrono::steady_clock::now();
    thread nwt(n_writer<double>, ref(tsns), 1000000);
    thread nrt(n_reader<double>, ref(tsns), 600000);
    thread nrt2(n_reader<double>, ref(tsns), 400000);
    nwt.join();
    nrt.join();
    nrt2.join();
    auto tn2 = chrono::steady_clock::now();
    cout << chrono::duration_cast<chrono::microseconds>(tn2-tn1).count() << endl;

    ts_lookup_table<string, double> tsl;
    
    
    tsl.add_update("price", 11.11);
    tsl.add_update("pri1", 11.111);
    tsl.add_update("pr2", 111.11);
    tsl.add_update("pre", 111.111);

    return 0;
}