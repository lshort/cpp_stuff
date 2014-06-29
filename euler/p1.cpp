#include "incs.h"

using namespace std;

int sum_of_mults(const int x)
{
    int sum=0;
    for (int i=3; i<x; ++i) {
        if (((i%5)==0) || ((i%3)==0))  {
            sum += i;
        }
    }
    return sum;

}

int main()
{
    cout << sum_of_mults(1000) << endl;



}
