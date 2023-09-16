#include <bits/stdc++.h>

using namespace std;
// fixed size sliding window
int findMaxSubArraySum(const vector<int>& arr, int k)
{
    int maxSum = INT_MIN;
    int currSum = 0;

    for(int i = 0; i < arr.size(); ++i)
    {
        currSum += arr[i];
        if(i >= k - 1) // reached K // window size
        {
            maxSum = max(maxSum, currSum);
            currSum -= arr[i - (k - 1)]; // slide & subtract the furthesr left  value.
        }
    }
    return maxSum;
}

// dynamic size sliding window
int smallestSubArray(const vector<int>& arr, int sum)
{
    int wSize = INT_MAX;
    int currSum = 0;
    int ws = 0;
    for(int we = 0; we < arr.size(); ++we)
    {
        currSum += arr[we];
        while(currSum >= sum) // check if condition is satisfied,
        {
            wSize = min(wSize, we - ws + 1);
            currSum -= arr[ws]; // try to improve/optimize further till cond is true.
            ws++;
        }
    }
    return wSize;

}

void decrFreq(unordered_map<char, int>& map, char c)
{
    if(--map[c] == 0)
        map.erase(c);
}

int longestSubStrDist(const string& s, int k)
{
    int maxS = INT_MIN;
    int currS = 0;
    unordered_map<char, int> freq;
    int ws = 0;
    for(int we = 0; we < s.size(); ++we)
    {
        currS++;
        freq[s[we]]++;
        while(freq.size() > k)
        {
            decrFreq(freq, s[ws]);
            currS--;
            ws++;
            maxS = max(maxS, currS);
        }
    }
    return (maxS == INT_MIN) ? currS : maxS;
}

int findMinOps(const vector<int>& arr, int x)
{
    auto sum = accumulate(arr.begin(), arr.end(), 0);
    int target = sum - x;
    if(target < 0)
        return -1;
    int currSum = 0;
    int wSize = INT_MIN;
    int ws = 0;
    for(int we = 0; we < arr.size(); ++we)
    {
        currSum += arr[we];
        while(currSum > target) // check if condition is satisfied,
        {                
            currSum -= arr[ws]; // try to improve/optimize further till cond is true.
            ws++;
        }
        if(currSum == target)
        {
            wSize = max(wSize, we - ws + 1);
        }
    }
    if(wSize == INT_MIN)
        return -1;
    return arr.size() - wSize;
}

    vector<int> findSubstring(string s, vector<string>& words) {
        vector<string> perms;
        vector<int> ret;
        sort(words.begin(), words.end());
        do
        {
            string str = "";
            for(const auto w: words )
                str += w;
            perms.push_back(str);
        }while(next_permutation(words.begin(), words.end()));

        string current = "";
        int csz = perms[0].size();
        if(s.size() < csz)
            return ret;
        int start = 0;
        for(int i = 0; i < s.size(); ++i)
        {
            current += s[i];
            if( current.size() >= csz)
            {
                if(find(perms.begin(), perms.end(), current) != perms.end())
                {
                    ret.push_back(start);
                }
                current.erase(current.begin());
                start++;
            }
        }
        return ret;
    }
int main()
{
    vector<int> vec{ 4, 2, 1, 7, 8, 1, 2, 8, 1, 0};
    auto maxSum = findMaxSubArraySum(vec, 3);
    int minW = smallestSubArray(vec, 8);
    int minW11 = smallestSubArray(vec, 11);
    cout << "MaxSum : " << maxSum << endl;
    cout << "minW8 : " << minW << endl;
    cout << "minW11 : " << minW11 << endl;
    string S = "AAAHHIBCC";
    auto maxStr = longestSubStrDist(S, 2);
    auto maxStr3 = longestSubStrDist(S, 4);

    cout << "maxS :" << maxStr << endl;
    cout << "maxS3 :" << maxStr3 << endl;

    vector<int> arrs{3,2,20,1,1,3};
    auto ops = findMinOps(arrs, 10);
    cout << ops << endl;
    vector<int> arr{1,1};
    auto ops1 = findMinOps(arr, 3);
    cout << ops1 << endl;
 
    string cons = "barfoothefoobarman";
    vector<string> words = {"foo","bar"};
    auto vret = findSubstring(cons, words);
    copy(vret.begin(), vret.end(), ostream_iterator<int>(cout, " "));
    return 0;
}