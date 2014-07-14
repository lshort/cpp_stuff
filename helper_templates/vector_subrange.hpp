///  vector_subrange.h
///     defines the vectorSubrange template, which allows you
///     to define a subrange of a vector that you can iterate
///     over, do iterator arithmetic on, etc. -- anything you
///     can do to an iterator, you can do with a subrange, because
///     a subrange is just a wrapper around a pair of actual
///     iterators

#ifndef __VECTOR_SUBRANGE_H
#define __VECTOR_SUBRANGE_H


#include <vector>


///  Allows you to iterate over a subrange of a vector.
///  Handy for range-based for.  This version is non-const.
///  Const version is constVectorSubrange.
template <typename T>
class vectorSubrange {
public:
    vectorSubrange( std::vector<T> &data, int first, int last )
        : _data(data), _first(first), _last(last)
    {  if (_last < _first) _last = _first - 1;  };

    typedef typename std::vector<T>::iterator mutIt;
    friend mutIt begin (vectorSubrange &range)
        {  return range._data.begin() + range._first; };
    friend mutIt end (vectorSubrange &range)
        { return range._data.begin() + range._last + 1; };
    friend const mutIt begin (const vectorSubrange &range)
        {  return range._data.begin() + range._first; };
    friend const mutIt end (const vectorSubrange &range)
        { return range._data.begin() + range._last + 1; };
private:
    std::vector<T> &_data;
    int _first;
    int _last;
};

///  Allows you to iterate over a subrange of a vector.
///  Handy for range-based for.  This version is const.
///  Mutable version is vectorSubrange.
template <typename T>
class constVectorSubrange {
public:
    constVectorSubrange( const std::vector<T> &data, int first, int last )
        : _data(data), _first(first), _last(last) 
        {  if (_last < _first) _last = _first - 1;  };

    typedef typename std::vector<T>::const_iterator constIt;
    friend constIt begin (constVectorSubrange &range)
        {  return range._data.begin() + range._first; };
    friend constIt end (constVectorSubrange &range)
        { return range._data.begin() + range._last + 1; };
private:
    const std::vector<T> &_data;
    int _first;
    int _last;
};

#endif  //  __VECTOR_SUBRANGE_H

