#include <string>
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
#include <cmath>
#include <ctgmath>

using namespace std;

auto add = [](auto a, auto b) { return a+b; };
auto sub = [](auto a, auto b) { return a-b; };
auto mul = [](auto a, auto b) { return a*b; };
auto dvd = [](auto a, auto b) { return a/b; };

auto incr = [](auto a) { return a+1; };
auto dbl = [](auto a) { return a*2; };
auto triple = [](auto a) { return a*3; };
//auto show = [](auto a) { cout << a << endl; };

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
    template<typename Lambda1, typename Lambda2>
    auto operator* (Lambda1 l1, Lambda2 l2)
    {
        return [l1, l2](auto x) { return l1(l2(x)); };
    };
}

using compose::operator*;

int main()
{
    vector<int> data {1,2,3,4};
    auto incd = functional::map(incr,data);
    cout << incd[0] << " " << incd[3] << endl;
    auto double_and_inc = incr * dbl;
    auto double_inc_triple = triple * incr * dbl;
    auto hacked = functional::map(double_and_inc,data);
    cout << hacked[0] << " " << hacked[3] << endl;
    int result = double_inc_triple(5);
    cout << result << endl;
    mutates::tmap(double_and_inc,data);
    cout << data[0] << " " << data[3] << endl;
    cout << 3*5 << endl;
    //  *** code below will not compile -- template operator* only takes lambdas
    // auto ident = cos * incd;
    // cout << ident(0.1) << endl;
}
