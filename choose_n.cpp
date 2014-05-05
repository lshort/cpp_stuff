#include <vector>
#include <set>
#include <utility>
#include <random>
#include <string>
#include <iostream>

using namespace std;

set<int> choose_n(vector<int> data, int n)
{
    int size = data.size();
    if (n>=size)
        return set<int>(data.begin(),data.end());
    for (int i=1; i<size; ++i) {
        int targ = rand() % (1+i);
        std::swap(data[i], data[targ]);
    }
    return set<int>(data.begin(),data.begin()+n);
}

set<int> choose_n_big_input(vector<int> data, int n)
{
    int size = data.size();
    if (n>=size)
        return set<int>(data.begin(),data.end());

    set<int> rval;
    for (int i=0; i<n; ++i)  {
        int target = rand() % (size-i);
        int offset = 0;
        for (auto x : rval)
            if (x < target)
                ++offset;
        rval.insert(data[target+offset]);
    }
    return rval;
}

int main()
{
    vector<int> d = {1,2,3,4,5,6,7,8,9,10};
    auto s = choose_n(d, 5);
    cout << s.size() << endl;
    for ( auto m : s )
        cout << m << " ";
    cout << endl;

}
