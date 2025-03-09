#include <bits/stdc++.h>
using namespace std;

class from_string
{
public:
from_string(string S)
{
    cout << __PRETTY_FUNCTION__ << endl;
    if(S.find('.') != string::npos)
    { _data = stof(S);}
    else {   _data = stoi(S);    }
}
operator int() const
{
    cout << __PRETTY_FUNCTION__ << endl;
    return get<int>(_data);
}

operator float() const
{
    cout << __PRETTY_FUNCTION__ << endl;
    return get<float>(_data);
}
private:
variant<int, float> _data;

};



int main()
{
    int i = from_string("123");
    float f = from_string("123.123");
    double s = from_string("123.123");

    cout << i << " " << f << " " << s << endl;
}