#include <bits/stdc++.h>

using namespace std;

template <typename T>
decltype(auto) move(T&& t)
{
    return static_cast<remove_reference_t<T> &&>(t);
}