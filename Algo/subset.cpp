#include <bits/stdc++.h>
using namespace std;


// imagine a solution-space tree.
// 1. recurse to possible neighbors
// 2. prune the tree if the solution is not feasible.
// 3.  if the solution is feasible, add to the solution set.
// 4.  if the solution is not feasible, backtrack to the previous state.

constexpr int N = 8;
vector<int> vec{1,2,3};
vector<vector<int>> powerset(const int n)
{
    vector<vector<int>> res;
    vector<bitset<32>> bsets;
    int nc = 1 << n;
    cout << "Sets " << nc << endl;
    for(int i = 0 ; i < nc; i++)
    {
        bsets.push_back(bitset<32>(i));
    }

    for(const auto & b : bsets)
    {
        vector<int> set;
        for(int i = 0; i < n; i++)
        {
            if(b[i])
                set.push_back(i);
        }
        res.push_back(move(set));
    }
    return res;
}

vector<vector<int>> bpowerset(const int n)
{
    vector<vector<int>> res;
    int nc = 1 << n;

    for(int m = 0; m < nc; m++)
    {
        vector<int> set;
        for(int i = 0; i < n; i++)
        {
            if(m & (1 << i))
                set.push_back(i);
        }
        res.push_back(move(set));
    }
    return res;
}

vector<vector<int>> comb(const vector<int>& C) // Use const reference
{
    vector<vector<int>> res;
    res.push_back({});
    
    for(int i = 0; i < C.size(); i++)
    {
        int sz = res.size();
        for(int j = 0; j < sz; j++)
        {
            auto vec = res[j];
            vec.push_back(C[i]);
            res.push_back(vec);
        }
    }

    return res;
}

void print(const auto & c)
{
    cout << "{ ";
    for(auto n : c)
        cout << n << " ";
    cout << " }" << endl;
}
int main()
{
    auto pset = powerset(4);
    auto bpset = bpowerset(4);
    for(const auto& S : pset)
    {
        print(S);
    }
    cout << "bits :" << endl;
    for(const auto& S : bpset)
    {
        print(S);
    }
    vector<int> vec{1,2,3,4,5};
    auto cset = comb(vec);
    cout << "combs" << endl;
    for(const auto& S : cset)
    {
        print(S);
    }

    return 0;
}