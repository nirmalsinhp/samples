#include <bits/stdc++.h>
using namespace std;

auto vglambda = [](auto printer)
                { return [=](auto &&...ts)
                    {
                        printer(std::forward<decltype(ts)>(ts)...);
                        return[=]() 
                        {
                            printer(ts...);
                        }; 
                    }; 
                };
int main()
{
    string s1 = "3";
    string s2 = "30";
    vector<string> vs{"3", "30", "300", "33", "3000", "0300"};
    sort(vs.begin(), vs.end(), [](auto s1, auto s2)
                                {
                                    return lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end());
                                });
    for(auto s : vs)
        cout << s << " ";
    cout << endl;
    cout  << boolalpha << !lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end()) << endl;
    return 0;
}