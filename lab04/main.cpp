//Получение множества двудольных компонент неориентированного графа

#include <iostream>
#include "graph.h"


int main() {
    //vector<vector<bool>> matrix
    //{
    //    {0, 1, 1, 1, 1},
    //    {1, 0, 1, 1, 1},
    //    {1, 1, 0, 1, 0},
    //    {1, 1, 1, 0, 0},
    //    {1, 1, 0, 0, 0}
    //};
    //Graph graph(matrix);
    Graph graph(8);
    graph.GenerateGraph();
    //graph.PrintAdjacencyMatrix();
    graph.PrintBipartiteComponents(1);

    return 0;
}
