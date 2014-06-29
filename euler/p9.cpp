#include "incs.h"
#include <cmath>
#include <algorithm>

using namespace std;

int product(const int x)
{
    int third = x/3;
    for (int i=1+third; i<x; ++i) {
        int lowest_possible = max(1,x-2*i);
        int highest_possible = min(i,x-i);
        for (int k=lowest_possible; k<highest_possible; ++k) {
            int j = x - i - k;
            cout << i << " " << j << " " << k << endl;
            if ( pow(i,2) == (pow(k,2) + pow(j,2)) )
                return i*j*k;
        }
    }
    return 0;
}

int main()
{
    cout << product(1000) << endl;



}
