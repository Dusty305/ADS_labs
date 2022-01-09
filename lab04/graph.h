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

typedef int VertexId;

struct Edge
{
    VertexId start;
    VertexId finish;
    Edge(int strt = -1, int fnsh = -1) : start(strt), finish(fnsh) { }
    inline bool operator==(const Edge& edge) { return start == edge.start && finish == edge.finish; }
};

struct Vertex 
{
    vector<Edge> edges;
    Vertex(int size = 0) { edges.reserve(size); }
};

class Graph 
{
private:
    // TODO: точно ли эффективно хранить в виде вектора вершин с ребрами?
    vector<Vertex> vertecies;
public:

    Graph(size_t size = 0) 
    { 
        vertecies = vector<Vertex>(size, Vertex(size - 1));
        srand(time(0)); 
    }

    Graph(vector<Vertex> vec) : vertecies(vec) { srand(time(0)); }

    Graph(const vector<vector<bool>>& adjMatrix)
    {
        vertecies = vector<Vertex>(adjMatrix.size(), Vertex(adjMatrix.size() - 1));
        for (int i = 0; i < adjMatrix.size(); ++i)
            for (int j = i + 1; j < adjMatrix.size(); ++j)
                if (adjMatrix[i][j])
                    AddEdge(i, j);
    }

    // ---------------------------------
    // Vertex related methods
    // ---------------------------------

    const vector<Vertex>& GetVerteceis() const { return vertecies; }

    const int GetVerteciesNumber() const { return vertecies.size(); }

    // TODO: Сделать inline?
    inline bool IsValidVertexId(VertexId id) const
    {
        return 0 <= id && id < vertecies.size();
    }

    // TODO: Нужна ли проверка на правильность id?
    Vertex& GetVertex(VertexId id)
    {
        if (IsValidVertexId(id))
            return vertecies[id];
        else
            throw "Invalid vertex id!";
    }

    // TODO: У гетера должен быть такой тип?
    const Vertex& GetVertex(VertexId id) const
    {
        if (IsValidVertexId(id))
            return vertecies[id];
        else
            throw "Invalid vertex id!";
    }

    // ---------------------------------
    // Edge related methods
    // ---------------------------------

    bool HasEdge(const VertexId start, const VertexId finish) const
    {
        if (!IsValidVertexId(start) or !IsValidVertexId(finish))
            throw "Invalid vertex id";
        return find_if(vertecies[start].edges.begin(), vertecies[start].edges.end(), 
            [&](const Edge& edge) { return finish == edge.finish; }) 
            != vertecies[start].edges.end();
    }

    // TODO: Исправить на VertexId?
    void AddEdge(const VertexId start, const VertexId finish)
    {
        vertecies[start].edges.push_back(Edge(start, finish));
        vertecies[finish].edges.push_back(Edge(finish, start));
    }

    void DeleteLastEdge(VertexId id1)
    {
        const VertexId id2 = vertecies[id1].edges.back().finish;
        vertecies[id1].edges.pop_back();
        vertecies[id2].edges.pop_back();
    }

    // ---------------------------------
    // Graph related methods
    // ---------------------------------

    void PrintAdjacencyMatrix() const 
    {
        vector<vector<bool>> matrix = vector<vector<bool>>(vertecies.size(), vector<bool>(vertecies.size(), false));
        for (Vertex v : vertecies)
            for (Edge e : v.edges)
                matrix[e.start][e.finish] = true;
        for (vector<bool> row : matrix)
        {
            for (bool b : row)
                cout << b << " ";
            cout << "\n";
        }
    }

    void GenerateGraph()
    {
        const int n = vertecies.size();
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

    vector<Edge> GenerateSpanningTree(const Graph& graph);
    void PrintBipartiteComponents(const int n);
    bool isBipartite() const;
};

vector<Edge> Graph::GenerateSpanningTree(const Graph& graph) {
    const int nOfV = graph.GetVerteciesNumber();

    stack<VertexId> dfs_stack;
    vector<bool> used(nOfV, false);
    vector<Edge> missingEdges;
    missingEdges.reserve(nOfV * (nOfV - 1) / 2);

    dfs_stack.push(0);
    used[0] = true;
    while (!dfs_stack.empty())
    {
        VertexId id = dfs_stack.top(); dfs_stack.pop();
        const Vertex& v = graph.GetVertex(id);

        for (Edge e : v.edges)
        {
            if (!used[e.finish])
            {
                AddEdge(e.start, e.finish);
                dfs_stack.push(e.finish);
                used[e.finish] = true;
            }
            else if (!HasEdge(e.finish, e.start) and !(find(missingEdges.begin(), missingEdges.end(), Edge(e.finish, e.start)) != missingEdges.end()))
                    missingEdges.push_back(Edge(e.start, e.finish));
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
    return missingEdges;
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
    //cout << "\n";
    //Строим остовное дерево
    Graph graph(vertecies.size());
    Graph new_graph(vertecies.size());
    vector<Edge> missingEdges = graph.GenerateSpanningTree(*this);
    //Добавляем по очереди не вошедшие ребра
    while (!missingEdges.empty())
    {
        Edge edge = missingEdges.back();
        missingEdges.pop_back();
        graph.AddEdge(edge.start, edge.finish);
        if (!graph.isBipartite()) 
        {
            graph.DeleteLastEdge(edge.start);
            new_graph.AddEdge(edge.start, edge.finish);
        }
    }
    cout << "Bipartite component #" << n << "\n";
    graph.PrintAdjacencyMatrix();
    cout << "------------------------------------------\n";
    new_graph.PrintBipartiteComponents(n + 1);
}

bool Graph::isBipartite() const 
{
    vector<int> color(vertecies.size(), -1);
    stack<int> dfs_stack;

    dfs_stack.push(0);
    color[0] = 0;
    while (!dfs_stack.empty())
    {
        int i = dfs_stack.top(); dfs_stack.pop();
        const Vertex& v = vertecies[i];
        for (Edge e : v.edges)
        {
            if (color[e.finish] == -1)
            {
                dfs_stack.push(e.finish);
                color[e.finish] = !color[i];
            }
            else if (color[e.finish] == color[i])
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
