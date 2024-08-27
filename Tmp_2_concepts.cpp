#include <bits/stdc++.h>
using namespace std;



template <typename T>
struct is_container
{
    public:
    template <typename U>
    static byte f(...)
    {
        byte b;
        cout << __PRETTY_FUNCTION__ << endl;
        return b;
    }

    template <typename U>
    static size_t f(U::iterator *)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        return 0;

    }

    static bool const value = (sizeof(f<T>(0)) == sizeof(size_t));
};

template <typename T>
constexpr inline bool is_container_v = is_container<T>::value;

template <typename T>
void print_(T t)
{
    if constexpr (is_container_v<T>)
    {
        cout << " Container : ";
        for(auto e : t)
            cout << e << ", ";
        cout << endl;
    }
    else
        cout << t << endl;
}

template <typename C>
concept NEBegEnd = requires(C c)
{
    {begin(c) != end(c) } -> same_as<bool>;
};

template <typename C>
concept Beg = requires(C c)
{
    begin(c);
};

template <typename C>
concept En = requires(C c)
{
    end(c);
};

int main()
{
    vector<int> v = {1,2,3,5,2,4,4,52};
    bool vc = is_container<vector<int>>:: value;
    bool vi = is_container<decltype(v)>:: value;
    bool vt = is_container<decltype("sngkre")>::value;
    bool vin = is_container<int>::value;
    cout << boolalpha << vc << " " << vi << " " <<vt << " " << vin<<     endl;
    print_(v);
    print_("sfre");
    print_(3.44);

}