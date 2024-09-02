#include <bits/stdc++.h>
using namespace std;


vector<int> getPrioIndex(vector<int>& vec)
{
    auto cmp = [](const pair<int, int>& p1, const pair<int, int>& p2 )
                {
                    if(p1.first == p2.first)
                        return (p1.second > p2.second);
                    else
                        return (p1.first > p2.first);
                };
    
    priority_queue<pair<int,int>, vector<pair<int, int>>, decltype(cmp) > pq(cmp);
    int i = 0;
    for(auto v : vec)
    {
        pq.push(pair<int, int>(v, i++));
    }
    
    vector<int> ret;
    
    while(!pq.empty())
    {
        ret.push_back(pq.top().second);
        pq.pop();
    }

    return ret; // should move with nrvo
}

vector<int> topK(vector<int>& vec, int k)
{
    vector<int> result;
    unordered_map<int, int> hmap;
    for(auto n : vec)
        hmap[n]++;
    priority_queue<pair<int, int>> pq;
    for(auto &[k, v] : hmap)
    {
        pq.push({v, k});
    }

    for(int i = 0; i < k; ++i)
    {
        result.push_back(pq.top().second);
        pq.pop();
    }
    return result;
}


int main()
{
    /*vector<int> vec;
    cout << "enter queue : " << endl;
    copy(istream_iterator<int>(cin), istream_iterator<int>(), back_inserter(vec));

    auto ret = getPrioIndex(vec);
    copy(ret.cbegin(), ret.cend(), ostream_iterator<int>(cout, ", "));
    cout << endl;
*/
    vector<int> vec1{1,1,1,1,1,1,1,2,2,3,3,12,2,1,31,1,31,1,31,21,21,31,2,12,1,3,2,32,4,2,32,3};
    auto res = topK(vec1, 4);
    copy(res.cbegin(), res.cend(), ostream_iterator<int>(cout, ", "));

}