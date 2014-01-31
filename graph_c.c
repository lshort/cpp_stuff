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

// caller is responsible for freeing vertices and adj_list
// return value is the size of the vertex list
int build(struct digraph* d) {
    int size = 5;
    d->vertices = malloc( size * sizeof(char*));
    d->adj_list = malloc( size * sizeof(struct edge*));
    d->vertices[0] = "A";
    d->adj_list[0] = (struct edge []){ {"A","B",2},{"A","E",1} };
    d->vertices[1] = "B";
    d->adj_list[1] = (struct edge []){ {"B","C",3},{"B","A",2} };
    d->vertices[2] = "C";
    d->adj_list[2] = (struct edge []){ {"C","C",1},{"C","D",2} };
    d->vertices[3] = "D";
    d->adj_list[3] = (struct edge []){ {"D","E",0} };
    d->vertices[4] = "E";
    d->adj_list[4] = (struct edge []){ {"E","D",0},{"E","B",2} };
    return size;
};




void cleanup( struct digraph gr ) {
    free(gr.vertices);
    free(gr.adj_list);
};



int main( int argc, char *argv[] ) {
    struct digraph x;
    int c=build(&x);

    cleanup(x);
    return 0;
}
