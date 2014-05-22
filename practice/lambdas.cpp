#include <string>
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
#include <cmath>
#include <ctgmath>

using std::cout;
using std::endl;
using std::vector;

auto incr = [](auto a) { return a+1; };
auto dbl = [](auto a) { return a*2; };
auto triple = [](auto a) { return a*3; };

namespace functional {
    auto map = [] (const auto &fcn, const auto &vec )
    {
        auto retval = vec;
        retval.clear();
        for (const auto &elem : vec)
            retval.push_back(fcn(elem));
        return retval;
    };
}

namespace mutates {
    auto tmap = [] (const auto &fcn, auto &vec )
    {
        for (auto &elem : vec)
            elem = fcn(elem);
    };
}

namespace compose {
    // works
    template<typename Lambda1, typename Lambda2>
    auto operator* (Lambda1 l1, Lambda2 l2)
    {
        return [l1, l2](auto x) { return l1(l2(x)); };
    };

    // doesn't work
    template <typename return_type, typename intermediate, typename base>
    auto operator* (return_type (*f1)(intermediate),
                    intermediate (*f2)(base))
    {
        return [f1, f2] (auto x) { return f1(f2(x)); };
    };
}

using compose::operator*;

int add1(int x)  { return x+1; }
int sub1(int x)  { return x-1; }

int main()
{
    vector<int> data {1,2,3,4};
    vector<int> incd = functional::map(incr,data);
    cout << incd[0] << " " << incd[3] << endl;
    auto double_and_incr = incr * dbl;
    auto double_incr_triple = triple * incr * dbl;

    vector<int> map_results = functional::map(double_and_incr,data);
    cout << map_results[0] << " " << map_results[3] << endl;
    cout << double_incr_triple(5) << endl;

    mutates::tmap(double_and_incr,data);
    cout << data[0] << " " << data[3] << endl;
    cout << 3*5 << endl;   // make sure we haven't hidden plain old *

    //  *** code below will not compile 
    //      compose::operator* (function ptrs) does not resolve to the template
    //      on line A (even if I remove op* from the namespace for lookup)
    //      
    int (*fp1)(int) = &add1;
    int (*fp2)(int) = &sub1;
    int (*ident)(int) = fp1 * fp2;     // A
    cout << ident(1) << endl;
}
