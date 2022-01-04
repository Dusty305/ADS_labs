//Получение множества двудольных компонент неориентированного графа

#include <iostream>
#include "graph.h"


int main() {
    Graph graph;

    graph.GenerateGraph(20);
    graph.PrintAdjacencyMatrix();
    cout << "Graph is " << (graph.isBipartite() ? "bipartite\n" : "not bipartite\n");

    return 0;
}
