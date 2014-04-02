/// graph.cpp
///   some graph algorithms

#include <functional>
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
using std::placeholders::_1;

/** Instances represent a directed graph; immutable once constructed    */
class digraph {
public:
    typedef char nodename;
    static constexpr nodename no_node = 0;
    struct edge {
        nodename from;
        nodename to;
        int weight;
    };
    digraph( const set<nodename> &vertices,
             const vector<edge> & edges );
    vector<nodename> dfs( const nodename vertex ) const;
    vector<nodename> bfs( const nodename vertex ) const;
    deque<nodename> dijkstra( const nodename origin, const nodename destination) const;
    vector<nodename> topsort( ) const;
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
    for ( auto &v : vertices )
        _adj_lists[v] = vector<pair<nodename,int>>();
    for ( auto &e : edges )  {
        if ( vertices.end() != vertices.find(e.from) &&
             vertices.end() != vertices.find(e.to) ) {
            _adj_lists[e.from].push_back(make_pair(e.to,e.weight));
        } else {
            cout << e.from << " " << e.to << endl;
            throw ("Edge (" + to_string(e.from) + "," + to_string(e.to) +
                   ") has nonexistent vertex");
        }
    }
};

/**  Performs a depth-first search of the digraph
     @param[in] vertex The origin vertex
     @return void */
vector<digraph::nodename> digraph::dfs( const nodename vertex ) const
{
    vector<nodename> retval;
    set<nodename> V;
    stack<nodename> S;
    S.push(vertex);
    while ( !S.empty() )  {
        nodename top = S.top();
        S.pop();
        if ( V.end() == V.find(top))  {
            V.insert(top);
            retval.push_back(top);
            for ( auto &p : _adj_lists.at(top) )  {
                S.push(get<0>(p));
            }
        }
    }
    return retval;
};

/**  Performs a breadth-first search of the digraph
     @param[in] vertex The origin vertex
     @return void */
vector<digraph::nodename> digraph::bfs( const nodename vertex ) const
{
    vector<nodename> retval;
    set<nodename> V;
    deque<nodename> Q;
    Q.push_back(vertex);
    V.insert(vertex);
    while (!Q.empty()) {
        nodename v = Q.front();
        Q.pop_front();
        retval.push_back(v);
        for ( auto &p : _adj_lists.at(v) )  {
            nodename to = get<0>(p);
            if ( V.end() == V.find(to)) {
                V.insert(to);
                Q.push_back(to);
            }
        }
    }
    return retval;
};

/** represents an entry in the priority queue in Dijkstra's algorithm  */
struct heap_data {
    pair<digraph::nodename,int> pq_data;
    heap_data(pair<digraph::nodename,int> data) : pq_data(data) {};
    bool operator<(const heap_data &b) const
        { return get<1>(pq_data) < get<1>(b.pq_data); };
};

/**  Finds the shortest path from origin to destination with Dijkstra's algorithm
     @param[in] origin The origin vertex
     @param[in] destination The destination vertex
     @return Returns an ordered <deque> of the nodenames in the path */
deque<digraph::nodename> digraph::dijkstra( const nodename origin,
                                            const nodename destination) const
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
        for ( auto &e : _adj_lists.at(v) )  {
            nodename to = get<0>(e);
            int cost = get<1>(e);
            handle myhand = handles[to];
            if ( dist - cost > get<1>((*myhand).pq_data) ) {
                get<1>((*myhand).pq_data) = dist - cost;
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
    return rval;
};


/**  Performs a topological sort of the graph
     @return Returns a <vector> of the ordered vertex names */
vector<digraph::nodename> digraph::topsort( ) const
{
    set<nodename> no_pred;
    vector<nodename> sorted;
    unordered_map<nodename,multiset<nodename>> reverse_adj_list;
    for ( auto a : _adj_lists) {
        for ( auto e : a.second ) {
            nodename v = get<0>(e);
            reverse_adj_list[v].insert(a.first);
        }
    }
    for ( auto v : _vertices ) {
        if ( 0 == reverse_adj_list[v].size() )
            no_pred.insert(v);
    }
    while ( !no_pred.empty()) {
        nodename v = *no_pred.begin();
        sorted.push_back(v);
        no_pred.erase(no_pred.find(v));
        auto edges = _adj_lists.at(v);
        for ( auto &e : edges ) {
            nodename to = get<0>(e);
            reverse_adj_list[to].erase(reverse_adj_list[to].find(v));
            if ( reverse_adj_list[to].empty() )  {
                no_pred.insert(to);
            }
        }
    }
    bool found = false;
    for ( auto a : reverse_adj_list )
        if ( !(a.second).empty() )
            found = true;
    if (found)
        throw "Found a cycle in the graph";
    else
        return sorted;
}


/**  Prints out any container of any class, provided the class has operator <<
     @param[in] ostr The output stream
     @param[in] xs The container
     @return Returns the output stream      */
template< typename T,
          template<typename El,
                   typename Alloc=std::allocator<El> > class Container >
ostream &operator<< (ostream &ostr, const Container<T> &xs)
{
    ostr << "{ ";
    for ( auto &x : xs )
        ostr << x << ",";
    ostr << " }";
    return ostr;
}


/**  A glorified operator ().  Should be a lambda, but you can't bind a lambda
 */
struct print_visit {
    print_visit() {};
    /** Prints out the visited nodes in order
    @param[in] title The title of the print listing
    @param[in] xs The container to print
    @return void */
    template< typename T,
              template<typename El,
                       typename Alloc=std::allocator<El> > class Container >
    void operator () ( const char* title, const Container<T> &xs) const
    {
        cout << title << " visiting ";
        cout << xs;
        cout << endl;
    };
};

digraph::nodename verts[] = "ABCDE";

digraph::edge eds[] = { {'A','B',2},{'A','E',1}
                        , {'B','C',3},{'B','A',2}
                        , {'C','C',1},{'C','D',2}
                        , {'D','E',0}
                        , {'E','D',1},{'E','B',2} };

digraph::edge eds2[] = { {'A','B',2},{'A','E',1}
                         , {'B','C',3}
                         , {'C','D',2}
                         , {'E','D',1},{'E','B',2} };
digraph::edge eds3[] = { {'A','B',2},{'A','E',1}
                         , {'B','C',3},{'A','B',3}
                         , {'C','D',2}
                         , {'E','D',1},{'E','B',2} };

int main( int argc, char *argv[] )
{

    digraph x(set<digraph::nodename>(verts,verts+5),
              vector<digraph::edge>(eds,eds+9));
    digraph y(set<digraph::nodename>(verts,verts+5),
              vector<digraph::edge>(eds2,eds2+6));
    digraph z(set<digraph::nodename>(verts,verts+5),
              vector<digraph::edge>(eds3,eds3+7));

    print_visit pv;

    auto bfs_visit = std::bind( pv,
                                "BFS", _1);
    auto dfs_visit = std::bind( pv,
                                "DFS", _1);
    auto topsort_visit = std::bind( pv,
                                    "Topsort", _1);
    auto dijkstra_visit = std::bind( pv,
                                    "Dijkstra", _1);

    auto some_tests = [&] (digraph &g, digraph::nodename source,
                           digraph::nodename destination)
        { cout << endl << "Testing a graph" << endl;
          bfs_visit(g.bfs(source));
          dfs_visit(g.dfs(source));
          topsort_visit(g.topsort());
          dijkstra_visit(g.dijkstra(source,destination)); };

    some_tests(y,'A','D');
    some_tests(z,'A','D');
    some_tests(x,'A','D');

    return 0;
}
