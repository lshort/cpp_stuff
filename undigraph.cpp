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
#include <tuple>
#include <stack>
#include <limits>
#include <map>
#include <deque>
#include <boost/heap/fibonacci_heap.hpp>
#include <unordered_set>

using namespace std;
using std::placeholders::_1;

/** Instances represent an undirected graph; immutable once constructed    */
class graph {
public:
    typedef char nodename;
    static constexpr nodename no_node = 0;
    struct edge {
        nodename from;
        nodename to;
        int weight;
    };
    graph( const set<nodename> &vertices, const vector<edge> & edges );
    graph mst_prim() const;
private:
    set<nodename> _vertices;
    unordered_map<nodename,vector<pair<nodename,int>>> _adj_lists;
    friend ostream &operator << (ostream &str, const graph &g);
    graph() {}
    void addVertex( nodename s ) { _vertices.insert(s); }
    void addEdge( nodename s, int cost, nodename d )  {
        if (_vertices.end() != _vertices.find(s)
            && _vertices.end() != _vertices.find(d))  {
            _adj_lists[s].push_back( make_pair(d,cost) );
            _adj_lists[d].push_back( make_pair(s,cost) );
        }
    }
};


/**  Constructs an undirected graph from lists of vertices and edges
     @param[in] vertices The <set> of vertices
     @param[in] edges The <vector> of edges     */
graph::graph(const set<nodename> &vertices,
             const vector<edge> & edges ) : _vertices(vertices)
{
    for ( auto &v : vertices )
        _adj_lists[v] = vector<pair<nodename,int>>();
    for ( auto &e : edges )  {
        if ( vertices.end() != vertices.find(e.from) &&
             vertices.end() != vertices.find(e.to) ) {
            _adj_lists[e.from].push_back(make_pair(e.to,e.weight));
            _adj_lists[e.to].push_back(make_pair(e.from,e.weight));
        } else {
            cout << e.from << " " << e.to << endl;
            throw ("Edge (" + to_string(e.from) + "," + to_string(e.to) +
                   ") has nonexistent vertex");
        }
    }
};


ostream &operator << (ostream &str, const graph &g) 
{
    for (auto &v: g._vertices)  {
        str << "Vertex " << v << " has edges:" << endl << "  ";
        vector<pair<graph::nodename,int>> adjacencies = g._adj_lists.at(v);
        for (auto &e : adjacencies)  {
            str << "(" << get<0>(e) << "," << get<1>(e) << "), ";
        }
        str << endl;
    }
    return str;
}




/** represents an entry in the priority queue in Prim's algorithm  */
struct min_heap_data {
    typedef tuple<graph::nodename,int,graph::nodename> my_tuple;
    my_tuple pq_data;
    min_heap_data(tuple<graph::nodename,int,graph::nodename> data) : pq_data(data) {};
    bool operator<(const min_heap_data &b) const
        { return get<1>(pq_data) > get<1>(b.pq_data); };
};


graph graph::mst_prim() const
{
    graph mst;
    unordered_set<nodename> V;
    boost::heap::fibonacci_heap<min_heap_data> PQ;
    typedef boost::heap::fibonacci_heap<min_heap_data>::handle_type handle;
    unordered_map<nodename,handle> handles;
    int inf = numeric_limits<int>::max();  // fib_heap is a min-heap, so use max
    int init = 0;
    for ( auto v: _vertices )  {
        int cost = ( init++ ? inf : 0);
        handles[v] = PQ.emplace(make_tuple(v,cost,' '));
    };
    while (!PQ.empty() && inf != get<1>(PQ.top().pq_data) )  {
        min_heap_data::my_tuple top_data = PQ.top().pq_data;
        nodename v = get<0>(top_data);
        PQ.pop();
        V.insert(v);
        mst.addVertex(v);
        if (' ' != get<2>(top_data)) 
            mst.addEdge(v,get<1>(top_data),get<2>(top_data));
        for ( auto e : _adj_lists.at(v) )    {
            nodename n = get<0>(e);
            if (V.end() == V.find(n))  {
                int edge_cost = get<1>(e);
                handle hand = handles[n];
                min_heap_data::my_tuple *tup = &((*hand).pq_data);
                if (edge_cost < get<1>(*tup))  {
                    get<1>(*tup) = edge_cost;
                    PQ.increase(hand, *hand);
                    get<2>(*tup) = v;
                }
            }
        }
    }
    return mst;
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

graph::nodename verts[] = "ABCDE";

graph::edge eds[] = { {'A','B',2},{'A','E',1}
                        , {'B','C',3},{'B','A',2}
                        , {'C','C',1},{'C','D',2}
                        , {'D','E',0}
                        , {'E','D',1},{'E','B',2} };

graph::edge eds2[] = { {'A','B',2},{'A','E',1}
                         , {'B','C',3}
                         , {'C','D',2}
                         , {'E','D',1},{'E','B',2} };
graph::edge eds3[] = { {'A','B',2},{'A','E',1}
                         , {'B','C',3},{'A','B',3}
                         , {'C','D',2}
                         , {'E','D',1},{'E','B',2} };

int main( int argc, char *argv[] )
{

    graph x(set<graph::nodename>(verts,verts+5),
              vector<graph::edge>(eds,eds+9));
    graph y(set<graph::nodename>(verts,verts+5),
              vector<graph::edge>(eds2,eds2+6));
    graph z(set<graph::nodename>(verts,verts+5),
              vector<graph::edge>(eds3,eds3+7));

    cout << x;
    cout << endl;
    cout << y;
    cout << endl;
    cout << z;
    cout << endl;
    cout << x.mst_prim();
    cout << endl;
    cout << y.mst_prim();
    cout << endl;
    cout << z.mst_prim();
    cout << endl;
    cout << endl << endl;
    return 0;
}
