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
#include <numeric_limits>
#include <boost/heap/priority_queue>

using namespace std;

typedef char nodename;

struct edge {
    nodename from;
    nodename to;
    int weight;
};


class digraph {
public:
    digraph( const set<nodename> &vertices,
             const vector<edge> & edges );
    void bfs( nodename vertex );
    void dfs( nodename vertex );
    vector<nodename> dijkstra( nodename origin, nodename destination);
private:
    set<nodename> _vertices;
    unordered_map<nodename,vector<pair<nodename,int>>> _adj_lists;
};

digraph::digraph(const set<nodename> &vertices,
                 const vector<edge> & edges ) : _vertices(vertices)
{
    for ( auto &e : edges )  {
        if ( vertices.end() != vertices.find(e.from)) {
            _adj_lists[e.from].push_back(make_pair(e.to,e.weight));
        }
    }
};

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

vector<nodename> digraph::dijkstra( nodename origin, nodename destination)
{
    set<nodename> V;
    typedef pair<nodename,int> pq_entry;
    deque<pq_entry> PQ;
    for ( auto &v : _vertices )
        PQ.insert(make_pair(v,? v==origin : 0 : numeric_limits<int>::max));
    while (!PQ.empty()) {
        sort( PQ.begin(), PQ.end()
              [] (const pq_entry &a, const pq_entry &b)
              { return get<1>(a) < get<1>(b);  }
              );
        nodename v = get<0>(PQ.front());
        int dist = get<1>(PQ.front());
        PQ.pop_front();
        V.add(v);
        for ( auto &e : _adj_lists[v] )  {
            nodename to = get<0>(e);
            cost = get<1>(e);
            crnt_cost =
if ( cost+dist <  )}

    }


};



struct edge eds[] = { {'A','B',2},{'A','E',1}
                           , {'B','C',3},{'B','A',2}
                           , {'C','C',1},{'C','D',2}
                           , {'D','E',0}
                           , {'E','D',0},{'E','B',2} };

char verts[] = "ABCDE";

int main( int argc, char *argv[] )
{
    digraph x(set<nodename>(verts,verts+5),vector<edge>(eds,eds+9));
    x.bfs('A');
    cout << endl;
    x.dfs('A');

    return 0;
}
