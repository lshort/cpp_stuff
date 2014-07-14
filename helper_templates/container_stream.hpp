///  container_stream.hpp
///      some templates for streaming containers

#ifndef __CONTAINER_STREAM_H
#define __CONTAINER_STREAM_H

#include <iostream>
#include <utility>
#include <unordered_map>

/**  Streams out any template container of any class, 
     provided the class has operator <<
     @param[in] ostr The output stream
     @param[in] xs The container
     @return Returns the output stream      */
template< typename T,
          template<typename El,
                   typename Alloc=std::allocator<El> > class Container >
std::ostream &operator<< (std::ostream &ostr, const Container<T> &xs)
{
    ostr << "{";
    for ( auto &x : xs )
        ostr << x << ",";
    ostr << "}";
    return ostr;
}


/**  Streams out any associative container of any class, 
     provided the contained class has an operator <<
     @param[in] ostr The output stream
     @param[in] xs The container
     @return Returns the output stream      */
template< typename T, typename T2,
     template<typename El,
              typename Alloc=std::allocator<El> > class AssocContainer >
std::ostream &operator<< (std::ostream &ostr,
                          const AssocContainer<T, T2> &xs)
{
    ostr << "{";
    for ( auto &x : xs )
        ostr << "(" << std::get<0>(x) << ":" << std::get<1>(x) << ")" << ",";
    ostr << "}";
    return ostr;
}


/**  Streams out an unordered_map, provided the mapped type and
     the value type both have an operator <<
     @param[in] ostr The output stream
     @param[in] xs The container
     @return Returns the output stream      */
template< typename T, typename T2>
std::ostream &operator<< (std::ostream &ostr,
                          const std::unordered_map<T, T2> &xs)
{
    ostr << "{";
    for ( auto &x : xs )
        ostr << "(" << std::get<0>(x) << ":" << std::get<1>(x) << ")" << ",";
    ostr << "}";
    return ostr;
}


#endif   //  __CONTAINER_STREAM_H
