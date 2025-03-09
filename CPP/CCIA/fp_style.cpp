#include <bits/stdc++.h>
using namespace std;

/*
- functional programming, functions are pure, result depends only on input params, no shared state, used. results are output onlt, no shared state modified.
- c++ with futures can be used for writing concurrent programms using FP style.

- MPI : message passing interface, communication between threads only using messages.
- Erlang have this model.
- each thread is effectively state machine.
- called Actor Model.
*/
template <typename T, template <typename ...> class container> 
static void print_container(const container<T>& C, string del = " ")
{
    for(const auto& c: C)
        cout << c << " ";
    cout << endl;
}

template<typename F,typename A>
//future<result_of<F(A&&)>::type>
auto spawn_task(F&& f,A&& a)
{
    typedef typename result_of<F(A&&)>::type result_type; // needs typename.
    packaged_task<result_type(A&&)> task(move(f));
    future<result_type> res(task.get_future());
    thread t(move(task),move(a));
    t.detach();
    return res;
}

template <typename T>
list<T> seq_fp_quick_sort(list<T> input)
{
    if(input.empty())
        return input;
    list<T> result;
    result.splice(result.begin(), input, input.begin());
    const T& pivot = result.front();
    auto pivot_pos = partition(input.begin(), input.end(), [&](const T& t)
                                                        {
                                                            return t < pivot;
                                                        });
    list<T> lower;
    lower.splice(lower.begin(), input, input.begin(), pivot_pos);
    auto new_lower(seq_fp_quick_sort(move(lower)));
    auto new_higher(seq_fp_quick_sort(move(input)));
    result.splice(result.begin(), new_lower);
    result.splice(result.end(), new_higher);
    return result;
}

template <typename T>
list<T> par_fp_quick_sort(list<T> input)
{
    //cout << input.size() << " " << this_thread::get_id() << endl;
    if(input.empty())
        return input;
    list<T> result;
    result.splice(result.begin(), input, input.begin());
    const T& pivot = result.front();
    auto pivot_pos = partition(input.begin(), input.end(), [&](const T& t)
                                                        {
                                                            return t < pivot;
                                                        });
    list<T> lower;
    lower.splice(lower.begin(), input, input.begin(), pivot_pos);
    future<list<T>> new_lower(async(&par_fp_quick_sort<T>, move(lower)));
    auto new_higher(par_fp_quick_sort(move(input)));
    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower.get());
    return result;
}

int main()
{
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist(1,10000);

    auto gen = [&dist, &rng](){
                    return dist(rng);
               };
    list<int> v(100);
    generate(v.begin(), v.end(), gen);
    auto copylist = v;
    print_container(v, ", ");
    auto qsorted = seq_fp_quick_sort(v);
    print_container(qsorted, ", ");
    
//    shuffle(v.begin(), v.end(), gen);
    v = copylist;
    print_container(v, ", ");
    qsorted = par_fp_quick_sort(v);
    print_container(qsorted, ", ");

    return 0;
}