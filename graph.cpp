/// graph.cpp
///   some graph algorithms

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <set>
#include <deque>
#include <iostream>
#include <stack>
#include <limits>
#include <map>
#include <deque>
#include <boost/heap/fibonacci_heap.hpp>

using namespace std;

/** This is the type we will use to represent the name of a graph node */
typedef char nodename;
constexpr nodename no_node = 0;

/** represents an edge in a directed graph  */
struct edge {
    nodename from;
    nodename to;
    int weight;
};


/** Instances represent a directed graph    */
class digraph {
public:
    digraph( const set<nodename> &vertices,
             const vector<edge> & edges );
    void bfs( nodename vertex );
    void dfs( nodename vertex );
    deque<nodename> dijkstra( nodename origin, nodename destination);
private:
    set<nodename> _vertices;
    unordered_map<nodename,vector<pair<nodename,int>>> _adj_lists;
};

/**  Constructs a directed graph from lists of vertices and edges
     @param[in] vertices The <set> of vertices
     @param[in] edges The <vector> of edges     */
digraph::digraph(const set<nodename> &vertices,
                 const vector<edge> & edges ) : _vertices(vertices)
{
    for ( auto &e : edges )  {
        if ( vertices.end() != vertices.find(e.from)) {
            _adj_lists[e.from].push_back(make_pair(e.to,e.weight));
        }
    }
};

/**  Performs a depth-first search of the digraph
     @param[in] vertex The origin vertex
     @return void */
void digraph::dfs( nodename vertex )
{
    set<nodename> V;
    stack<nodename> S;
    S.push(vertex);
    while ( !S.empty() )  {
        nodename top = S.top();
        S.pop();
        if ( V.end() == V.find(top))  {
            V.insert(top);
            cout << "DFS Visiting " << top << endl;
            for ( auto &p : _adj_lists[top] )  {
                S.push(get<0>(p));
            }
        }
    }
};

/**  Performs a breadth-first search of the digraph
     @param[in] vertex The origin vertex
     @return void */
void digraph::bfs( nodename vertex )
{
    set<nodename> V;
    deque<nodename> Q;
    Q.push_back(vertex);
    V.insert(vertex);
    while (!Q.empty()) {
        nodename v = Q.front();
        Q.pop_front();
        cout << "BFS Visiting " << v << endl;
        for ( auto &p : _adj_lists[v] )  {
            nodename to = get<0>(p);
            if ( V.end() == V.find(to)) {
                V.insert(to);
                Q.push_back(to);
            }
        }
    }
};

/** represents an entry in the priority queue in Dijkstra's algorithm  */
struct heap_data {
    pair<nodename,int> pq_data;
    heap_data(pair<nodename,int> data) : pq_data(data) {};
    bool operator<(const heap_data &b) const
        { return get<1>(pq_data) < get<1>(b.pq_data); };
};

/**  Finds the shortest path from origin to destination with Dijkstra's algorithm
     @param[in] origin The origin vertex
     @param[in] destination The destination vertex
     @return Returns an ordered <deque> of the nodenames in the path */
deque<nodename> digraph::dijkstra( nodename origin, nodename destination)
{
    set<nodename> V;
    boost::heap::fibonacci_heap<heap_data> PQ;
    typedef boost::heap::fibonacci_heap<heap_data>::handle_type handle;
    map<nodename,handle> handles;
    map<nodename,nodename> previous;
    int inf = numeric_limits<int>::min();  // fib_heap is a max-heap, so use min
    for ( auto &v : _vertices ) {
        int cost = (v==origin ? 0 : inf);
        handles[v] = PQ.push(make_pair(v, cost));
        previous[v] = no_node;
    }
    while (!PQ.empty() && V.end()==V.find(destination) )  {
        nodename v = get<0>(PQ.top().pq_data);
        int dist = get<1>(PQ.top().pq_data);
        PQ.pop();
        V.insert(v);
        if ( dist == inf )
            break;
        for ( auto &e : _adj_lists[v] )  {
            nodename to = get<0>(e);
            int cost = get<1>(e);
            handle myhand = handles[to];
            if ( dist-cost > get<1>((*myhand).pq_data) ) {
                get<1>((*myhand).pq_data) = dist-cost;
                PQ.increase(myhand, *myhand );
                previous[to] = v;
            }
        }
    }
    deque<nodename> rval;
    if ( previous[destination] != no_node) {
        nodename crnt = destination;
        while (crnt != origin) {
            rval.push_front(crnt);
            crnt = previous[crnt];
        }
        rval.push_front(crnt);
    }
    cout << endl;
    for_each( rval.begin(), rval.end(), [] (nodename v) { cout << v << " ";}  );
    cout << endl << endl;
    return rval;
};



struct edge eds[] = { {'A','B',2},{'A','E',1}
                      , {'B','C',3},{'B','A',2}
                      , {'C','C',1},{'C','D',2}
                      , {'D','E',0}
                      , {'E','D',1},{'E','B',2} };

nodename verts[] = "ABCDE";

int main( int argc, char *argv[] )
{
    digraph x(set<nodename>(verts,verts+5),vector<edge>(eds,eds+9));
    x.bfs('A');
    cout << endl;
    x.dfs('A');
    x.dijkstra('A','D');

    return 0;
}
