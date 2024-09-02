#include <bits/stdc++.h>
using namespace std;

// template <typename ... Args>
// auto maxf(Args ... args)
// {
//     auto mf = [](auto&& a, auto&& b){return a > b ? a : b;};
//     return (... + mf(args));
// }



// template meta programming  uses template instantiation to dirve compile time evalution.

// type identity
template <typename T>
struct type_is
{
    using type = T;
    /* data */
};


template <typename T>
struct remove_const_m : type_is<T>
{};

template <typename U>
struct remove_const_m<U const> : type_is<U>
{};

// abs
template <int N>
struct abs1
{
    static_assert(N != INT_MIN);
    static constexpr int value = (N < 0) ? -N : N;
};


template <unsigned M, unsigned N>
struct gcd1
{
    static const int value = gcd1<N, M % N>::value;
    /* data */
};

template <unsigned M>
struct gcd1<M,0>
{
    static_assert(M != 0);
    static const int value = M;

    /* data */
};


constexpr int absf(int N)
{
    return (N < 0) ? -N : N;
}

template<typename T>
struct  arank
{
    static size_t const rank = 0u;
    /* data */
};


template<typename T, size_t N>
struct  arank<T[N]>
{
    static size_t const rank = 1u + arank<T>::rank;
};

// below 2 is consitional implementation in c++11.
template <bool b, typename T, typename U>
struct  IF_T : type_is<T>
{};

template <typename T, typename F>
struct  IF_T<false, T, F> : type_is<F>
{};

// single type variantion on conditional, if true use T, else none.
template <bool, class T = void>
struct  enable_if_m : type_is<T>
{};

template <class T>
struct  enable_if_m<false, T>
{};
// enable_if<false>::type is not defined, so helps with SFINAE
/*
-- during the template instantiation, the compiler will figure out template argument
    - take verbatim if provided explicitly
    - decuced from function arg
    - taken from declaration default params

-- replace template parameter throughout the template by arguments.
-- if these produce well formed code, template instantiation succeed, but
-- if resulting code is ill-formed, it is considered not viable(due to substition failure) and is silently discarded.
-- above done during overload set resolution.

*/
// value returning meta functions,

template <class T, T val>
struct  integral_const_m
{
    static constexpr T value = val;
    constexpr operator T() const noexcept {return value;}
    constexpr T operator ()() const noexcept {return value;}

    /* data */
};

// rank using integral constants.
template<typename T>
struct  acrank : integral_const_m<size_t, 0u>
{};


template<typename T, size_t N>
struct  acrank<T[N]> : integral_const_m<size_t, 1u + acrank<T>::value>
{};

template<typename T>
struct  acrank<T[]> : integral_const_m<size_t, 1u + acrank<T>::value>
{};



// void_t
template <class T>
struct is_void_m : false_type {};

template <>
struct is_void_m <void> : true_type {};

template <>
struct is_void_m <void const> : true_type {};

template <class T, class U>
struct is_same_m : false_type{};

template <class T>
struct is_same_m<T,T> : true_type{};

// is void diff
template <typename T>
using is_void = is_same_m<remove_cv_t<T>, void>;


/*
template <class T>
using remove_cv = remove_volatile<remove_const<T>>;
template <class T>
using remove_cv_t = remove_cv<T>::type; 

*/



// 

template <class T, class = void>
struct has_type_member : false_type {};

template <class T>
struct has_type_member<T, void_t<typename T::type>> : true_type {};



template<typename T, T... Args>
constexpr T amax(std::integer_sequence<T, Args...>)
{

    auto mmax = make_named_operator([](auto&& a, auto&& b){ return std::max(a,b); });
    return (... + mmax(Args));
}

int main()
{

    //static_assert(has_type_member<string>::value, "String");
    //static_assert(has_type_member<vector<int>>::value, "vector");
    static_assert(has_type_member<type_is<int>>::value, "vector");

    //auto mf = maxf(1,2,40,3,33,6,1000,3,2,3,41,44,-1);
    //auto af = amax({1,2,40,3,33,6,1000,3,2,3,41,44,-1});
    // auto zf = maxf();
    //cout << af << endl;
    const int val = -100;
    cout << abs1<val>::value << " " << absf(val) << endl;
    cout << "gcd of 50, 400 ";
    cout << gcd1<50,400>::value << endl; 
    using arrt = int[10][20][30][40];
    cout << arank<arrt>::rank << endl;
    const string s = "ss";
    remove_const_m<const string>::type sc = "cc";
    cout << sc << endl;


    return 0;
}