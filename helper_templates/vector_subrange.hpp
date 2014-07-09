#include <vector>


template <typename T> class vector_subrange;
template <typename T>
typename std::vector<T>::iterator begin(const vector_subrange<T> &r);
template <typename T>
typename std::vector<T>::iterator end(const vector_subrange<T> &r);


template <typename T>
class vector_subrange {
public:
    vector_subrange( const std::vector<T> &data, int first, int last )
        : _data(data), _first(first), _last(last)  { };
    friend typename std::vector<T>::iterator begin <> (const vector_subrange &range)
        { return range._data.begin() + range._first; };
    friend typename std::vector<T>::iterator end <> (const vector_subrange &range)
        { return range._data.begin() + range._first; };
private:
    const std::vector<T> &_data;
    int _first;
    int _last;
};

