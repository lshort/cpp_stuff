#include <vector>
#include <set>
#include <utility>
#include <random>
#include <string>
#include <iostream>

using namespace std;


/**  Randomly choose n members of the input vector.  This runs
     in O(n) time and uses O(1) space.
     @param[in] data The input vector
     @param[in] n How many members to select
     @return A vector of the chosen members  */
vector<int> choose_n(vector<int> &data, int n)
{
    int size = data.size();
    if (n>=size)
        return vector<int>(data.begin(),data.end());
    vector<int> rval(n,0);
    // on the first pass rval has the order of the swaps performed,
    // later we will fill it with the return values
    for (int i=0; i<n; ++i) {
        int target = i + (rand()%(size-i));
        rval[i] = target;
        std::swap(data[i], data[target]);
    }
    //  the first n elements of data have been randomly selected,
    //  now we need to move them into place
    for (int k=n-1; k>=0; --k) {
        int val = data[k];
        std::swap(data[k],data[rval[k]]);
        rval[k] = val;
    }
    return move(rval);
};



/**  Prints out any container of any class, provided the class has operator <<
     @param[in] ostr The output stream
     @param[in] xs The container
     @return Returns the output stream      */
template< typename T,
          template<typename El,
                   typename Alloc=std::allocator<El> > class Container >
ostream &operator<< (ostream &ostr, const Container<T> &xs)
{
    ostr << "{ ";
    for ( auto &x : xs )
        ostr << x << ",";
    ostr << " }";
    return ostr;
}


int main(int argc, char *argv[])
{
    vector<int> d = {1,2,3,4,5,6,7,8,9,10};

    vector<unsigned int> counts(11,0);

    for (int i=0; i<atoi(argv[1]); ++i)  {
        auto s = choose_n(d, 3);
        ++counts[s[0]-1];
        ++counts[s[1]-1];
        ++counts[s[2]-1];
    }
    cout << "******** counts ********" << endl;
    cout << counts << endl;
}
