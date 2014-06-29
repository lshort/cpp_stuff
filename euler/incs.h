#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <istream>
#include <ostream>
#include <utility>
#include <cmath>

using std::ostream;

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
