/// graph.cpp
///   some graph algorithms

#include <string>
#include <vector>
#include <limits>
#include <tuple>

using namespace std;

int doof(vector<int> food)
{
    return 1;
}

template <typename T>
struct SkipNode {
    const T sentinel = numeric_limits<T>::minimum();
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
private:
    vector<SkipNode<T>*> head_list;
    int getRandomHeight();
    tuple<vector<SkipNode<T>*>,int> findPreviousNodes( const T x );
    int findHeight(const T x) const;
};

template <typename T>
SkipList<T>::SkipList()
{
    SkipNode<T> *header = new SkipNode<T>;
    header->next_nodes->push_back(NULL);
    header->x = SkipNode<T>::sentinel;
};

template <typename T>
tuple<vector<SkipNode<T>*>,int> SkipList<T>::findPreviousNodes( const T x )
{
    int foundlevel = -1;
    int level = head_list.size() - 1;
    vector<SkipNode<T>*> retval(level+1, NULL);
    SkipNode<T> *previous = head_list;
    while (level >= 0)  {
        while (NULL!=previous->next_nodes[level] &&
               previous->next_nodes[level]->x < x)     {
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
    while (height > head_list.size()) {
        head_list.push_back(NULL);
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
