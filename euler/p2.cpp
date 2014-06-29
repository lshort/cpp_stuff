#include "incs.h"

using namespace std;

int even_fibs(const int x)
{
    int sum=0;
    vector<int> fibs;
    fibs.push_back(1);
    fibs.push_back(1);
    while (fibs.back() <= x) {
        int last = fibs.size() - 1;
        fibs.push_back(fibs[last] + fibs[last-1]);
        if (fibs.back()%2 == 0)
            sum += fibs.back();
    }
    return sum;
}

int main()
{
    cout << even_fibs(4000000) << endl;



}
