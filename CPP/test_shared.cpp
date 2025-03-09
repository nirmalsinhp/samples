#include <bits/stdc++.h>
#include "shared.hpp"
using namespace std;


int main()
{
    int * pi = new int(20);
    //int* pi = &i;
    shared_ptr<int> s1(pi);
    //shared_ptr<int> s2(pi);
    shared_ptr<int> s2(s1);
    sharedptr<int> si1 = new int(20);
    sharedptr<int> si2 = new int(2);
    auto si3 = si2;
    si1 = si2;
    si2.reset();
    cout << si1.count() << endl;
    cout << si2.count() << endl;
    cout << si3.count() << endl;
    return 0;
}