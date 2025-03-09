#include <bits/stdc++.h>
#include <tuple>
using namespace std;

int main()
{
    tuple<double, int, double, string> tp{100, 101, 101.101, "102"};
     //cout << "Type : " << endl; 
     //cout << get<double>(tp) << endl;
    // cout << get<string>(tp) << endl;
    // cout << get<int>(tp) << endl;
    //cout << get<long long>(tp) << endl;

    cout << "index : " << endl; 
    cout << get<0>(tp) << endl;
    cout << get<1>(tp) << endl;
    cout << get<2>(tp) << endl;
    cout << get<3>(tp) << endl;
   // cout << get<100>(tp) << endl;
    //cout << get<-100>(tp) << endl;


}