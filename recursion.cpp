#include <bits/stdc++.h>
using namespace std;

/*  steps"
1. find the simplest possible input.
2. relate hard case with simple case
3. generelize the pattern
4. write code.

*/

long long Sum(long n)
{
    cout << "n : " << n << endl;
    if(n == 1)
        return 1;
    return n + Sum(n - 1);
}

int gridPaths(int m, int n)
{
    cout << "m , n : " << m << " " << n << endl;
    if(n == 1 || m  == 1)
        return 1;
    return gridPaths(n - 1, m) + gridPaths(n, m - 1);
}

int main()
{
    auto val = Sum(10);
    cout << "sum :" << val << endl;
    auto paths = gridPaths(4, 3);
    auto paths2 = gridPaths(9, 6);
    cout << paths << endl;
    cout << paths2 << endl;
    return 0;
}