#include "incs.h"
#include <cmath>

using namespace std;

vector<unsigned int> primes(const unsigned int x)
{
    vector<bool> maybe_prime(x,1);
    vector<unsigned int> rval;
    for (unsigned int i=2; i<x; ++i)  {
        if (1==maybe_prime[i]) {
            rval.push_back(i);
            for (int k=2; k*i<x; ++k) {
                maybe_prime[k*i] = 0;
            }
        }
    }
    return rval;
}

vector<unsigned int> prime_factors(unsigned int x,
                                   const vector<unsigned int> &primes)
{
    vector<unsigned int> rval(primes.size(),0);
    int i=0;
    while ( x>1 && x>=primes[i] && i < primes.size())
        if (0 == x%primes[i]) {
            ++rval[i];
            x = x/primes[i];
        } else {
            ++i;
        }
    return rval;
}

unsigned long long scm(unsigned int x)
{
    auto primelist = primes(x);
    vector<unsigned int> biggest_factors(primelist.size(),0);
    for (int i=2; i<=x; ++i)   {
        auto factors = prime_factors(i,primelist);
        for (int k=0; k<primelist.size(); ++k)
            if (factors[k] > biggest_factors[k])
                biggest_factors[k] = factors[k];
    }
    unsigned long long int mult = 1;
    cout << primelist << endl;
    cout << biggest_factors << endl;
    for (int k=0; k<primelist.size(); ++k)
        mult *= pow(primelist[k],biggest_factors[k]);
    return mult;
}



int main()
{
    cout << scm(20) << endl;

}
