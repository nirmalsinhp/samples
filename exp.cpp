#include <iostream>
#include <string>
#include <bits/stdc++.h>


using namespace std;

bool sp(char c)
{
    return bool(isspace(c));
}

int* invalid_pointer()
{
    int x;
    return &x; // instant disaster!
}

int main()
{

    string ss = "sss";
    auto ms(move(ss));
    cout << "s :" << ss << " " << ss.size() << endl;
    return 0;


//    auto x1 = invalid_pointer();
//    cout << *x1 << endl;
    const char * hello = "Hua hi, Hello";
    string sh(hello, hello + strlen(hello));
//    cout << sh << endl;
    vector<int> v(10,100);
    vector<int> u(10);
    copy(v.begin(), v.end(), u.begin());
    for(auto ue : u)
    {
        cout << ue << ", ";
    }
    cout << endl;
    return 0;

    string name;
    cout << "name " << name << endl;
    cout << "enter name :";
    cin >> name;
    3 + 4;
    const string greeting = "Hello " + name + "!";
    const string space(greeting.size(), ' ');
    const string second = "*" + space + "*";
    const string first(second.size(), '*');

    // string n = "nir" + "nirl"; for + to work any one operand needs to be string type
    cout << endl;
    cout << first << endl;
    cout << second << endl;
    cout << "*" << greeting << "*" << endl;
    cout << second << endl;
    cout << first << endl;
    cout << "change name :";
    cin >> name;


    auto sz1 = name.size();
    int x;
    cout << x + 1 << endl;
   //  (++x)++;
    cout <<x << endl;
    cout << "Hello " << name << "!" << endl;

    string s = "words are many, and many and many";
    auto sz = s.size();
    int i = 0;
    vector<string> vs;

    copy(vs.begin(), vs.end(), vs.end()); 
    while(i != sz)
    {
        while(i != sz && isspace(s[i]))
            ++i;

        auto j = i;
        cout << s[i] << endl;
        while(j != sz && !isspace(s[j]))
            ++j;
        
        if(i != j)
        {
            vs.push_back(s.substr(i, j - i));
            i = j;
        }
    }

    for(auto & s : vs)
    {
        cout << s << endl;
    }

    string s1 = "rotor";
    find_if(s1.begin(), s1.end(), sp);
    auto iter = partition(s.begin(), s.end(), sp);
    cout << s << endl;
    auto pal = equal(s1.begin(), s1.end(), s1.rbegin());
    //equal_range
    cout << pal << endl;

    unordered_map<int, int> hmap;
    vetor<int> res;
    int n5 = 5;
    copy_if(hmap.begin(), hmap.end(), back_inserter(res), [](auto kv){  if kv.second > n5
                                                                            return kv.first;   }
                                        )
    return 0;

}
