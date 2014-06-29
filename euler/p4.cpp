#include "incs.h"
#include <cmath>

using namespace std;

bool is_palindrome(unsigned int x)
{
    vector<int> digits;
    while (x>0) {
        digits.push_back(x%10);
        x = x/10;
    }
    for (int i=0; i<digits.size()/2; ++i)  {
        if (digits[i] != digits[digits.size()-i-1])
            return false;
    }
    return true;
}

unsigned int lrg_palin(unsigned int x)
{
    int largest_palin = 0;
    for (int i=x; i>1; --i)
        for (int j=i; j>1; --j)
            if (is_palindrome(i*j))
                if (i*j > largest_palin)
                    largest_palin = i*j;
    return largest_palin;
}

int main()
{
    cout << lrg_palin(999) << endl;



}
