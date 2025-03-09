#include <bits/stdc++.h>
#include <chrono> // Added missing include

using namespace std;

const int R = 26;

/* char Ops:

Ops,
prefix match
regex match(wildcard)
longest prefix
*/

struct node
{
    bool end = false;
    node* child[R];
};

class Trie
{
private:
    node* root;
    int sz = 0;

public:
    Trie()
    {
        root = new node();
    }
    ~Trie() = default;

    void put(const string& s)
    {
        root = put(root, s, 0);
    }

    node* put(node* root, const string& s, int len)
    {
        if (!root) root = new node();
        if (s.length() == len) { root->end = true; ++sz; return root; }
        int idx = s[len] - 'a';
        root->child[idx] = put(root->child[idx], s, len + 1);
        return root;
    }

    bool get(const string& s)
    {
        auto node = get(root, s, 0);
        return (node);
    }

    node* get(node* root, const string& s, int len)
    {
        if (!root) return nullptr;
        if (s.length() == len) return root;
        int idx = s[len] - 'a';
        return get(root->child[idx], s, len + 1);
    }


    vector<string> keys_with_prefix(const string& prefix = "")
    {
        vector<string> ks;
        auto node = get(root, prefix, 0);
        collect(node, prefix, ks);
        return ks;
    }

    vector<string> keys()
    {
        return keys_with_prefix();
    }

    vector<string> keys_that_match(const string& s)
    {
        vector<string> ks;
        collect(root, "", s, ks);
        return ks;
    }

    string longest_prefix_of(const string& s)
    {
        int len = search(root, s, 0, 0);
        return s.substr(0, len);
    }

    int size()
    {
        return sz;
    }

    void t_delete(const string& s)
    {
        root = t_delete(root, s, 0);
    }

    node* t_delete(node * root, const string& key, int len)
    {
        if(!root) return nullptr;
        if(len == key.size()) root->end = false;
        else
        {
            int idx = key[len] - 'a';
            root->child[idx] = t_delete(root->child[idx], key, len + 1);
        }

        if(root->end) return root;

        for(int i = 0; i < R; ++i)
            if(root->child[i]) return root;
        return nullptr;
    }
private:
    void collect(node* root, string prefix, vector<string>& keys)
    {
        if (!root) return;
        if (root->end) keys.push_back(prefix);
        for (int i = 0; i < R; ++i)
        {
            char c = 'a' + i;
            collect(root->child[i], string(prefix + c), keys);
        }
    }

    // collect for pattern match
    void collect(node* root, string prefix, string pattern, vector<string>& keys)
    {
        if (!root) return;
        if (prefix.length() == pattern.length() && root->end) keys.push_back(prefix);
        if (prefix.length() == pattern.length()) return;
        char c = pattern[prefix.length()];
        for (int i = 0; i < R; ++i)
        {
            if (c == '.' || c == 'a' + i)
                collect(root->child[i], prefix + char('a' + i), pattern, keys);
        }
    }

    int search(node* root, const string& s, int idx, int len)
    {
        if (!root) return len;
        if (root->end) len = idx;
        if (s.length() == len) return len;
        int id = s[idx] - 'a';
        return search(root->child[id], s, idx + 1, len);
    }

};

int main()
{
    Trie T;
    T.put("she");
    T.put("shell");
    T.put("share");
    T.put("shore");
    T.put("sell");
    T.put("nirmal");
    T.put("bhut");
    T.put("baby");
    T.put("constant");
    T.put("circuit");
    T.put("common");
    T.put("put");
    cout << boolalpha << endl;
    cout << T.get("sell") << endl;
    cout << T.get("sells") << endl;
    cout << T.get("nirmal") << endl;
    auto keys = T.keys();
    cout << "Keys" << endl;
    for (auto& k : keys)
        cout << k << " ";
    cout << endl;
    auto pkeys = T.keys_with_prefix("she");
    cout << " P Keys" << endl;
    for (auto& k : pkeys)
        cout << k << " ";
    cout << endl;
    auto mkeys = T.keys_that_match("s.e");
    cout << " M Keys" << endl;
    for (auto& k : mkeys)
        cout << k << " ";
    cout << endl;
    auto pf = T.longest_prefix_of("shorecircut");\
    cout << "Longest Prefix :" << pf << endl;
    cout << endl;
    cout << "Size: " << T.size() << endl;
    T.t_delete("shell");
    T.t_delete("she");
    keys = T.keys();
    cout << "Keys" << endl;
    for (auto& k : keys)
        cout << k << " ";
    cout << endl;
}
