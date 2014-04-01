/// graph.cpp
///   some graph algorithms

#include <string>
#include <vector>
#include <limits>
#include <tuple>
#include <cstdlib>
#include <iostream>

using namespace std;

int doof(vector<int> food)
{
    return 1;
}

template <typename T>
struct SkipNode {
    static const T sentinel = numeric_limits<T>::min();
    vector<SkipNode*> next_nodes;
    T x;
};

template <typename T>
class SkipList {
public:
    SkipList();
    void insert(const T x);
    void remove(const T x);
    bool find(const T x) const;
    ostream &operator << (ostream &strm);
private:
    SkipNode<T> *head;
    int getRandomHeight();
    tuple<vector<SkipNode<T>*>,int> findPreviousNodes( const T x );
    int findHeight(const T x) const;
};

template <typename T>
SkipList<T>::SkipList()
{
    head = new SkipNode<T>;
    head->next_nodes.push_back(NULL);
    head->x = SkipNode<T>::sentinel;
};

template <typename T>
tuple<vector<SkipNode<T>*>,int> SkipList<T>::findPreviousNodes( const T x )
{
    int foundlevel = -1;
    int level = head->next_nodes.size() - 1;
    vector<SkipNode<T>*> retval(level+1, NULL);
    SkipNode<T> *previous = head;
    while (level >= 0)  {
        while (NULL!=previous->next_nodes[level] &&
               previous->next_nodes[level]->x < x) {
            previous = previous->next_nodes[level];
        }
        retval[level] = previous;
        if (NULL!=previous->next_nodes[level] &&
            x == previous->next_nodes[level]->x && -1 == foundlevel)
            foundlevel = level;
        --level;
    }
    return make_tuple(move(retval),foundlevel);
}

template <typename T>
void SkipList<T>::insert(const T x)
{
    SkipNode<T> *new_node = new SkipNode<T>;
    const int height = getRandomHeight();
    new_node->next_nodes = vector<SkipNode<T>*>(height,NULL);
    while (height > head->next_nodes.size() ) {
        head->next_nodes.push_back(NULL);
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

template <typename T>
int SkipList<T>::findHeight(const T x) const
{
    return get<1>(findPreviousNodes(x));
};

template <typename T>
bool SkipList<T>::find(const T x) const
{
    return (-1 != findHeight(x));
};

template <typename T>
void SkipList<T>::remove(const T x)
{
    SkipNode<T> *new_node = new SkipNode<T>;
    const int height = getRandomHeight();
    auto tuple = findPreviousNodes(x);
    int level = get<1>(tuple);
    auto nodes = get<0>(tuple);
    while (level>0) {
        auto my_node = nodes[level]->next_node[level];
        auto next = my_node->next_nodes[level];
        nodes[level]->next_node[level] = next;
        delete my_node;
    }
};

template<typename T>
int SkipList<T>::getRandomHeight()
{
    int height = 1;
    int max = head->next_nodes.size() + 2;
    while ( height < max && 1==rand()%2 )
        ++height;
    return height;
};

template<typename T>
ostream &SkipList<T>::operator << (ostream &strm)
{
    for ( int level=0; level<head->next_nodes.size(); ++level)   {
        strm << "Level " << to_string(level) << ": ";
        SkipNode<T> *node = head->next_nodes[level];
        while ( NULL != node ) {
            strm << to_string(node.x) << " ";
            node = node->next_nodes[level];
        }
        strm << endl;
    }
};



int main( int argc, char *argv[])
{
    SkipList<int> sl = SkipList<int>();
    sl.insert(1);
    sl.insert(10);
    sl.insert(100);
    sl.insert(51);
    sl.insert(21);
    sl.insert(13);
    sl.insert(-1);
    sl.insert(-111);
    sl.insert(17);


}
