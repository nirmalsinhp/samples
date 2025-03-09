#include <bits/stdc++.h>
#include <execution>
using namespace std;
/*
- Parrallel algos
    extra first arg to normal algo signature to specify execution policy, it is permission, not requirement to do it in parallel.
- policies
    std::execution::sequenced_policy
    std::execution::parallel_policy
    std::execution::parallel_unsequenced_policy
- it affects
    - The algorithm’s complexity : may increase
    - The behavior when an exception is thrown : will call terminate & not propogate exception
    - Where, how, and when the steps of the algorithm are executed
- if the “normal” algorithm allows Input Iterators or Output Iterators, then the overloads with an execution policy require Forward Iterators instead
*/

int main()
{
    vector<int> v(1000);
    int count = 0;
    for_each(execution::seq, v.begin(), v.end(),
        [&](int& x) { x = ++count; });
    for(auto n : v)
        cout << n << " ";
    cout << endl;
    v.clear();
    count = 0;
    for_each( v.begin(), v.end(),
        [&](int& x) { x = ++count; });
    for(auto n : v)
        cout << n << " ";
    cout << endl;

}