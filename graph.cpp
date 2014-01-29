/// graph.cpp
///   some graph algorithms
///

#include <string>

using std::string;

struct node;

struct node_list {
    node *data;
    node_list *next;
};


struct node  {
    node_list* connections;
    string name;
};

class vertex {
    vertex( string name ) : _name(name) {};
    const string & getName() { return _name; };
private:
    string _name;
};

class edge {
    edge( const vertex &from, const vertex &to) : _from(from), _to(to) {};
    const vertex &getFrom() { return _from; };
    const vertex &getTo() { return _to; };
private:
    const vertex &_from, &_to;
};


class digraph {
public:

private:

};
