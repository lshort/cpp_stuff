#include "digraph.cpp"
#include <sstream>


void graph_tests(const digraph::digraph &g, digraph::nodename source,
                 digraph::nodename destination,
                 const set<string> & except_on,
                 unordered_map<string,string> expect )
{
    auto exc_return = [] ()
        { cout << "Caught Expected Exception" << endl;
          return string("Exception"); };
    auto tst = [exc_return, except_on, expect] (string fcn, auto a)
        {
            auto exc_test = [except_on, fcn] ()
                { return except_on.end()!=except_on.find(fcn); };
            auto normal_return = [fcn] (auto retval)
            { stringstream ss;
              ss << fcn << " visiting " << retval << endl;
              cout << ss.str();
              return ss.str();
            };
            expect_exception_l(a, exc_test, exc_return,
                               normal_return, expect.at(fcn) );
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

    digraph::digraph x(set<digraph::nodename>(verts,verts+5),
                       vector<digraph::edge>(edges,edges+9));
    digraph::digraph y(set<digraph::nodename>(verts,verts+5),
                       vector<digraph::edge>(edges2,edges2+6));
    digraph::digraph z(set<digraph::nodename>(verts,verts+5),
                       vector<digraph::edge>(edges3,edges3+7));


    set<string> none;
    graph_tests(y,'A','D', none, 
                unordered_map<string,string> {
                    {"BFS","BFS visiting {A,B,E,C,D,}"},
                    {"DFS","DFS visiting {A,E,B,C,D,}"},
                    {"Topsort", "Topsort visiting {A,E,B,C,D,}"},
                    {"Dijkstra", "Dijkstra visiting {A,E,D,}"},
                    {"Bellman", "Bellman visiting {(E:A),(D:E),(C:B),(B:A),(A:-),}"}
                } );
    /*
    graph_tests(z,'A','D', none);
    graph_tests(x,'A','D', set<string>{"Topsort"});

    auto fail1 = [z] () { graph_tests(z,'A','D', set<string>{"Topsort"});
                          return 1; };
    auto show_fail = [] () { cout << "Caught Expected Exception..." << endl; };
    auto show_err = [] (int ignore) { cout << "Failed to Catch Exception!!!"; };
    expect_exception(fail1, true, show_fail, show_err );

    auto fail2 = [x, none] () { graph_tests(x,'A','D', none); return 1; };
    expect_exception(fail2, true, show_fail, show_err );
    */
    return 0;
}
