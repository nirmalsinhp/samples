#include <bits/stdc++.h>
using namespace std;

class B
{
    public:
    virtual void func()
    {
        cout << "BF" << __PRETTY_FUNCTION__ << endl;
    }
    void func(char c)
    {
        cout << "DF" << __PRETTY_FUNCTION__ << endl;
    }

};

class D : public B
{
    public:
    virtual void func()
    {
        cout << "VDF" << __PRETTY_FUNCTION__ << endl;
    }
};

int main()
{
    map<int,int> m;
    auto it = m.begin();
    B* b = new D();
    b->func();
    mutex m1, m2;
 /* mutex copy ctor is deleted, and move ctor is not defined by default so move or copy does not work on it.
    mutex m3 = m1;
    mutex m4 = move(m2); */
    return 0;
}