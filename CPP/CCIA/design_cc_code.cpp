#include <bits/stdc++.h>
using namespace std;
/*
- for concurrent applications you also need to consider which data to share, how to synchronize accesses to that data, which threads need to wait for which other threads to complete certain operations, and so on.
# Deviding data
- before processing begins.
- using recursive calls
- thread pool : that’s a set of threads that each take work to do from a list of pending work, do the work, and then go back to the list for more.
-  Dividing work by task type
    - Dividing work by task type to separate concerns : user interface & back - end
    - Dividing a sequence of tasks between threads.
- Factors affecting the performance of concurrent code
    - number of processers
        - over-subscription : number of threads more than number of processors.
    - Data contention and cache ping-pong
    # false sharing:
    - Processor caches don’t generally deal in individual memory locations; instead, they deal in blocks of memory called cache lines.
    - if the data items in a cache line are unrelated and need to be accessed by different threads, this can be a major cause of performance problems. The cache line is shared, even though none of the data is, hence the term false sharing
    - How close is your data? : if data is spread, multiple cache lines might needs to be loaced.
    - Oversubscription and excessive task switching
#  Designing data structures for multithreaded performance
    - The key things to bear in mind when designing your data structures for multithreaded performance are contention, false sharing, and data proximity.
    - Try to adjust the data distribution between threads so that data that’s close together is worked on by the same thread.
    - Try to minimize the data required by any given thread.
    - Try to ensure that data accessed by separate threads is sufficiently far apart to avoid false sharing using std::hardware_destructive_interference_size as a guide.
# exception safety
    - By contrast, in a parallel algorithm many of the operations will be running on separate threads. In this case, the exception can’t be allowed to propagate because it’s on the wrong call stack. If a function spawned on a new thread exits with an exception, the application is terminated.
    - The key thing to note for exception safety is that if you destroy the future without waiting for it, the destructor will wait for the thread to complete.
- For any given multithreaded program, the number of threads that are performing useful work will vary as the program runs.Threads often spend time waiting for each other or waiting for I/O operations to complete.Every time one thread has to wait for something (whatever that something is), unless there’s another thread ready to take its place on the processor, you have a processor sitting idle that could be doing useful work.
- Scalability is about reducing the time it takes to perform an action or increasing the amount of data that can be processed in a given time as more processors are added.
- ahmdal's law
    P = 1 / (Fs + ((1 - Fs) / N)) where
    N is number of processers
    Fs is serial secion.
    P is performace gain.   naive but useful approximation.
- multithreading to hide latency, to improve responsiveness etc.
*/

class join_thread
{
    vector<thread>& _threads;
public:
    join_thread(vector<thread>& threads) :
        _threads(threads)
    {}
    ~join_thread()
    {
        for (int i = 0; i < _threads.size(); ++i)
        {
            if (_threads[i].joinable())
                _threads[i].join();
        }
    }
};


template <typename Iter, typename T>
struct  accumulate_block
{
    T operator()(Iter start, Iter end)
    {
        return accumulate(start, end, T());
    }
    /* data */
};

template <typename Iter, typename T>
T paralle_accumulate(Iter first, Iter last, T init)
{
    const unsigned length = distance(first, last);
    if (!length)
        return init;
    const unsigned int min_per_t = 50;
    const unsigned long max_threads = (length + min_per_t - 1) / min_per_t;
    const unsigned long hw_cc = thread::hardware_concurrency();
    const unsigned long num_ts = min(hw_cc ? hw_cc : 2UL, max_threads);
    cout << "number of threads" << num_ts << endl;
    const auto block_size = length / num_ts;
    vector<future<T>> futs(num_ts - 1);
    vector<thread> threads(num_ts - 1);
    Iter block_start = first;
    join_thread jt(threads);

    for (auto i = 0; i < num_ts - 1; ++i)
    {
        Iter block_end = block_start;
        advance(block_end, block_size);
        //packaged_task<T(Iter, Iter)> task(accumulate_block<Iter, T>());
        std::packaged_task<T(Iter, Iter)> task{ accumulate_block<Iter,T>() };
        futs[i] = task.get_future();
        threads[i] = thread(move(task), block_start, block_end);
        block_start = block_end;
    }
    T last_res = accumulate_block<Iter, T>()(block_start, last);
    T res = init;
    for (int i = 0; i < num_ts - 1; ++i)
    {
        res += futs[i].get();
    }
    res += last_res;
    return res;
}

template <typename Iter, typename T>
T rec_paralle_accumulate(Iter first, Iter last, T init)
{
    const unsigned length = distance(first, last);
    if (length <= 50)
        return accumulate(first, last, init);

    Iter mid = first;
    advance(mid, length / 2);
    future<T> fres = async(rec_paralle_accumulate<Iter, T>, first, mid, init);
    T sres = rec_paralle_accumulate(mid, last, T());
    return fres.get() + sres;
}

template <typename Iter, typename F>
void par_for_each(Iter first, Iter last, F func)
{
    const unsigned length = distance(first, last);
    if (!length)
        return;
    const unsigned int min_per_t = 50;
    const unsigned long max_threads = (length + min_per_t - 1) / min_per_t;
    const unsigned long hw_cc = thread::hardware_concurrency();
    const unsigned long num_ts = min(hw_cc ? hw_cc : 2UL, max_threads);
    cout << "number of threads" << num_ts << endl;
    const auto block_size = length / num_ts;
    vector<future<void>> futs(num_ts - 1);
    vector<thread> threads(num_ts - 1);
    Iter block_start = first;
    join_thread jt(threads);

    for (auto i = 0; i < num_ts - 1; ++i)
    {
        Iter block_end = block_start;
        advance(block_end, block_size);
        //packaged_task<T(Iter, Iter)> task(accumulate_block<Iter, T>());
        std::packaged_task<void(void)> task{
            [=]()
            {
                for_each(block_start, block_end, func);
            }
        };
        futs[i] = task.get_future();
        threads[i] = thread(move(task));
        block_start = block_end;
    }
    for_each(block_start, last, func);
    for (int i = 0; i < num_ts - 1; ++i)
    {
        futs[i].get();
    }
}

static long rec1 = 0;
static long rec2 = 0;
template <typename Iter, typename F>
void rec_par_for_each(Iter first, Iter last, F func)
{
    rec1++;
    const unsigned long length = distance(first, last);
    if (!length)
        return;
    if (length < 50)
        for_each(first, last, func);
    else
    {
        const Iter mid = first + length / 2;
        future<void> fres = async(&rec_par_for_each<Iter, F>, first, mid, func);
        rec_par_for_each(mid, last, func);
        fres.get();
    }
}


template<typename Iterator, typename Func>
void parallel_for_each(Iterator first, Iterator last, Func f)
{
    unsigned long const length = std::distance(first, last);
    if (!length)
        return;
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads =
        (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads =
        std::thread::hardware_concurrency();
    unsigned long const num_threads =
        std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;
    std::vector<std::future<void> > futures(num_threads - 1);
    std::vector<std::thread> threads(num_threads - 1);
    join_thread joiner(threads);
    Iterator block_start = first;
    for (unsigned long i = 0;i < (num_threads - 1);++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        std::packaged_task<void(void)> task(
            [=]()
            {
                std::for_each(block_start, block_end, f);
            });
        futures[i] = task.get_future();
        threads[i] = std::thread(std::move(task));
        block_start = block_end;
    }
    std::for_each(block_start, last, f);
    for (unsigned long i = 0;i < (num_threads - 1);++i)
    {
        futures[i].get();
    }
}

template<typename Iterator, typename Func>
void rec_parallel_for_each(Iterator first, Iterator last, Func f)
{
    rec2++;
    unsigned long const length = std::distance(first, last);
    if (!length)
        return;
    unsigned long const min_per_thread = 25;
    if (length < (2 * min_per_thread))
    {
        std::for_each(first, last, f);
    }
    else
    {
        Iterator const mid_point = first + length / 2;
        std::future<void> first_half =
            std::async(&rec_parallel_for_each<Iterator, Func>,
                first, mid_point, f);
        rec_parallel_for_each(mid_point, last, f);
        first_half.get();
    }
}

template <typename Iter, typename MatchT>
Iter par_find(Iter first, Iter last, MatchT ele)
{
    struct  find_el
    {
        void operator()(Iter begin, Iter end, MatchT e, promise<Iter>* p, atomic<bool>* flag)
        {
            try
            {
                for (; begin != end; ++begin)
                {
                    if (*begin == e)
                    {
                        p->set_value(begin);
                        flag->store(true);
                    }
                }
            }
            catch (...)
            {
                try
                {
                    p->set_exception(current_exception());
                    flag->store(true);
                }
                catch (const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }

            }

        }
        /* data */
    };

    unsigned long const length = std::distance(first, last);
    if (!length)
        return last;
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads =
        (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads =
        std::thread::hardware_concurrency();
    unsigned long const num_threads =
        std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;
    promise<Iter> result;
    atomic<bool> done(false);
    std::vector<std::thread> threads(num_threads - 1);
    {
        join_thread joiner(threads);
        Iter block_start = first;
        for (auto i = 0; i < num_threads - 1; ++i)
        {
            Iter block_end = block_start;
            advance(block_end, block_size);
            //packaged_task<T(Iter, Iter)> task(accumulate_block<Iter, T>());
            threads[i] = thread(find_el(), block_start, block_end, ele, result, done);
            block_start = block_end;
        }
        find_el(block_start, last, ele, result, done);
    }
    if (!done.load())
        return last;
    return result.get_future().get();

}

template <typename Iter>
void par_partial_sum(Iter first, Iter last)
{
    using value_type = typename Iter::value_type;

    struct process_chunks
    {
        void operator()(Iter begin, Iter last,
            future<value_type>* prv_end,
            promise<value_type>* end_val
            )
        {
            try
            {

                Iter end = last;
                end++;
                partial_sum(begin, end, begin);
                if (prv_end)
                {
                    value_type padd = prv_end->get();
                    *last += padd;
                    if (end_val)
                    {
                        end_val->set_value(*last);
                    }
                    for_each(begin, last, [padd](value_type& item)
                        {
                            item += padd;
                        });
                }
                else if (end_val)
                {
                    end_val->set_value(*last);
                }
                /* code */
            }
            catch (const std::exception& e)
            {
                if (end_val)
                {
                    end_val->set_exception(current_exception());
                }
                else
                    throw;
            }
        }
        /* data */
    };

    unsigned long const length = std::distance(first, last);
    if (!length)
        return;
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads =
        (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads =
        std::thread::hardware_concurrency();
    unsigned long const num_threads =
        std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;
    vector<thread> threads(num_threads - 1);
    vector<promise<value_type>> end_vals(num_threads - 1);
    vector<future<value_type>> prev_vals;
    prev_vals.reserve(num_threads-1);
    join_thread joiner(threads);
    Iter block_start = first;
    for(unsigned long i = 0; i < num_threads - 1; ++i)
    {
        Iter block_end = block_start;
        advance(block_end, block_size - 1);
        threads[i] = thread(process_chunks(), block_start, block_end, 
                            (i!=0) ? &prev_vals[i-1]: nullptr,
                            &end_vals[i]);
        block_start = block_end;
        ++block_start;
        prev_vals.push_back(end_vals[i].get_future());
    }
        Iter final_e = block_start;
        advance(final_e, distance(block_start, last) - 1 );
        process_chunks()( block_start, final_e, 
                            (num_threads > 1) ? &prev_vals.back(): nullptr,
                            nullptr);

}


int main()
{
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> dist(1, 10000000);

    auto gen = [&dist, &rng]() {
        return dist(rng);
        };
    vector<long> v(100000000);
    generate(v.begin(), v.end(), gen);
    /*
        auto t1 = chrono::high_resolution_clock::now();
        auto res = paralle_accumulate(v.begin(), v.end(), 0L);
        cout << res << endl;
        auto t2 = chrono::high_resolution_clock::now();
        auto d1 = chrono::duration_cast<chrono::microseconds>(t2 - t1);
        cout << "PAR " << d1.count() << " MicroSecond(s)" << endl;

        auto t3 = chrono::high_resolution_clock::now();
        auto resw = accumulate(v.begin(), v.end(), 0L);
        cout << resw << endl;
        auto t4 = chrono::high_resolution_clock::now();
        auto d2 = chrono::duration_cast<chrono::microseconds>(t4 - t3);
        cout << "STD " << d2.count() << " MicroSecond(s)" << endl;

        auto t5 = chrono::high_resolution_clock::now();
        auto rres = rec_paralle_accumulate(v.begin(), v.end(), 0L);
        cout << rres << endl;
        auto t6 = chrono::high_resolution_clock::now();
        auto d3 = chrono::duration_cast<chrono::microseconds>(t6 - t5);
        cout << "REC " << d3.count() << " MicroSecond(s)" << endl;
    */

    auto f = [](int i) {
        i = i * 2;
        };
    auto et3 = chrono::high_resolution_clock::now();
    for_each(v.begin(), v.end(), f);
    auto et4 = chrono::high_resolution_clock::now();
    auto ed2 = chrono::duration_cast<chrono::microseconds>(et4 - et3);
    cout << "STD " << ed2.count() << " MicroSecond(s)" << endl;

    auto et1 = chrono::high_resolution_clock::now();
    par_for_each(v.begin(), v.end(), f);
    //par_for_each(v.begin(), v.end(), bind(multiplies<int>(), placeholders::_1, 2));
    auto et2 = chrono::high_resolution_clock::now();
    auto ed1 = chrono::duration_cast<chrono::microseconds>(et2 - et1);
    cout << "PAR " << ed1.count() << " MicroSecond(s)" << endl;

    auto et7 = chrono::high_resolution_clock::now();
    rec_parallel_for_each(v.begin(), v.end(), f);
    auto et8 = chrono::high_resolution_clock::now();
    auto ed4 = chrono::duration_cast<chrono::microseconds>(et8 - et7);
    cout << "REC2 " << rec2 << " " << ed4.count() << " MicroSecond(s)" << endl;

    auto et5 = chrono::high_resolution_clock::now();
    rec_par_for_each(v.begin(), v.end(), f);
    auto et6 = chrono::high_resolution_clock::now();
    auto ed3 = chrono::duration_cast<chrono::microseconds>(et6 - et5);
    cout << "REC " << rec1 << " " << ed3.count() << " MicroSecond(s)" << endl;

    vector<int> newv(v.begin(), v.begin() + 10000 );
    par_partial_sum(newv.begin(), newv.end());
    parallel_for_each(v.begin(), v.end(), f);
    return 0;
}