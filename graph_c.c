/// graph_c.c
///   some graph algorithms

#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>


typedef char nodename;

struct edge {
    nodename from;
    nodename to;
    int weight;
};

struct digraph_c {
    nodename *vertices;
    struct edge **adj_list;
    int  *edge_count;
    int  vertex_count;
};


struct edge edges_1a[] = { {'A','B',2},{'A','E',1} };
struct edge edges_1b[] = { {'B','C',3},{'B','A',2} };
struct edge edges_1c[] = { {'C','C',1},{'C','D',2} };
struct edge edges_1d[] = { {'D','E',0} };
struct edge edges_1e[] = { {'E','D',0},{'E','B',2} };

// caller is responsible for freeing vertices and adj_list
// return value is the size of the vertex list
void build_a(struct digraph_c *d) {
    d->vertex_count = 5;
    d->vertices = calloc((size_t) d->vertex_count, sizeof(nodename));
    d->adj_list = calloc((size_t) d->vertex_count, sizeof(struct edge*));
    d->edge_count = calloc( (size_t)d->vertex_count, sizeof(int));
    d->vertices[0] = 'A';
    d->adj_list[0] = edges_1a;
    d->edge_count[0] = 2;
    d->vertices[1] = 'B';
    d->adj_list[1] = edges_1b;
    d->edge_count[1] = 2;
    d->vertices[2] = 'C';
    d->adj_list[2] = edges_1c;
    d->edge_count[2] = 2;
    d->vertices[3] = 'D';
    d->adj_list[3] = edges_1d;
    d->edge_count[3] = 1;
    d->vertices[4] = 'E';
    d->adj_list[4] = edges_1e;
    d->edge_count[4] = 2;
};


void cleanup( struct digraph_c d ) {
    free(d.vertices);
    free(d.adj_list);
    d.vertices = NULL;
    d.adj_list = NULL;
};

int index_in( nodename name, nodename* list, int list_size )  {
    for ( int t=0; t<list_size; ++t )  {
        if ( name==list[t])
        //        if (!strcmp(name,list[t]))
            return t;
    }
    return -1;
};

void breadth_first( struct digraph_c *d, nodename vertex ) {
    int vertex_size = d->vertex_count;
    nodename *V = calloc( (size_t)vertex_size, sizeof(nodename));
    V[0] = vertex;
    int v_size = 1;
    nodename *Q = calloc( (size_t)vertex_size, sizeof(nodename));
    int q_first = 0;
    int q_last = 0;
    Q[0] = vertex;
    while ( q_first <= q_last ) {
        nodename v = Q[q_first];
        ++q_first;
        printf( "BFS visiting %c\n", v); fflush(stdout);
        int idx = index_in( v, d->vertices, vertex_size);
        for ( int j=0; j<d->edge_count[idx]; ++j )  {
            nodename to = d->adj_list[idx][j].to;
            if (-1 == index_in(to,V,v_size)) {
                V[v_size++] = to;
                Q[++q_last] = to;
            }
        }
    }
    free (Q);
    free (V);
};


void depth_first( struct digraph_c *d, nodename vertex ) {
    int vertex_size = d->vertex_count;
    nodename *V = calloc( (size_t)vertex_size, sizeof(nodename));
    int v_size = 0;
    nodename *S = calloc( (size_t)vertex_size, sizeof(nodename));
    int s_top = 0;
    S[0] = vertex;
    while ( s_top >= 0 ) {
        nodename v = S[s_top];
        --s_top;
        if (-1 == index_in(v,V,v_size)) {
            V[v_size++] = v;
            printf( "DFS visiting %c\n", v); fflush(stdout);
            int idx = index_in( v, d->vertices, vertex_size);
            for ( int j=0; j<d->edge_count[idx]; ++j )  {
                nodename to = d->adj_list[idx][j].to;
                S[++s_top] = to;
            }
        }
    }
    free (S);
    free (V);
};


int main( int argc, char *argv[] ) {
    struct digraph_c x;
    build_a(&x);

    depth_first(&x,'A');
    printf("\n");
    breadth_first(&x,'A');

    cleanup(x);
    return 0;
}
