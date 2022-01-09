#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <time.h>
#include <cstdio>
#include <vector>
#include <stack>
#include <map>

using namespace std;


class Graph 
{
private:
    typedef int VertexId;
    vector<vector<bool>> matrix;
public:

    Graph(size_t size = 0) 
    { 
        matrix = vector<vector<bool>>(size, vector<bool>(size, false));
        srand(time(0)); 
    }

    //Graph(vector<Vertex> vec) : matrix(vec) { srand(time(0)); }

    Graph(const vector<vector<bool>>& adjMatrix)
    {
        Graph(adjMatrix.size());
        for (int i = 0; i < adjMatrix.size(); ++i)
            for (int j = i + 1; j < adjMatrix.size(); ++j)
                if (adjMatrix[i][j])
                    AddEdge(i, j);
    }

    // ---------------------------------
    // Vertex related methods
    // ---------------------------------

    //const vector<Vertex>& GetVerteceis() const { return matrix; }

    const int GetVerteciesNumber() const { return matrix.size(); }

    // TODO: Сделать inline?
    inline bool IsValidVertexId(VertexId id) const
    {
        return 0 <= id && id < matrix.size();
    }

    //// TODO: Нужна ли проверка на правильность id?
    //Vertex& GetVertex(VertexId id)
    //{
    //    if (IsValidVertexId(id))
    //        return matrix[id];
    //    else
    //        throw "Invalid vertex id!";
    //}

    //// TODO: У гетера должен быть такой тип?
    //const Vertex& GetVertex(VertexId id) const
    //{
    //    if (IsValidVertexId(id))
    //        return matrix[id];
    //    else
    //        throw "Invalid vertex id!";
    //}

    // ---------------------------------
    // Edge related methods
    // ---------------------------------

    bool HasEdge(const VertexId start, const VertexId finish) const
    {
        if (!IsValidVertexId(start) or !IsValidVertexId(finish))
            throw "Invalid vertex id";
        return matrix[start][finish];
    }

    // TODO: Исправить на VertexId?
    void AddEdge(const VertexId start, const VertexId finish)
    {
        matrix[start][finish] = true;
        matrix[finish][start] = true;
    }

    void DeleteEdge(VertexId start, VertexId finish)
    {
        matrix[start][finish] = false;
        matrix[finish][start] = false;
    }

    // ---------------------------------
    // Graph related methods
    // ---------------------------------

    void PrintAdjacencyMatrix() const 
    {
        for (vector<bool> row : matrix)
        {
            for (bool b : row)
                cout << b << " ";
            cout << "\n";
        }
    }

    void GenerateGraph()
    {
        const int n = matrix.size();
        const int edges_number = rand() % (n * (n - 1) / 2) + 1;

        for (int k = 0; k < edges_number; ++k)
        {
            VertexId i = rand() % n,
                     j = rand() % n;
            if (i != j and !HasEdge(i, j))
                AddEdge(i, j);
            else
                --k;
        }
    }   

    Graph GenerateSpanningTree(const Graph& graph);
    void PrintBipartiteComponents(const int n);
    bool isBipartite() const;
};

// returns graph with missing edges
// rval return?
Graph Graph::GenerateSpanningTree(const Graph& graph) {
    const int nOfV = graph.matrix.size();

    stack<VertexId> dfs_stack;
    vector<bool> used(nOfV, false);
    Graph result_graph(nOfV);

    dfs_stack.push(0);
    used[0] = true;
    while (!dfs_stack.empty())
    {
        VertexId id = dfs_stack.top(); dfs_stack.pop();
        
        for (int i = 0; i < graph.matrix[id].size(); ++i)
        {
            if(graph.matrix[id][i])
                if (!used[i])
                {
                    AddEdge(id, i);
                    dfs_stack.push(i);
                    used[i] = true;
                }
                else
                    result_graph.AddEdge(id, i);
        }
        // (Оптимизировать)
        // Если компонент связности несколько, то ищем следующую компоненту для добавления ребер
        if (dfs_stack.empty())
            for (int i = 0; i < used.size(); ++i)
                if (!used[i])
                {
                    dfs_stack.push(i);
                    used[i] = true;
                    break;
                }
    }

    //cout << "Spanning tree\n";
    //PrintAdjacencyMatrix();
    return result_graph;
}

void Graph::PrintBipartiteComponents(const int n = 1) 
{
    if (isBipartite()) 
    {
        cout << "Bipartite component #" << n << "\n";
        PrintAdjacencyMatrix();
        cout << "\n";
        return;
    }
    //cout << "Graph\n";
    //PrintAdjacencyMatrix();
    //Строим остовное дерево
    Graph graph(matrix.size());
    Graph new_graph(matrix.size());
    new_graph = graph.GenerateSpanningTree(*this);
    //Добавляем по очереди не вошедшие ребра
    for(int i = 0; i < matrix.size(); ++i)
        for(int j = 0; j < matrix.size(); ++j)
        {
            if (!new_graph.matrix[i][j])
                continue;
            graph.AddEdge(i, j);
            if (!graph.isBipartite())
                graph.DeleteEdge(i, j);
            else
                new_graph.DeleteEdge(i, j);
        }
    cout << "Bipartite component #" << n << "\n";
    graph.PrintAdjacencyMatrix();
    cout << "------------------------------------------\n";
    new_graph.PrintBipartiteComponents(n + 1);
}

bool Graph::isBipartite() const 
{
    vector<int> color(matrix.size(), -1);
    stack<int> dfs_stack;

    dfs_stack.push(0);
    color[0] = 0;
    while (!dfs_stack.empty())
    {
        VertexId id = dfs_stack.top(); dfs_stack.pop();
        for (int i = 0; i < matrix.size(); ++i)
        {
            if(matrix[id][i])
                if (color[i] == -1)
                {
                    dfs_stack.push(i);
                    color[i] = !color[id];
                }
                else if (color[i] == color[id])
                    return false;
        }
        // (Оптимизировать)
        // Если компонент связности несколько, то ищем следующую компоненту для добавления ребер
        if (dfs_stack.empty())
            for (int i = 0; i < color.size(); ++i)
                if (color[i] == -1)
                {
                    dfs_stack.push(i);
                    color[i] = 0;
                    break;
                }
    }
    return true;
}
