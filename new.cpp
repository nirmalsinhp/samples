#include <bits/stdc++.h>
using namespace std;


void exp() noexcept
{
    throw 42;
    return;
}

void f(int i)
{
    std::cout << "int" << endl;
}

void f(void * v)
{
    std::cout << "void" << endl;
}

void fs(const unique_ptr<int> & up)
{
    std::cout << "smart" << endl;
}

using RT = variant<int, double, long>;
//template <typename T>
// auto from_string(string s)
// {
//     int ret = 0;
    
//     if(s.empty())
//         return ret;
//     int dotc = count(s.begin(), s.end(),'.');
//     if( dotc > 1)
//         throw runtime_error("wrong numver");
//     stringstream ss(s);
//     if(dotc)
//     {
//         double d;
//         ss >> d;
//         return d;
//         ret = d;
//     }
//     else
//     {
//         int i;
//         ss >> i;
//         return i;
//         ret = i;
//     }
//     return ret;
// }

vector<int> collect_min(vector<int> vec)
{
    vector<int> out;
    int min = vec.front();
    for(auto v : vec)
    {
        if(v < min)
        {
            out.clear();
            out.push_back(v);
            min = v;
        }
        else if(v == min)
        {   
            out.push_back(v);
        }
    }

    return out;
}

int main()
{
    vector<int&> vr;
    vector<any> va;
    va.push_back(string("vf"));
    va.push_back(2.2);
    va.push_back(2);
    va[2] = string("mvewmg");
    cout << any_cast<string>(va[2]) << endl;
    vector<int> vvi{1,2,2,232,2,32,232,122,33,4,4,2};
    ranges::sort(vvi, less{});

    for(auto n : vvi)
        cout << n << " ";
    cout << endl;
    optional<string> os;
    cout << os.value_or("khali") << endl;
    os = "vewkn";
    cout << os.value_or("khali") << endl;
    //cout << (os ? *os << "empty") << endl;
    return 0;

    //   *(char*) 0 = 0;
    // *reinterpret_cast<char *>(nullptr) = 0; does not compile
    // *bit_cast<char*>(nullptr) = 0;
    // *__builtin_bit_cast(char *, nullptr) = 0;
    any a1 = string("nirmal");
    cout << any_cast<int>(a1) << endl;
    //int ai = any_cast<int>(a1);//
    //cout << ai << endl;
    // int val = from_string("111");
    // float val2 = from_string("111.111");
    // int val2 = from_string("111111111");
    // float val3 = from_string("111.111");
    // from_chars()
//    std::cout << val << " " << val2 << " " << val3 << endl;
    // auto xx;  can not left uninitialized.
    //cout << val << endl;
    return 0;
    unordered_map<int, int> umap;
    umap.insert({1,1});
    umap.equal_range(1);
    auto[iter, inserted] = umap.insert({1,2});
    std::cout << boolalpha << inserted << endl;
    std::cout << umap[1] << endl;
 
    unordered_multimap<int, int> ummap;
    ummap.insert({1, 1});
    ummap.insert({1, 2});
    auto[fiter, liter] = ummap.equal_range(1);

//    copy(fiter, liter, ostream_iterator<pair<int, int>>(std::cout, " "));

    for(auto i = fiter; i != liter; ++i)
    {
        std::cout << i->first << " " << i->second << endl;
    }

    vector<int> v1 {1,2,3,4,4};
    vector<int> v2;
    vector<int> v3(10);
    vector<int> v4 = move(v1);
    std::cout << endl;
    // reverse()
    copy(v1.begin(), v1.end(), back_inserter(v2));
    copy(v1.begin(), v1.end(), ostream_iterator<int>(std::cout, " "));
    std::cout << endl;
    copy(v1.crbegin(), v1.crend(), ostream_iterator<int>(std::cout, " "));
    std::cout << endl;
    iota(v3.begin(), v3.end(), 10);
    copy(v3.begin(), v3.end(), ostream_iterator<int>(std::cout, " "));
    std::cout << endl;
    
    // auto array
    const char cr[] = "this is array!!";
    auto acr = cr; // deduced const char *
    auto & rcr = cr; // deduced const char(&)[size]
    auto di = {12.2}; // intitilizer_list <T>

    f(0);
    f(nullptr);
    auto dp =  new double(22.22);
    unique_ptr<double> up = nullptr;
    // unique_ptr<double> udp = dp; does not compile
    unique_ptr<double> uddp(dp);
    //up = dp; // does not compile
    
    fs(nullptr); // works only when passed by type or const ref, can not bind smart pointer ref to
    // f(NULL);
    auto ci =  v1.cbegin();
    //auto iit = static_cast<vector<int>::iterator>(ci);
    int x = 312;
    string s = to_string(x);
    reverse(begin(s), end(s));
    auto xx = stoi(s);
    std::cout << xx << endl;
    vector<regex> vr;
    vr.emplace_back(nullptr);
    try
    {
        //exp(); // terminates
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
 //   vr.push_back(nullptr);

    return 0;
}