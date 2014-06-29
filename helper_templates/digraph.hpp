///   digraph.hpp
///       include file for the graph algorithms in digraph.cpp


#include <functional>
#include <unordered_map>
#include <vector>
#include <utility>
#include <set>
#include <deque>
#include <boost/optional/optional.hpp>
#include "expect_exception.hpp"
#include "container_stream.hpp"


/** Instances represent a directed graph; immutable once constructed    */
class digraph {
public:
    typedef char nodename;
    static const nodename no_node = '-';
    struct edge {
        nodename from;
        nodename to;
        int weight;
    };
    digraph( const std::set<nodename> &vertices,
             const std::vector<edge> & edges );

    std::vector<nodename> dfs( const nodename vertex ) const;
    std::vector<nodename> bfs( const nodename vertex ) const;

    std::deque<nodename> dijkstra( const nodename origin,
                                   const nodename destination) const;
    std::vector<nodename> topsort( ) const;

    typedef std::pair<std::unordered_map<nodename,nodename>,
                      std::unordered_map<nodename,int>> bestPaths;
    typedef boost::optional<bestPaths> maybeBestPaths;
    maybeBestPaths bellman_ford(nodename source) const;

private:
    std::set<nodename> _vertices;
    std::unordered_map<nodename,std::vector<std::pair<nodename,int>>> _adj_lists;
    bool relaxAllEdges(std::unordered_map<nodename,nodename> &backPtr,
                       std::unordered_map<nodename,int> &cost, int infinity) const;
};

