#include "container_stream.hpp"
#include "fast_digraph.hpp"
#include "expect_exception.hpp"
#include <sstream>

using namespace std;

/**  Runs a sequence of graph algorithm tests on the input graph. 
     Will emit an exception if either (a) any algorithm throws an
     an exception and that algorithm is not named in except_on, 
     (b) an algorithm named in except_on does not throw, or 
     (c) any algorithm's output is different than specified in the 
     'expect' map
     @param[in] g The input graph
     @param[in] source The source vertex for algorithms that need one
     @param[in] destination The destination vertex for algorithms that need one
     @param[in] except_on The algorithms that should throw an exception
     @param[in] expect A map indicating the expected output of each algorithm
     @return void   */
void graph_tests(const digraph &g, digraph::nodename source,
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
              ss << fcn << " visiting " << retval;
              cout << ss.str() << endl;
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
        { return (g.bellman_ford(source).get()); } );
    cout << "Finished test set" << endl;
};



int main( int argc, char *argv[] )
{
    // we need to define some graphs to test on
    //
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


    // now let's start with the actual testing
    //

    set<string> none;

    // passing tests for graph y
    unordered_map<string,string> expected_results_y  {
        {"BFS","BFS visiting {A,B,E,C,D,}"},
        {"DFS","DFS visiting {A,E,B,C,D,}"},
        {"Topsort", "Topsort visiting {A,E,B,C,D,}"},
        {"Dijkstra", "Dijkstra visiting {A,E,D,}"},
        {"Bellman", "Bellman visiting {A<--@0,B<-A@2,C<-B@5,D<-E@2,E<-A@1,}"}
    };
    graph_tests(y,'A','D', none, expected_results_y);

    // passing tests for graph z
    unordered_map<string,string> expected_results_z  {
        {"BFS","BFS visiting {A,B,E,C,D,}"},
        {"DFS","DFS visiting {A,B,C,D,E,}"},
        {"Topsort", "Topsort visiting {A,E,B,C,D,}"},
        {"Dijkstra", "Dijkstra visiting {A,E,D,}"},
        {"Bellman", "Bellman visiting {A<--@0,B<-A@2,C<-B@5,D<-E@2,E<-A@1,}"}
    };
    graph_tests(z,'A','D', none, expected_results_z);

    // passing tests for graph x
    unordered_map<string,string> expected_results_x  {
        {"BFS","BFS visiting {A,B,E,C,D,}"},
        {"DFS","DFS visiting {A,E,B,C,D,}"},
        {"Topsort", "Exception"},
        {"Dijkstra", "Dijkstra visiting {A,E,D,}"},
        {"Bellman", "Bellman visiting {A<--@0,B<-A@2,C<-B@5,D<-E@2,E<-A@1,}"}
    };
    graph_tests(x,'A','D', set<string>{"Topsort"}, expected_results_x);

    // failing tests for graph z
    // the inner expect_exception throws, but it is caught by the out expect_exception
    auto fail1 = [z, expected_results_z] ()
        { graph_tests(z,'A','D', set<string>{"Topsort"}, expected_results_z);
          return 1; };
    auto show_fail = [] ()
        { cout << "Caught Expected Exception..." << endl; return 0; };
    auto show_err = [] (int ignore) { cout << "Failed to Catch Exception!!!"; return 0; };
    expect_exception(fail1, true, show_fail, show_err, 1);

    // passing tests for graph x
    // the inner expect_exception throws, but it is caught by the out expect_exception
    auto fail2 = [x, none, expected_results_x] ()
        { graph_tests(x,'A','D', none, expected_results_x); return 1; };
    expect_exception(fail2, true, show_fail, show_err, 1 );
    return 0;
}
