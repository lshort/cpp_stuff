#include <string>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

int stairWays(int n)  {
    vector<int> count(n+1,0);
    count[0] = 1;
    count[1] = 1;
    for (int i=2; i<=n; ++i)  {
        count[i] = count[i-2] + count[i-1];
    }
    return count[n];
}


int stackWays( int n )
{
    vector<vector<int>> a(n+1,vector<int>(n+1,0));
    for (int i=1; i<=n; ++i)
        a[i][i] = 1;
    for (int i=1; i<=n; ++i)
        for (int k=1; k<=i; ++k)
            for (int j=1; j<k; ++j) 
                if (i+j <= n) {
                    a[i+j][j]++;
                }
    int sum = 0;
    for (int i=1; i<=n; ++i)
        sum += a[n][i];
    return sum; 
}


int twosInPowerOf10(int power_of_ten)
{
    if (power_of_ten==0)
        return 0;
    if (power_of_ten==1)
        return 1;
    return (pow(10,power_of_ten)/10)
        + 10 * twosInPowerOf10(power_of_ten-1);
}

int count2s(int n)
{
    if (n <= 11) {
        if (n>1)
            return 1;
        else
            return 0;
    };
    int digits = 1 + floor(log10(n));
    int first_digit = n / (pow(10,digits-1));
    int base = n % ((int)pow(10,digits-1));

    int base_count = first_digit * twosInPowerOf10(digits);
    int left_count;
    if (first_digit > 2)  {
        left_count = pow(10,digits-1);        
    } else if (first_digit < 2)  {
        left_count = 0;
    } else {
        left_count = n % base;
    }
    int remainder_count = count2s(n%base);
    return base_count + left_count + remainder_count;
}



int main() 
{
    for (int k=1; k<2000; ++k)
        cout << count2s(k) << endl;

    return 0;
}
