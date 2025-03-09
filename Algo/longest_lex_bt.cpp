#include <bits/stdc++.h>
using namespace std;


bool dfs(int idx, vector<int>& ans, unordered_set<int> visit, int target) {
    cout << idx << endl;
    // condition satisfied.
    if (idx == ans.size())
    {
        cout << "Final ret" << endl;
        return true;
    }

    if (ans[idx] != 0)
        return dfs(idx + 1, ans, visit, target);
    else {
        for (int i = target; i >= 1; --i) {
            if (visit.contains(i))
                continue;
            visit.insert(i);
            ans[idx] = i;
            if (i == 1) {
                if (dfs(idx + 1, ans, visit, target))
                    return true;
            }
            else if (idx + i < ans.size() && ans[idx + i] == 0) {
                ans[idx + i] = i;
                if(dfs(idx + 1, ans, visit, target)) return true;
                ans[idx + i] = 0;
            }

            ans[idx] = 0;
            visit.erase(i);
        }
    }
    return false;
}

vector<int> constructDistancedSequence(int n) {
    unordered_set<int> visit;
    vector<int> ans = vector<int>(2 * n - 1, 0);
    dfs(0, ans, visit, n);
    return ans;
}

int main()
{
    auto ans = constructDistancedSequence(30);
    for (auto a : ans)
        cout << a << " ";
    cout << endl;
    return 0;
}