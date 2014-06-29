#include "incs.h"
#include <cmath>
#include <algorithm>

using namespace std;

vector<unsigned long long> primes(const unsigned long long x)
{
    vector<bool> maybe_prime(x,1);
    vector<unsigned long long> rval;
    for (unsigned int i=2; i<x; ++i)  {
        if (1==maybe_prime[i]) {
            rval.push_back(i);
            for (int k=2; k*i<x; ++k) {
                maybe_prime[k*i] = 0;
            }
        }
    }
    cout << rval.back() << endl;
    return rval;
}

unsigned long long sum_primes(unsigned long long x)
{
    auto primelist = primes(x);
    cout << primelist;
    long long sum = 0;
    for ( auto x : primelist)
        sum += x;
    return sum;
    //    return accumulate(primelist.begin(), primelist.end(), 0,
    //[] (unsigned long long x, unsigned long long y) {return x+y;});
}



int main()
{
    cout << sum_primes(2000000) << endl;

}
