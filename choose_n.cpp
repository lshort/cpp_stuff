#include <vector>
#include <set>
#include <utility>
#include <random>
#include <string>
#include <iostream>

using namespace std;


/**  Randomly choose n members of the input vector.  This runs
     in O(n^2) time.
     @param[in] data The input vector
     @param[in] n How many members to select
     @return A vector of the chosen members  */
vector<int> choose_n_big_input(const vector<int> &data, int n)
{
    int size = data.size();
    if (n>=size)
        return vector<int>(data.begin(),data.end());

    vector<int> rval;
    vector<int> used_indices; // this is kept sorted
    for (int i=0; i<n; ++i)  {
        int target = rand() % (size-i);
        int offset = 0;
        auto it = used_indices.begin();
        while (it!=used_indices.end() && *it <= target+offset) {
            ++offset;
            ++it;
        }
        used_indices.emplace(it, target+offset);
        rval.push_back(data[target+offset]);
    }
    return rval;
}

/**  Randomly choose n members of the input vector.  This dispatches
     to choose_n_big_input if the size of the input is more than n^2.
     If not dispatched, runs in time linear with the length of the vector
     @param[in] data The input vector
     @param[in] n How many members to select
     @return A vector of the chosen members  */
vector<int> choose_n(const vector<int> &data, int n)
{
    int size = data.size();
    if (n>=size)
        return vector<int>(data.begin(),data.end());
    if (n*n<size)
        return choose_n_big_input(data, n);
    vector<int> tmp(data);
    for (int i=1; i<size; ++i) {
        int targ = rand() % (1+i);
        std::swap(tmp[i], tmp[targ]);
    }
    return vector<int>(tmp.begin(),tmp.begin()+n);
}

/**  Randomly choose n members of the input vector.  Reorders the
     input vector as a destructive side effect.
     @param[in] data The input vector
     @param[in] n How many members to select
     @return A vector of the chosen members  */
vector<int> choose_n_destructive(vector<int> &data, int n)
{
    int size = data.size();
    if (n>=size)
        return vector<int>(data.begin(),data.end());
    for (int i=0; i<n; ++i) {
        int target = i + (rand()%(size-i));
        std::swap(data[i], data[target]);
    }
    return vector<int>(data.begin(),data.begin()+n);
}


int main()
{
    vector<int> d = {1,2,3,4,5,6,7,8,9,10};
    auto s = choose_n(d, 3);
    cout << s.size() << endl;
    for ( auto m : s )
        cout << m << " ";
    cout << endl;

}
