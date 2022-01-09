#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>

using namespace std;

class Graph 
{
private:
    typedef int VertexId;
    vector<vector<bool>> matrix; // triangle adjecency matrix 
    int power;                   // number of vertecies
public:

    Graph(size_t size = 0) 
    { 
        matrix = vector<vector<bool>>(size);
        for (int i = 0; i < size; ++i)
            matrix[i] = vector<bool>(size - i, false);
        power = size;
        srand(time(0)); 
    }

    Graph(const vector<vector<bool>>& adjMatrix) : Graph(adjMatrix.size())
    {
        for (int i = 0; i < power; ++i)
            for (int j = i + 1; j < power; ++j)
                if (adjMatrix[i][j])
                    AddEdge(i, j);
    }

    pair<VertexId, VertexId> GetValidEdgeId(VertexId start, VertexId finish) const
    {
        if (start > finish)
            swap(start, finish);
        return make_pair(start, finish - start);
    }

    void AddEdge(VertexId start, VertexId finish) 
    {
        pair<VertexId, VertexId> edge = GetValidEdgeId(start, finish);
        matrix[edge.first][edge.second] = true;
    }

    void DeleteEdge(VertexId start, VertexId finish)
    {
        pair<VertexId, VertexId> edge = GetValidEdgeId(start, finish);
        matrix[edge.first][edge.second] = false;
    }

    const bool GetEdge(VertexId start, VertexId finish) const
    {
        pair<VertexId, VertexId> edge = GetValidEdgeId(start, finish);
        return matrix[edge.first][edge.second];
    }

    void PrintAdjacencyMatrix() const 
    {
        for (int i = 0; i < power; ++i)
        {
            for (int j = 0; j < power; ++j)
                cout << GetEdge(i, j) << " ";
            cout << "\n";
        }
    }

    void GenerateGraph()
    {
        const int n = power;
        const int edges_number = rand() % (power * (power - 1) / 2) + 1;

        for (int k = 0; k < edges_number; ++k)
        {
            VertexId i = rand() % power,
                     j = rand() % power;
            if (i != j and !GetEdge(i, j))
                AddEdge(i, j);
            else
                --k;
        }
    }   

    Graph GenerateSpanningTree(const Graph& graph);
    void PrintBipartiteComponents(const int n);
    bool IsBipartite() const;
};

Graph Graph::GenerateSpanningTree(const Graph& graph) {
    stack<VertexId> dfs_stack;
    vector<bool> used(power, false);
    Graph result_graph(power); // contains edges not included in "*this" graph 

    dfs_stack.push(0);
    used[0] = true;
    while (!dfs_stack.empty())
    {
        VertexId id = dfs_stack.top(); dfs_stack.pop();
        
        for (int i = 0; i < power; ++i)
        {
            if (!graph.GetEdge(id, i))
                continue;
            if (!used[i])
            {
                AddEdge(id, i);
                dfs_stack.push(i);
                used[i] = true;
            }
            else
                result_graph.AddEdge(id, i);
        }

        // Will work only if there is a number of 
        // connectivity components 
        // --------------------------------------------------------------
        if (dfs_stack.empty())
            for (int i = 0; i < used.size(); ++i)
                if (!used[i])
                {
                    dfs_stack.push(i);
                    used[i] = true;
                    break;
                }
    }

    return result_graph;
}

void Graph::PrintBipartiteComponents(const int n = 1) 
{
    if (IsBipartite())
    {
        cout << "Bipartite component #" << n << "\n";
        PrintAdjacencyMatrix();
        cout << "\n";
        return;
    }

    // Second component includes edges that will not be included 
    // in the first component
    // --------------------------------------------------------------
    Graph first_comp(power);
    Graph second_comp(power); 
    second_comp = first_comp.GenerateSpanningTree(*this);

    // Going through all edges that might be moved from
    // the second component to the first one 
    // --------------------------------------------------------------
    for(VertexId i = 0; i < power; ++i)
        for(VertexId j = 0; j < power; ++j)
        {
            if (!second_comp.GetEdge(i, j))
                continue;
            first_comp.AddEdge(i, j);
            if (first_comp.IsBipartite())
                second_comp.DeleteEdge(i, j);
            else
                first_comp.DeleteEdge(i, j);
        }

    cout << "Bipartite component #" << n << "\n";
    first_comp.PrintAdjacencyMatrix();
    second_comp.PrintBipartiteComponents(n + 1);
}

bool Graph::IsBipartite() const
{
    vector<int> color(power, -1);
    stack<int> dfs_stack;

    dfs_stack.push(0);
    color[0] = 0;
    while (!dfs_stack.empty())
    {
        VertexId id = dfs_stack.top(); dfs_stack.pop();
        for (VertexId i = 0; i < power; ++i)
        {
            if (!GetEdge(id, i))
                continue;
            if (color[i] == -1)
            {
                dfs_stack.push(i);
                color[i] = !color[id];
            }
            else if (color[i] == color[id])
                return false;
        }

        // Will work only if there is a number of 
        // connectivity components 
        // --------------------------------------------------------------
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
