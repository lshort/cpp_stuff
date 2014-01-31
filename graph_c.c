/// graph.cpp
///   some graph algorithms

#include <malloc.h>
#include <stdio.h>

struct node;

struct edge {
    char *from;
    char *to;
    int weight;
};

struct digraph {
    char **vertices;
    struct edge **adj_list;
};

int main( int argc, char *argv[] ) {
    struct digraph rval;
    int size = 5;
    rval.vertices = malloc( size * sizeof(char*));
    rval.adj_list = malloc( size * sizeof(struct edge*));
    rval.vertices[0] = "A";
    rval.adj_list[0] = (struct edge []){ {"A","B",2},{"A","E",1} };
    rval.vertices[1] = "B";
    rval.adj_list[1] = (struct edge []){ {"B","C",3},{"B","A",2} };
    rval.vertices[2] = "C";
    rval.vertices[3] = "D";
    rval.vertices[4] = "E";
    printf("%s", rval.vertices[0]);
    free(rval.vertices);
    free(rval.adj_list);
    return 0;
}
