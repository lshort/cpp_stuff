/// graph.cpp
///   some graph algorithms

#include <string>
#include <vector>
#include <limits>
#include <tuple>
#include <cstdlib>
#include <iostream>

using namespace std;

///  helper struct
template <typename T>
struct SkipNode {
    static const T head_sentinel = numeric_limits<T>::min();
    static const T tail_sentinel = numeric_limits<T>::max();
    vector<SkipNode*> next_nodes;
    T x;
};


//  forward declarations needed by template goofiness
template<typename T> class SkipList;
template<typename T>ostream &operator << (ostream &strm, SkipList<T> &ths);

/// instances represent a skip list of value type T
///
template <typename T>
class SkipList {
public:
    SkipList();
    void insert(const T x);
    void remove(const T x);
    bool find(const T x) const;
    friend ostream &operator << <> (ostream &strm, SkipList<T> &ths);
private:
    SkipNode<T> *head;
    SkipNode<T> *tail;
    int getRandomHeight();
    tuple<vector<SkipNode<T>*>,int> findPreviousNodes( const T x );
    int findHeight(const T x) const;
};

/** Constructs the skip list, with head and tail heights of 1 */
template <typename T>
SkipList<T>::SkipList()
{
    tail = new SkipNode<T>;
    tail->next_nodes.push_back(NULL);
    tail->x = SkipNode<T>::tail_sentinel;
    head = new SkipNode<T>;
    head->next_nodes.push_back(tail);
    head->x = SkipNode<T>::head_sentinel;
};

/**  Finds the nodes at each level that are directly before the
     value in question, and the height of the value if it already
     is present
     @param[in] vertex The origin vertex
     @return A tuple of the previous nodes and the height of the
     value if present (if not present, the value is -1) */
template <typename T>
tuple<vector<SkipNode<T>*>,int> SkipList<T>::findPreviousNodes( const T x )
{
    int foundlevel = -1;
    int level = head->next_nodes.size() - 1;
    vector<SkipNode<T>*> retval(level+1, NULL);
    SkipNode<T> *previous = head;
    while (level >= 0)  {
        while (previous->next_nodes[level]->x < x) {
            previous = previous->next_nodes[level];
        }
        retval[level] = previous;
        if (x == previous->next_nodes[level]->x && -1 == foundlevel)
            foundlevel = level;
        --level;
    }
    return make_tuple(move(retval),foundlevel);
}

/**  Inserts the value x in the skip list
     @param[in] x The value to insert
     @return Void  */
template <typename T>
void SkipList<T>::insert(const T x)
{
    SkipNode<T> *new_node = new SkipNode<T>;
    new_node->x = x;
    const int height = getRandomHeight();
    new_node->next_nodes = vector<SkipNode<T>*>(height,NULL);
    while (height > head->next_nodes.size() ) {
        tail->next_nodes.push_back(NULL);
        head->next_nodes.push_back(tail);
    }
    auto prevNodes = get<0>(findPreviousNodes(x));
    int level = 0;
    for ( auto &node : prevNodes) {
        new_node->next_nodes[level] = node->next_nodes[level];
        node->next_nodes[level] = new_node;
        ++level;
        if (level>=height)
            break;
    }
};

/** Finds the height of the value in the skip list
    @param[in] x The value to find
    @return The height of the value, or -1 if it is not preset */
template <typename T>
int SkipList<T>::findHeight(const T x) const
{
    return get<1>(findPreviousNodes(x));
};

/** Finds the value in the skip list
    @param[in] x The value to find
    @return True if the value is present, False if it is not */
template <typename T>
bool SkipList<T>::find(const T x) const
{
    return (-1 != findHeight(x));
};

/** Removes the value from the skip list
    @param[in] x The value to remove
    @return Void */
template <typename T>
void SkipList<T>::remove(const T x)
{
    auto tuple = findPreviousNodes(x);
    int level = get<1>(tuple);
    if (level >= 0) {
        auto nodes = get<0>(tuple);
        auto my_node = nodes[level]->next_nodes[level];
        while (level >= 0) {
            auto next = my_node->next_nodes[level];
            nodes[level]->next_nodes[level] = next;
            --level;
        }
        delete my_node;
    }
};

/** Generates a random height for a new skip list entry
    @return The height for the new entry */
template<typename T>
int SkipList<T>::getRandomHeight()
{
    int height = 1;
    int max = head->next_nodes.size() + 2;
    while ( height < max && 1==rand()%2 )
        ++height;
    return height;
};

/** Outputs the skip list to the stream
    @param[in] strm The stream to output to
    @param[in] ths The skip list to output
    @return The output stream  */
template<typename T>
ostream &operator << (ostream &strm, SkipList<T> &ths)
{
    for ( int level=0; level<ths.head->next_nodes.size(); ++level)   {
        strm << "Level  " << to_string(level) << ": ";
        SkipNode<T> *node = ths.head->next_nodes[level];
        while ( NULL != node ) {
            strm << to_string(node->x) << " ";
            node = node->next_nodes[level];
        }
        strm << endl;
    }
};



int main( int argc, char *argv[])
{
    SkipList<int> sl = SkipList<int>();
    sl.insert(1);
    sl.insert(-1);
    sl.insert(100);
    sl.insert(51);
    sl.insert(10);
    sl.insert(21);
    sl.insert(13);
    sl.insert(-111);
    sl.insert(17);
    cout << sl;
    sl.remove(51);
    cout << sl;
    sl.remove(13);
    cout << sl;

}
