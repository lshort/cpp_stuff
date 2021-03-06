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
#include <boost/optional/optional.hpp>
#include <boost/proto/core.hpp>


using namespace std;
using std::placeholders::_1;

/** Instances represent a directed graph; immutable once constructed    */
class digraph {
public:
    typedef char nodename;
    static const nodename no_node = 0;
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
    typedef pair<unordered_map<nodename,nodename>,unordered_map<nodename,int>> bestPaths;
    typedef boost::optional<bestPaths> maybeBestPaths;
    maybeBestPaths bellman_ford(nodename source);

    vector<nodename> topsort( ) const;
    vector<digraph> scc() const;
private:
    set<nodename> _vertices;
    unordered_map<nodename,vector<pair<nodename,int>>> _adj_lists;
    bool relaxAllEdges(unordered_map<nodename,nodename> &backPtr,
                       unordered_map<nodename,int> &cost, int infinity);
};
const digraph::nodename digraph::no_node;

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



/**  Try to relax all edges in the graph; destructively modify the map arguments
     @param[in] backPtr The list of back pointers
     @param[in] costs The cost list
     @return True if any edge was relaxed  */
bool digraph::relaxAllEdges(unordered_map<nodename,nodename> &backPtr,
                            unordered_map<nodename,int> &cost, int infinity)
{
    bool retval = false;
    for (auto v : _vertices)  {
        int c = cost[v];
        if ( c != infinity )
            for ( auto &e : _adj_lists[v])  {
                if ( c + get<0>(e) < cost[get<1>(e)]) {
                    cost[get<1>(e)] = c + get<0>(e);
                    backPtr[get<1>(e)] = v;
                    retval = true;
                }
            }
    }
    return retval;
}


/**  Finds shortest path, even with negative edge weights
     @param[in] source The source vertex
     @return A vector that holds the path  */
digraph::maybeBestPaths  digraph::bellman_ford(digraph::nodename source )
{
    unordered_map<nodename, nodename> backPtr;
    unordered_map<nodename, int> cost;
    int infinity = numeric_limits<int>::max();
    for ( auto v : _vertices )  {
        backPtr.insert( make_pair(v, no_node) );
        cost.emplace( make_pair(v, infinity) );
    }
    cost[source] = 0;
    for (int i=0; i<_vertices.size(); ++i ) {
        bool ignore = relaxAllEdges(backPtr, cost, infinity);
    }
    if (relaxAllEdges(backPtr, cost, infinity))
        return boost::optional<bestPaths>();
    return boost::optional<bestPaths>(make_pair(backPtr,cost));
}


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


vector<digraph> digraph::scc() const 
{
    vector<digraph> retval;
    stack<nodename> kosaraju_stack;
    while (kosaraju_stack.size() < _vertices.size())  {
        stack<nodename> dfs_stack;
        nodename n = *_vertices.begin();
        dfs_stack.push(n);
        while (!dfs_stack.empty())  {
            nodename v = dfs_stack.top();
            dfs_stack.pop();
            if (kosaraju_stack.end() == kosaraju_stack.find(v) ) {
                kosaraju_stack.push_back(v);
                for ( auto e : _adj_list[v] ) {
                    dfs_stack.push_back(get<0>(e));
                }
            }
            
        }
    }
    digraph transpose = buildTranspose();
    while (!kosajaru_stack.empty())  {
        nodename n = kosajaru_stack.top();
        stack<nodename> dfs_stack();
        dfs_stack.push(n);
        set<nodename> visited;
        while(!dfs_stack.empty()) {
            nodename v = dfs_stack.top();
            if (dfs_stack.end()==dfs_stack.find(v)) {
                visited.insert(v);
                for (auto e : transpose._adj_list[v])
                    dfs_stack.push_back(get<0>(e));
            }
        }
        retval.push_back(buildSubGraph(visited));
        removeFromStack(kosajaru_stack, visited);
    }
    return retval;
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


/**  Prints out any associative container of any class, 
     provided the contained class has an operator <<
     @param[in] ostr The output stream
     @param[in] xs The container
     @return Returns the output stream      */
template< typename T, typename T2,
          template<typename El,
                   typename Alloc=std::allocator<El> > class AssocContainer >
ostream &operator<< (ostream &ostr, const AssocContainer<T, T2> &xs)
{
    ostr << "{ ";
    for ( auto &x : xs )
        ostr << "(" << get<0>(x) << ":" << get<1>(x) << ")" << ",";
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

/*
template<typename ExeLambda, typename AllowExcLambda>
auto expect_exception( ExeLambda exe_lambda, AllowExcLambda except_lambda )
                       //                       OnExcLambda on_exc )
{
    try {
        auto x = exe_lambda();
        if (except_lambda())
            throw "failed to catch expected exception";
        return x;
    }
    catch (...) {
        if (!except_lambda())
            throw "caught unexpect exception";
        auto y;
        return y;
    }
}
*/

template<typename ExeLambda, typename OnExcLambda, typename NoExcLambda>
auto expect_exception( ExeLambda exe_lambda, bool expect_p,
                       OnExcLambda exc_lambda, NoExcLambda no_exc_lambda)
{
    try {
        auto x = exe_lambda();
        if (expect_p)
            throw "failed to catch expected exception";
        return no_exc_lambda(x);
    }
    catch (...) {
        if (!expect_p)
            throw "caught unexpect exception";
        return exc_lambda();
    }
}

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

    auto print_xs = [] (const char* title, auto xs)
        {   cout << title << " visiting " << xs << endl;  };

    auto some_tests = [&]  (digraph &g, digraph::nodename source,
                            digraph::nodename destination,
                            const set<string> &  except_on )
        {
            auto exc_test = [except_on] (string name)
                { return except_on.end()!=except_on.find(name); };
            auto exc_return = [] ()
                { cout << "Caught Expected Exception" << endl; };
            auto tst = [exc_test, exc_return] (string fcn, auto a)
            {
                auto normal_return = [fcn] (auto retval)
                    { cout << fcn << " visiting " << retval << endl; };
                expect_exception(a, exc_test(fcn), exc_return, normal_return );
            };
            tst("BFS", [g, source] () { return g.bfs(source); } );
            tst("DFS", [g, source] () { return g.dfs(source); } );
            tst("Topsort", [g] () { return g.topsort(); } );
            tst("Dijkstra", [g, source, destination] () { return g.dijkstra(source, destination); } );
            //auto m = g.bellman_ford(source);
            //bellman_visit(get<0>(*m));
        };

    set<string> none;
    some_tests(y,'A','D', none);
    some_tests(z,'A','D', none);
    some_tests(x,'A','D', set<string>{"Topsort"});

    return 0;
}
