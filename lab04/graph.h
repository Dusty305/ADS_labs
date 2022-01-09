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
    bool operator==(const Edge& edge) { return start == edge.start && finish == edge.finish; }
};

struct Vertex 
{
    vector<Edge> edges;
    Vertex(int size = 0) { edges.reserve(size); }
    bool HasEdge(int start, int finish)
    {
        for (Edge e : edges)
            if (e.start == start and e.finish == finish)
                return true;
        return false;
    }
};

class Graph 
{
private:
    // TODO: ����� �� ���������� ������� � ���� ������� ������ � �������?
    vector<Vertex> vertecies;

public:

    Graph() { srand(time(0)); }

    Graph(vector<Vertex> vec) : vertecies(vec) { srand(time(0)); }

    Graph(const vector<vector<bool>>& adjMatrix)
    {
        vertecies = vector<Vertex>(adjMatrix.size(), Vertex(adjMatrix.size() - 1));
        for (int i = 0; i < adjMatrix.size(); ++i)
            for (int j = i + 1; j < adjMatrix.size(); ++j)
                if (adjMatrix[i][j]) // TODO: move?
                    AddEdge(Edge(i, j));
    }

    // ---------------------------------
    // Vertex related methods
    // ---------------------------------

    const vector<Vertex>& GetVerteceis() const { return vertecies; }

    const int GetVerteciesNumber() const { return vertecies.size(); }

    // TODO: ������� inline?
    bool IsValidVertexId(VertexId id) const
    {
        if (0 <= id && id < vertecies.size())
            return true;
        else
            return false;
    }

    // TODO: ����� �� �������� �� ������������ id?
    Vertex& GetVertex(VertexId id)
    {
        if (IsValidVertexId(id))
            return vertecies[id];
        else
            throw "Invalid vertex id!";
    }

    // TODO: � ������ ������ ���� ����� ���?
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

    // TODO: ��������� �� VertexId? �������� ����� find ������ ������������ �����?
    bool HasEdge(const Edge& edge) const
    {
        for (Edge e : vertecies[edge.start].edges)
            if (e.finish == edge.finish)
                return true;
        return false;
    }

    // TODO: ��������� �� VertexId?
    void AddEdge(const Edge& edge)
    {
        vertecies[edge.start].edges.push_back(Edge(edge.start, edge.finish));
        vertecies[edge.finish].edges.push_back(Edge(edge.finish, edge.start));
    }

    // TODO: ����� bool? ���� �� ������� ������� ��������?
    bool DeleteEdge(VertexId i, VertexId j) 
    {
        if (!IsValidVertexId(i) or !IsValidVertexId(j))
            return false;
        Vertex& v1 = vertecies[i];
        for (auto it = v1.edges.begin(); it < v1.edges.end(); ++it)
            if ((*it).finish == j)
            {
                v1.edges.erase(it);
                break;
            }

        Vertex& v2 = vertecies[j];
        for (auto it = v2.edges.begin(); it < v2.edges.end(); ++it)
            if ((*it).finish == i)
            {
                v2.edges.erase(it);
                break;
            }

        return true;
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

    void GenerateGraph(int n)
    {
        vertecies = vector<Vertex>(n, Vertex(n - 1));
        const int edges_number = rand() % (n * (n - 1) / 2) + 1;

        for (int k = 0; k < edges_number; ++k)
        {
            int i = rand() % n,
                j = rand() % n;
            if (i != j and !vertecies[i].HasEdge(i, j))
            {
                vertecies[i].edges.push_back(Edge(i, j));
                vertecies[j].edges.push_back(Edge(j, i));
            }
            else
                --k;
        }
    }   

    vector<Edge> GenerateSpanningTree(const Graph& graph);
    void PrintBipartiteComponents(const int n = 1);
    bool isBipartite() const;
};


// ���������� ������������� �����
vector<Edge> Graph::GenerateSpanningTree(const Graph& graph) {
    const int nOfV = graph.GetVerteciesNumber();
    vertecies = vector<Vertex>(nOfV, Vertex(nOfV - 1));
    stack<VertexId> dfs_stack;
    vector<bool> used(graph.GetVerteciesNumber(), false);
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
            Edge new_edge = Edge(e.start, e.finish);
            Edge new_edge_back = Edge(e.finish, e.start);
            if (!used[e.finish])
            {
                vertecies[id].edges.push_back(new_edge);
                vertecies[e.finish].edges.push_back(new_edge_back);
                dfs_stack.push(e.finish);
                used[e.finish] = true;
            }
            else // ���� �� ��������������� �������� ����� ��� ������������ �����
                if (!HasEdge(new_edge_back) and !(find(missingEdges.begin(), missingEdges.end(), new_edge_back) != missingEdges.end()))
                    missingEdges.push_back(new_edge);
        }
        // (��������������)
        // ���� ��������� ��������� ���������, �� ���� ��������� ���������� ��� ���������� �����
        if (dfs_stack.empty())
            for (int i = 0; i < used.size(); ++i)
                if (!used[i])
                {
                    dfs_stack.push(i);
                    used[i] = true;
                    break;
                }
    }

    return missingEdges;
}

void Graph::PrintBipartiteComponents(const int n) {
    if (isBipartite()) {
        cout << "Bipartite component #" << n << "\n";
        PrintAdjacencyMatrix();
        cout << "\n";
        return;
    }
    //������ �������� ������
    Graph graph;
    Graph new_graph;
    vector<Edge> missingEdges = graph.GenerateSpanningTree(*this);
    vector<Vertex> newGraphVertecies = vector<Vertex>(GetVerteciesNumber(), Vertex(GetVerteciesNumber() - 1));
    //��������� �� ������� �� �������� �����
    while (!missingEdges.empty())
    {
        Edge edge = missingEdges.back();
        missingEdges.pop_back();
        graph.AddEdge(edge);
        if (!graph.isBipartite()) 
        {
            graph.DeleteEdge(edge.start, edge.finish);
            newGraphVertecies[edge.start].edges.push_back(Edge(edge.start, edge.finish));
            newGraphVertecies[edge.finish].edges.push_back(Edge(edge.finish, edge.start));
        }
    }
    cout << "Bipartite component #" << n << "\n";
    graph.PrintAdjacencyMatrix();

    new_graph = Graph(newGraphVertecies);
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
        // (��������������)
        // ���� ��������� ��������� ���������, �� ���� ��������� ���������� ��� ���������� �����
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
