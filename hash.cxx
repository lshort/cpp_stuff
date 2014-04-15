#include <string>
#include <vector>
#include <boost/optional/optional.hpp>
#include <utility>

using namespace std;
using boost::optional;


int hashFcn(const int x)
{
    return x;
};


template <typename keyT, typename valueT>
class hashMap {
public:
    using dataT = pair<keyT,valueT>;
    using maybeT = optional<dataT>;

    hashMap(int size) : entries(0) {
        capacity = getCapacity(size);
        data.assign((size_t)size, notFound);
    };
    hashMap() : capacity(0), entries(0) {};

    bool insert(const keyT &k, const valueT &v) {
        growIfFull();
        int hash = hashFcn(k);
        int offset = 0;
        while ( offset < capacity && entryExists(hash+offset) &&
                !entryHasKey(hash+offset,k)  )
            ++offset;
        if (offset >= capacity)
            return false;    // table is full
        data[(hash+offset)%capacity] = maybeT(make_pair(k,v));
        return true;
    };
    maybeT find(const keyT &k) const {
        int hash = hashFcn(k);
        int offset = 0;
        while ( offset < capacity && entryExists(hash+offset) &&
                !entryHasKey(hash+offset,k) )
            ++offset;
        if ( offset >= capacity || !entryExists(hash+offset) ||
             !entryHasKey(hash+offset,k) )
            return notFound;
        else
            return data[(hash+offset)%capacity];
    };
    bool isFound(const maybeT &m) { return notFound != m; };
    bool remove(const keyT &k);
private:
    maybeT notFound = optional<dataT> ();
    vector<maybeT> data;
    int capacity;
    int entries;
    int getCapacity(int size) { return size; };
    bool growIfFull() {
        if (entries >= (9 * capacity)/10)  {
            capacity = 4093;
            data.assign(capacity,notFound);
            // clobbers old values
        }
    };
    bool entryExists(const int index) const  {
        return (notFound != data[index%capacity]);
    };
    bool entryHasKey(int index, const keyT k) const {
        return (k==get<0>(data[index%capacity].get()));
    }
};


hashMap<int,string> h;
void test_found(int x) {
    cout << to_string(x) << ": ";
    auto foo = h.find(x);
    if (h.isFound(foo) )  {
        cout << "found it! ";
        cout << get<1>(foo.get()) << endl;
    }  else
        cout << "not found." << endl;
};


int main(int argc, char *argv[])
{
    h.insert(3,"three");
    test_found(3);
    test_found(4);
    test_found(5);
    test_found(4096);

    h.insert(4096,"forty ninety-size");
    h.insert(4,"four");
    test_found(3);
    test_found(4);
    test_found(5);
    test_found(4096);


}
