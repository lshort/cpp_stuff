/// graph.cpp
///   some graph algorithms

#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

struct node;

struct edge {
    node *from;
    node *to;
    int *weight;
}

struct edge_list {
    edge *data;
    node_list *next;
};

struct node  {
    node_list *connections;
    char *name;
};

struct digraph {
    char *vertices[];
    edge_list *adj_lists;
};
