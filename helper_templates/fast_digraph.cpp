/// fast_digraph.cpp
///   some graph algorithms

#include <boost/heap/fibonacci_heap.hpp>
#include <limits>
#include <stack>
#include <map>
#include "fast_digraph.hpp"

using namespace std;
using std::placeholders::_1;


const digraph::nodename digraph::no_node;

bool valid_edge( const digraph::edge &e,
                 const set<digraph::nodename> &verts ) 
{
    return (verts.end() != verts.find(e.from) &&
            verts.end() != verts.find(e.to) );
};


/**  Constructs a directed graph from lists of vertices and edges
     @param[in] vertices The <set> of vertices
     @param[in] edges The <vector> of edges     */
digraph::digraph(const set<nodename> &vertices,
                 const vector<edge> & edges ) : _edges(edges)
{
    for ( auto &e : _edges )  {
        if ( !valid_edge(e, vertices) ) {
            throw ("Edge (" + to_string(e.from) + "," + to_string(e.to) +
                   ") has nonexistent vertex");
        }
    }
    sort(_edges.begin(), _edges.end(), [] (const edge &a, const edge &b)
         { return a.from < b.from; } );

    _verts.reserve(vertices.size());
    _adjLists.reserve(vertices.size());
    int count=0;
    int idx_count=0;
    // don't have to worry about dup vertices 
    // it's a set so its sorted and has no dups
    for ( const auto & v : vertices)  {
        _verts.emplace_back(v);
        _idxs[v] = idx_count;
        int first = count;
        int last = first;
        while (count<_edges.size() && v == _edges[count].from)  
            last = count++;
        _adjLists.emplace_back(vectorSubrange<edge>(_edges,first,last));
        ++idx_count;
    }
};


/**  Returns the adjacency list for the specified vertex.  
     @param[in] vertex The vertex
     @return A reference to that vertex's adjacency list */
vectorSubrange<digraph::edge> &digraph::getAdjList(const nodename vertex)
{
    return *(const_cast<vectorSubrange<digraph::edge>*>
             (&_adjLists.at(toIdx(vertex))));
}


/**  Returns the adjacency list for the specified vertex.  
     @param[in] vertex The vertex
     @return A reference to that vertex's adjacency list */
const vectorSubrange<digraph::edge> &digraph::getAdjList(const nodename vertex) const
{
    return *(const_cast<vectorSubrange<digraph::edge>*>
             (&_adjLists.at(toIdx(vertex))));
}


/**  Returns the vector index for the specified vertex.
     @param[in] vertex The vertex
     @return The index in the vertex vector (and other vectors)  */
int digraph::toIdx(nodename n) const
{
    return _idxs.at(n);
}



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
            for ( auto &p : getAdjList(top) )  {
                S.push(p.to);
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
        for ( const auto &p : getAdjList(v) ) 
            if ( V.end() == V.find(p.to)) {
                V.insert(p.to);
                Q.push_back(p.to);
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
    vector<handle> handles;   // indexed by _idxs
    vector<nodename> previous; // indexed by _idxs
    //    map<nodename,handle> handles;
    //    map<nodename,nodename> previous;
    int inf = numeric_limits<int>::min();  // fib_heap is a max-heap, so use min
    for ( auto v : _verts) {
        int cost = (v==origin ? 0 : inf);
        int idx = toIdx(v);
        handles[idx] = PQ.push(make_pair(v, cost));
        previous[idx] = no_node;
    }
    while (!PQ.empty() && V.end()==V.find(destination) )  {
        nodename v = get<0>(PQ.top().pq_data);
        int dist = get<1>(PQ.top().pq_data);
        PQ.pop();
        V.insert(v);
        if ( dist == inf )
            break;
        for ( auto &e : getAdjList(v) ) {
            nodename to = e.to;
            int cost = e.weight;
            handle myhand = handles[toIdx(to)];
            if ( dist - cost > get<1>((*myhand).pq_data) ) {
                get<1>((*myhand).pq_data) = dist - cost;
                PQ.increase(myhand, *myhand );
                previous[toIdx(to)] = v;
            }
        }
    }
    deque<nodename> rval;
    if ( previous[toIdx(destination)] != no_node) {
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
bool digraph::relaxAllEdges(std::vector<backPointer> &backPtrs,
                            int infinity) const
{
    bool retval = false;
    for (auto v : _verts)  {
        int c = backPtrs[toIdx(v)].cost;
        if (c != infinity ) {
            for ( const auto &e : getAdjList(v) ) {
                int to = toIdx(e.to);
                if ( c + e.weight < backPtrs[to].cost) {
                    backPtrs[to].cost = c + e.weight;
                    backPtrs[to].prev_vertex = v;
                    retval = true;
    }   }   }   }
    return retval;
}

/**  Finds shortest path, even with negative edge weights
     @param[in] source The source vertex
     @return A vector that holds the path  */
digraph::maybeBestPaths  digraph::bellman_ford(digraph::nodename source ) const
{
    int infinity = numeric_limits<int>::max();
    backPointer dummy {no_node, no_node, infinity };
    vector<backPointer> backPtrs(_verts.size(), dummy );
    for ( auto v : _verts )  {
        backPtrs[toIdx(v)].vertex = v;
    }
    backPtrs[toIdx(source)].cost = 0;
    for (int i=0; i<_verts.size(); ++i ) {
        bool ignore = relaxAllEdges(backPtrs, infinity);
    }
    if (relaxAllEdges(backPtrs, infinity))
        return boost::optional<bestPaths>();  // negative weight cycle
    return boost::optional<bestPaths>(backPtrs);
}


/**  Performs a topological sort of the graph
     @return Returns a <vector> of the ordered vertex names */
vector<digraph::nodename> digraph::topsort( ) const
{
    set<nodename> no_pred(_verts.begin(),_verts.end());
    vector<nodename> sorted;
    vector<multiset<nodename>> reverse_adj_list;
    for ( const auto &e : _edges ) {
        reverse_adj_list[toIdx(e.to)].insert(e.from);
        no_pred.erase(no_pred.find(e.to)); // inefficient on dense graph
    }
    while ( !no_pred.empty()) {
        nodename v = *no_pred.begin();
        sorted.push_back(v);
        no_pred.erase(no_pred.find(v));
        for ( auto &e : getAdjList(v) ) {
            int to = toIdx(e.to);
            reverse_adj_list[to].erase(reverse_adj_list[to].find(v));
            if ( reverse_adj_list[to].empty() )  {
                no_pred.insert(to);
            }
        }
    }
    for ( const auto &a : reverse_adj_list )
        if ( !a.empty() )
            throw "Found a cycle in the graph";
    return sorted;
}

