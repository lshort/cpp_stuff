#include "incs.h"

using namespace std;

unsigned int sum_to_n(unsigned int n)
{
    return (n * (n+1))/2;
}


int sum_sqr_diff(const int x)
{
    int sqr_sum=0;
    for (int i=1; i<=x; ++i) {
        sqr_sum += pow(i,2);
    }
    cout << sqr_sum << endl;
    return (pow(sum_to_n(x),2) - sqr_sum);
}

int main()
{
    cout << sum_sqr_diff(100) << endl;

}
