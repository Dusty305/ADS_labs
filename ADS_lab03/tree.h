#include <iostream>
#include <algorithm>
#include <cstring>
#include <time.h>
#include <cstdio>
#include <time.h>
#include <vector>

#include "timer.h"

using namespace std;

class Node {
    char data;
    Node* left; // left son
    Node* right; // right son
    int depth; // Node's depth

public:

    Node() : left(nullptr), right(nullptr) {}

    friend class Tree;
};

class Tree {
private:
    Node* root;
    int maxdepth, offset;
    char maxdata;
    char** SCREEN;
    vector<bool> binary;

    void DestroyNode(Node* node) {
        if (node) {
            DestroyNode(node->left);
            DestroyNode(node->right);
            delete node;
        }
    }

public:
    Tree() : root(nullptr), maxdepth(0), offset(40), maxdata('a') {
        srand(time(0));
    }

    ~Tree() {
        DestroyNode(root);
        for (int i = 0; i <= maxdepth; ++i) delete[]SCREEN[i];
        delete[]SCREEN;
    }

    Node* GetRoot() {
        return root;
    }

    Node* GnerateNode(int Depth) {
        bool flag = (Depth < rand() % 6 + 1) and (maxdata <= 'z');
        binary.push_back(flag);
        if (flag) {
            int dpth = Depth + 1;
            Node* node;
            node = new Node;
            maxdepth = max(maxdepth, Depth);
            node->data = maxdata++;
            node->depth = Depth;
            node->left = GnerateNode(dpth);
            node->right = GnerateNode(dpth);

            return node;
        }
        else return nullptr;
    }

    void GnerateTree() {
        cout << "Tree generation.\n";
        Timer timer;
        root = GnerateNode(0);
        SCREEN = new char* [maxdepth + 1];
        for (int i = 0; i <= maxdepth; ++i) 
            SCREEN[i] = new char[80];
    }

    void clearSCREEN() {
        for (int i = 0; i <= maxdepth; ++i)
            memset(SCREEN[i], '.', 80);
    }

    void PrintNodes(Node* v, int r, int c) {
        if (r && c && (c < 80)) 
            SCREEN[r - 1][c - 1] = v->data;
        if (r <= maxdepth) {
            if (v->left) 
                PrintNodes(v->left, r + 1, c - (offset >> r));
            if (v->right) 
                PrintNodes(v->right, r + 1, c + (offset >> r));
        }
    }

    void PrintTree() {
        Timer timer;
        clearSCREEN();
        PrintNodes(root, 1, offset);
        for (int i = 0; i <= maxdepth; ++i) {
            SCREEN[i][79] = 0;
            cout << '\n' << SCREEN[i];
        }
        cout << "\n\nTree printing\n";
    }

    int DFS(Node* node) {
        if (node)
        {
            if (node == root) {
                cout << "Algorithm\n";
                Timer timer;
                return DFS(root->left);
            }
            else if (node->left or node->right) 
                return max(DFS(node->left), DFS(node->right));
            else 
                return node->depth;
        }
        else return 0;
    }

    string BinaryRepresentation() {
        string str;
        for (bool b : binary)
            str.push_back(b ? '1' : '0');
        return str;
    }

};


