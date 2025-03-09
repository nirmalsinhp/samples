#include <bits/stdc++.h>
#include <ranges>
using namespace std;

    vector<int> maximumBeauty(vector<vector<int>>& items,
                              vector<int>& queries) {

        vector<int> out;
        sort(items.begin(), items.end(), [](const auto& v1, const auto& v2) {
            if (v1[0] == v2[0])
                return v1[1] > v2[1];
            else
                return v1[0] < v2[0];
        });
        // for(auto item : items)
        // {
        //     for(auto i : item)
        //         cout << i << " ";
        //     cout << endl;
        // }
        for (int i = 0; i < queries.size(); ++i) {
            auto li = lower_bound(
                items.begin(), items.end(), queries[i],
                [](const auto& item, int val) { return item[0] < val; });
            cout << queries[i] <<  endl;
            if (li == items.end())
            {
                out[i] = 0;
                cout << " end " << endl;
            }
            else {
                int me = 0;
                for(auto iter = items.begin(); iter <= li; iter++)
                {
                    cout << "Q : ";

                    auto vec = *iter;
                    for(auto v : vec)
                    {
                        cout << v << " ";
                    }
                    cout << vec[1];
                    me  = max(vec[1], me);
                    cout << " L " << me << " ";
                    cout << endl;

                }
                out[i] = me;
            }
        }
        return out;
    }


int main()
{
    
    vector<vector<int>> vec = {{1,2},{3,2},{2,4},{5,6},{3,5}};
    vector<int> Q = {1,2,3,4,5,6,7,8,9,0,111,12,2,3,23243,434332};
    auto odd = ranges::count_if(Q, [](auto i)
                                    {
                                        return i % 2;
                                    });
    cout << odd << endl;
    auto t5 = ranges::views::filter(Q, [](auto i)
                                {
                                    return i % 2;
                                });
    for(auto t : t5)
        cout << t << " ";
    cout << endl;
    //cout << t5.data() << endl;
    cout << Q.data() << endl;
    //auto out = maximumBeauty(vec, Q);
    for(auto q : Q)
        cout << q << " ";
    cout << endl;

    auto drop_two = Q | std::views::take(5)
    | std::views::filter([](double x) {return x < 5.0;})
    | std::views::transform([](double x) {return x * x;})
    | std::views::drop(2);

    for(auto q : drop_two)
        cout << q << " ";
    cout << endl;
    //cout << drop_two.data() << endl;
    cout << erf(10) <<  " " << erf(100)<< " " << erf(999) << endl;
    cout << erf(-10) <<  " " << erf(1111.111)<< " " << erf(1233.999) << endl;
    return 0;
}