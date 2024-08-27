#include <bits/stdc++.h>
using namespace std;
class SettingsManager
{
   
   private :
   unordered_map<string,any> settings_;
    public :
    template<typename T>
    void setSetting(const string& name , const T& value)
    {
        settings_[name]=value;
    }
   
    template<typename T>
    T getSetting(const string& name) const
    {
        auto it = settings_.find(name);
        if(it != settings_.end())
            return any_cast<T>(it->second);
        return T{};
    }
};



int main()
{
SettingsManager sm;
    sm.setSetting("Themecolor", string ("Blue"));
    sm.setSetting("FontSize",12);
   
    cout<<"ThemeColor : " << sm.getSetting<int>("Themecolor") << endl;
return 0;
}