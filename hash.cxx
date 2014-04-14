#include <string>
#include <vector>
#include <boost/optional/optional.hpp>
#include <utility>

using namespace std;
using boost::optional;

template <typename keyT, typename valueT>
class hashMap {
public:
    using dataT = pair<keyT,valueT>;
    using maybeT = optional<dataT>;
    maybeT notFound = optional<dataT> ();
    hashMap(int size);
    hashMap() : capacity(0), entries(0) {};
    bool insert(keyT k, valueT &v);
    maybeT find(keyT &v);
    bool remove(keyT k);
private:
    vector<maybeT> data;
    int capacity;
    int entries;
    int getCapacity(int size) { return size; };
};


template <typename keyT, typename valueT>
hashMap<keyT,valueT>::hashMap(int size)
{
    capacity = getCapacity(size);
    data.assign((size_t)size, notFound);
};

template <typename keyT, typename valueT>
bool hashMap<keyT,valueT>::insert(keyT k, valueT &v)
{
    int hash = hash(k);
    int offset = 0;
    while ( offset < capacity && data[(hash+offset)%capacity]!=notFound)
        ++offset;
    if (offset >= capacity)
        return false;    // table is full
    data[(hash+offset)%capacity] = maybeT(make_pair(k,v));
    return true;
};

template <typename keyT, typename valueT>
typename hashMap<keyT, valueT>::maybeT hashMap<keyT,valueT>::find(keyT &k)
{
    int hash = hash(k);
    int offset = 0;
    while ( offset < capacity &&
            data[(hash+offset)%capacity]!=notFound &&
            get<0>(data[(hash+offset)%capacity].get()) != k )
        ++offset;

}
