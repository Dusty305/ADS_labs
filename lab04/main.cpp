#include <iostream>
#include "graph.h"


int main() {
    Graph graph(8);
    graph.GenerateGraph();
    cout << "Graph's adjecency matrix\n";
    graph.PrintAdjacencyMatrix();
    graph.PrintBipartiteComponents();

    return 0;
}
