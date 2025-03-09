#include <bits/stdc++.h>
using namespace std;

// needs to be defined before variadic one, else last call with single arg will fail.
/*
template <typename T>
void print_n(T t)
{
    cout << t << endl;
}

template <typename T0, typename ... T1toTN>
void print_n(T0 t, T1toTN... rest)
{
    cout << t << ", ";
    print_n(rest...);
}

template <typename TP, size_t ... indices>
void printtp_impl(TP tp, index_sequence<indices...> )
{
    print_n(get<indices>(tp)...);
}

template <typename TP>
void printtp(TP tp)
{
    //printtp_impl(tp, make_index_sequence<tuple_size_v<TP>>);
    printtp_impl(tp, make_index_sequence<tuple_size_v<TP>>{});
}
*/
// above commented code takes everything by value, very inefficient, using forward/universal reference here.
template <typename T>
void print_n(T&& t)
{
    cout << forward<T>(t) << endl;
}

template <typename T0, typename ... T1toTN>
void print_n(T0&& t, T1toTN&&... rest)
{
    cout << forward<T0>(t) << ", ";
    print_n(forward<T1toTN>(rest)...);
}

template <typename TP, size_t ... indices>
void printtp_impl(TP&& tp, index_sequence<indices...> )
{
    print_n(get<indices>(forward<TP>(tp))...);
}

template <typename TP>
void printtp(TP&& tp)
{
    //printtp_impl(tp, make_index_sequence<tuple_size_v<TP>>);
    printtp_impl(forward<TP>(tp), make_index_sequence<tuple_size_v<remove_reference_t<TP>>>{});
}

void print_n()
{
    cout << "end" << endl;
}

int main()
{
    print_n(9, "9", 9.9, false);
    print_n();
    const auto tp = make_tuple(9, "9", 9.9, false);
    printtp(tp);
    // another way to print tuple.
    apply([](auto&&... args)
            {
                ((cout << args  << " - "), ...);
            }, tp);
    return 0;

}