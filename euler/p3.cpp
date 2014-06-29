#include "incs.h"
#include <cmath>

using namespace std;

unsigned long long lpf(const unsigned long long x)
{
    unsigned int limit = (unsigned long long) floor(sqrt(x));
    vector<bool> maybe_prime(limit,1);
    int largest_prime_factor = 1;
    for (int i=2; i<limit; ++i)  {
        if (1==maybe_prime[i]) {
            if (x%i==0)
                largest_prime_factor = i;
            for (int k=2; k*i<limit; ++k) {
                maybe_prime[k*i] = 0;
            }
        }
    }
    return largest_prime_factor;
}

int main()
{
    cout << lpf(600851475143) << endl;



}
