#include "digraph.cpp"

digraph::nodename verts[] = "ABCDE";

digraph::edge edges[] = { {'A','B',2},{'A','E',1}
                        , {'B','C',3},{'B','A',2}
                        , {'C','C',1},{'C','D',2}
                        , {'D','E',0}
                        , {'E','D',1},{'E','B',2} };

digraph::edge edges2[] = { {'A','B',2},{'A','E',1}
                         , {'B','C',3}
                         , {'C','D',2}
                         , {'E','D',1},{'E','B',2} };
digraph::edge edges3[] = { {'A','B',2},{'A','E',1}
                         , {'B','C',3},{'A','B',3}
                         , {'C','D',2}
                         , {'E','D',1},{'E','B',2} };

digraph x(set<digraph::nodename>(verts,verts+5),
          vector<digraph::edge>(edges,edges+9));
digraph y(set<digraph::nodename>(verts,verts+5),
          vector<digraph::edge>(edges2,edges2+6));
digraph z(set<digraph::nodename>(verts,verts+5),
          vector<digraph::edge>(edges3,edges3+7));


void graph_tests(digraph &g, digraph::nodename source,
                digraph::nodename destination, const set<string> & except_on )
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

    cout << endl << "=====> Beginning a test set" << endl;
    tst("BFS", [g, source] () { return g.bfs(source); } );
    tst("DFS", [g, source] () { return g.dfs(source); } );
    tst("Topsort", [g] () { return g.topsort(); } );
    tst("Dijkstra", [g, source, destination] ()
        { return g.dijkstra(source, destination); } );
    tst("Bellman", [g, source] ()
        { return get<0>(g.bellman_ford(source).get()); } );
};



int main( int argc, char *argv[] )
{

    set<string> none;
    graph_tests(y,'A','D', none);
    graph_tests(z,'A','D', none);
    graph_tests(x,'A','D', set<string>{"Topsort"});

    try {
        graph_tests(z,'A','D', set<string>{"Topsort"});
    }
    catch (...) {
        cout << "Caught Exception A" << endl;
    };
    
    try {
        graph_tests(x,'A','D', none);
    }
    catch (...) {
        cout << "Caught Exception B" << endl;
    };
    return 0;
}
