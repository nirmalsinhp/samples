#include <bits/stdc++.h>
#include <cassert>
using namespace std;


bool twoSum(const vector<int>& arr, int target)
{
    assert(is_sorted(arr.begin(), arr.end()));
    int left = 0, right = arr.size() - 1;
    while(left < right)
    {
        int csum = arr[left] + arr[right];
        if(csum == target)
            return true;
        if(csum < target)
            left++;
        else
            right++;
    }
    
    return false;
}

int containerWithMostWater(const vector<int>& H)
{
    int maxa = 0;
    int left = 0, right = H.size() - 1;
    while(left < right)
    {
        int w = right - left;
        int mh = H[left] < H[right] ? H[left++] : H[right--];
        maxa = max(maxa, mh * w);
    }
    return maxa;
}

vector<vector<int>> three_sum(vector<int>& arr)
{
    vector<vector<int>> out;
    sort(arr.begin(), arr.end());

    for(int i = 0; i < arr.size() - 2; ++i)
    {
        if(i > 0 && arr[i] == arr[i-1])
            continue;
        int l = i + 1;
        int r = arr.size() - 1;
        while(l < r)
        {
            auto sum = arr[i] + arr[l] + arr[r];
            if(sum == 0)
            {
                out.push_back({arr[i], arr[l], arr[r]});
                while(l < r && arr[l] == arr[l+1]) l++;
                while(l < r && arr[r] == arr[r-1]) r--;
                l++; r--;
            }
            else if(sum > 0)
                r--;
            else 
                l++;
        }
    }

    return out;
}

int main()
{
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> dist(1, 100);

    auto gen = [&dist, &rng]()
        {
            return dist(rng);
        };
    vector<int> v(1000);
    vector<int> vc{1,3,4,6,8,10,13};
    generate(v.begin(), v.end(), gen);
    sort(v.begin(), v.end());
    auto found = twoSum(vc, 10);
    cout << found << endl;
    vector<int> tv{-1,0,1,2,-1,-1};
    auto triplets = three_sum(tv);
    for(auto t : triplets)
    {
        for(auto n : t)
            cout << n << " ";
        cout << endl;
    }
    auto water = containerWithMostWater(v);
    cout<< "Water : " << water << endl;

}