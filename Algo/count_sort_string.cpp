#include <bits/stdc++.h>
using namespace std;

const int R = 26;
string count_sort(string s)
{
    vector<int> cnt(R+1, 0);
    string cs(s.size(), ' ');
    for(auto c : s)
        cnt[c - 'a' + 1]++;
    for(int i = 0; i < R + 1; ++i)
        cnt[i+1] += cnt[i];
    for(auto c : cnt)
        cout << c << " ";
    cout << endl; 
    for(auto c : s)
        cs[cnt[c - 'a']++] = c;
    return cs;
}


int main()
{
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> dist(0,25);
    auto gen = [&dist, &rng]()
        {
            return 'a' + dist(rng);
        };
    string s(100, '*');
    ranges::generate(s, gen);
    //string s = "thisisaveryloingstringforcomparisonslookssmallbutshoudnensolinf";
    auto cs = count_sort(s);
    cout << s << endl << cs << endl;
    // cout << is_sorted(cs.begin(), cs.end());
    cout << ::ranges::is_sorted(cs) << endl;
}