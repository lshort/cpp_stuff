/// graph.cpp
///   some graph algorithms

#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct node;

struct edge {
    char *from;
    char *to;
    int weight;
};

struct digraph {
    char **vertices;
    struct edge **adj_list;
    int  *edge_count;
    int  vertex_count;
};

// caller is responsible for freeing vertices and adj_list
// return value is the size of the vertex list
void build_a(struct digraph *d) {
    d->vertex_count = 5;
    d->vertices = malloc( d->vertex_count * sizeof(char*));
    d->adj_list = malloc( d->vertex_count * sizeof(struct edge*));
    d->edge_count = malloc( d->vertex_count * sizeof(int));
    d->vertices[0] = "A";
    d->adj_list[0] = (struct edge []){ {"A","B",2},{"A","E",1} };
    d->edge_count[0] = 2;
    d->vertices[1] = "B";
    d->adj_list[1] = (struct edge []){ {"B","C",3},{"B","A",2} };
    d->edge_count[1] = 2;
    d->vertices[2] = "C";
    d->adj_list[2] = (struct edge []){ {"C","C",1},{"C","D",2} };
    d->edge_count[2] = 2;
    d->vertices[3] = "D";
    d->adj_list[3] = (struct edge []){ {"D","E",0} };
    d->edge_count[3] = 1;
    d->vertices[4] = "E";
    d->adj_list[4] = (struct edge []){ {"E","D",0},{"E","B",2} };
    d->edge_count[4] = 2;
};


void cleanup( struct digraph d ) {
    free(d.vertices);
    free(d.adj_list);
};

int index_in( char* name, char** list, int list_size )  {
    for ( int t=0; t<list_size; ++t )  {
        if (!strcmp(name,list[t]))
            return t;
    }
    return -1;
};

void breadth_first( struct digraph *d, char* vertex ) {
    int vertex_size = d->vertex_count;
    char **V = malloc( vertex_size * sizeof(char*));
    int v_size = 0;
    char **Q = malloc( vertex_size * sizeof(char*));
    int q_first = 0;
    int q_last = 0;
    Q[0] = vertex;
    while ( q_first >= q_last ) {
        V[v_size++] = Q[q_first];
        printf( "visiting %s\n", V[v_size-1]);
        int idx = index_in( Q[q_first], d->vertices, vertex_size);
        ++q_first;
        for ( int j=0; j<d->edge_count[idx]; ++j )  {
            char *to = d->adj_list[idx][j].to;
            if (-1 == index_in(to,V,v_size)) {
                Q[++q_last] = to;
            }
        }
    }
    free (Q);
    free (V);
};


int main( int argc, char *argv[] ) {
    struct digraph x;
    build_a(&x);

    breadth_first(&x,"A");

    cleanup(x);
    return 0;
}
