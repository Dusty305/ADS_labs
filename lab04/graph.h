#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <time.h>
#include <cstdio>
#include <vector>
#include <stack>

using namespace std;

class Graph {
private:
    typedef int VertexId;
    struct Edge {
        VertexId start;
        VertexId finish;
        Edge(int strt = -1, int fnsh = -1) : start(strt), finish(fnsh) { }
    };

    struct Vertex {
        vector<Edge> edges;
        Vertex(int size = 0) { edges.reserve(size); }
        bool HasEdge(int start, int finish) {
            for (Edge e : edges)
                if (e.start == start and e.finish == finish)
                    return true;
            return false;
        }
    };

    vector<Vertex> vertecies;

public:

    Graph() { }

    const vector<Vertex>& GetVerteceis() const { return vertecies; }

    Vertex& GetVertex(VertexId id)
    {
        if (IsValidVertexId(id))
            return vertecies[id];
        else
            throw "Invalid vertex id!";
    }

    const Vertex& GetVertex(VertexId id) const
    {
        if (IsValidVertexId(id))
            return vertecies[id];
        else
            throw "Invalid vertex id!";
    }

    bool IsValidVertexId(VertexId id) const
    {
        if (0 <= id && id < vertecies.size())
            return true;
        else
            return false;
    }

    void PrintAdjacencyMatrix() const {
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
        srand(time(0));
        vertecies = vector<Vertex>(n, Vertex(n - 1));
        const int edges_number = rand() % (n * (n - 1) / 2);

        for (int k = 0; k < edges_number; ++k) 
        {
            int i = rand() % n, 
                j = rand() % n;
            if (i != j and !vertecies[i].HasEdge(i, j)) 
            {
                vertecies[i].edges.push_back(Edge(i, j));
                vertecies[j].edges.push_back(Edge(j, i));
            }
            // Стоит убрать, чтобы количество итераций не увеличивалось?
            else
                --k;
        }
    }

    vector<Graph> FindBipartiteComponents() {
        return vector<Graph>(0);
    }

    bool isBipartite() const {
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
        }
        return true;
    }

};



