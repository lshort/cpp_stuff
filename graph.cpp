/// graph.cpp
///   some graph algorithms

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
    edge( const vertex &from, const vertex &to, int wgt)
        : _from(from), _to(to), _weigh(wgt) {};
    const vertex &getFrom() { return _from; };
    const vertex &getTo() { return _to; };
    int getWgt { return _weight; };
private:
    const vertex &_from, &_to;
    int _weight;
};


class digraph {
public:
    digraph( const vector<string> &vertices,
             const vector<tuple<string,string,int>> & edges );
private:
    vector<string> _vertices;
    map<string,vector<pair<string,int>> _adj_lists;
};
