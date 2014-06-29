#include <vector>
#include <type_traits>
#include <limits>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <memory>

using namespace std;



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



template <typename T>
T extractDigit(T num, int digit)
{
    int divisor = pow(10,digit-1);
    return (num/divisor)%10;
}

template <typename T>
unique_ptr<vector<T>> radix_sort( const vector<T> &data )
{
    static_assert(is_integral<T>::value, "radix_sort requires integral types");
    int mx = numeric_limits<T>::min();
    for (auto d : data)
        mx = max(mx,d);
    unique_ptr<vector<T>> tmp = unique_ptr<vector<T>>(new vector<T>(data));
    int digits = ceil(log10((float)mx));
    for (int digit=1; digit <= digits; ++digit) {
        array<vector<T>,10> buckets;
        for (auto t : *tmp)
            buckets[extractDigit(t,digit)].push_back(t);
        tmp->clear();
        for (int k=0; k<10; ++k)
            tmp->insert(tmp->end(),buckets[k].begin(),buckets[k].end());
    }
    return move(tmp);
}


unsigned int add( unsigned int a, unsigned int b )
{
    unsigned int sum = 0;
    unsigned int carry = 0;
    for (int digit=0; digit<32; ++digit) {
        unsigned int digita = (a << (31-digit)) >> 31;
        unsigned int digitb = (b << (31-digit)) >> 31;
        unsigned int next = (digita ^ digitb) ^ carry;
        carry = (digita & digitb) | (digita & carry) | (digitb & carry) ;
        sum |= next << digit;
    }
    return sum;
}



int main(int argc, char *argv[])
{
    vector<int> data;

    for (int i=1; i<argc; ++i)
        data.push_back(atoi(argv[i]));
    cout << *radix_sort(data) << endl;

    cout << to_string(add(atoi(argv[1]), atoi(argv[2]))) << endl;
}
