///   fast_digraph.hpp
///       include file for the graph algorithms in digraph.cpp
///       faster than digraph.hpp, as it uses vectors whenever
///       possible

#ifndef __FAST_DIGRAPH_H
#define __FAST_DIGRAPH_H


#include <functional>
#include <unordered_map>
#include <vector>
#include <utility>
#include <set>
#include <deque>
#include <iostream>
#include <boost/optional/optional.hpp>
#include "expect_exception.hpp"
#include "vector_subrange.hpp"


/** Instances represent a directed graph; immutable once constructed    */
class digraph {
public:
    //  code is not optimized for char
    //! nodename only needs to support ordering ops (=,<, etc.)
    typedef char nodename;
    static const nodename no_node = '-';
    struct edge {
        nodename from;
        nodename to;
        int weight;
    };
    digraph( const std::set<nodename> &vertices,
             const std::vector<edge> & edges );

    //! depth first search 
    std::vector<nodename> dfs( const nodename vertex ) const; 
    //! breadth first search 
    std::vector<nodename> bfs( const nodename vertex ) const;

    //! shortest path from origin to destination
    std::deque<nodename> dijkstra( const nodename origin,
                                   const nodename destination) const;
    //! topological sort
    std::vector<nodename> topsort( ) const;

    //! all-pairs shortest paths
    struct backPointer {
        nodename vertex;
        nodename prev_vertex;
        int cost;
    };

    typedef std::vector<backPointer> bestPaths;
    typedef boost::optional<bestPaths> maybeBestPaths;
    maybeBestPaths bellman_ford(nodename source) const;

private:
    // these are grouped alphebetically by 'from'
    std::vector<edge> _edges;

    // first, last are indices into _edges[]
    static const int noId = -1;

    // sorted alphabetically by 'name'
    std::vector<nodename> _verts;

    // reverse lookup -- finds the index into _verts from the name
    // used also as an index into adjLists and temp arrays; otherwise 
    // would not be worth the extra space (and actually is a bad
    // tradeoff if you are only using it to grab adjLists -- it costs 
    // one extra pointer chase any time you access adjList)
    std::unordered_map<nodename,int> _idxs;

    std::vector<vectorSubrange<edge>> _adjLists;  

    // utilities
    int toIdx(nodename n) const;
    bool relaxAllEdges(std::vector<backPointer> &backPtrs, int infinity) const;
    vectorSubrange<edge> &getAdjList(const nodename vertex);
    const vectorSubrange<edge> &getAdjList(const nodename vertex) const;
};

extern std::ostream &operator<< (std::ostream &ostr,
                                 const digraph::backPointer &p);

#endif  // __FAST_DIGRAPH_H
