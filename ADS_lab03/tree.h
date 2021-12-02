#include <iostream>
#include <algorithm>
#include <cstring>
#include <time.h>
#include <cstdio>


using namespace std;

class Node{
    char data;
    Node* left; // left son
    Node* right; // right son
    int depth; // Node's depth

    public:

        Node() : left(nullptr), right(nullptr) {}

    friend class Tree;
};

class Tree{
    private:
        Node* root;
        int maxdepth, offset;
        char maxdata;
        char** SCREEN;

        void DestroyNode(Node* node) {
            if (node) {
                DestroyNode(node->left);
                DestroyNode(node->right);
                delete node;
            }
        }

    public:
        Tree() : root(nullptr), maxdepth(0), offset(40), maxdata('a'){}

        ~Tree() {
            DestroyNode(root);
            for(int i=0; i < maxdepth; i++) delete []SCREEN[i];
            delete []SCREEN;
        }

        Node* GetRoot(){
            return root;
        }

        Node* GnerateNode(int Depth){
            srand(time(0));
            bool flag = Depth < rand()%6+1;
            if (flag and (maxdata <= 'z')){
                int dpth = Depth+1;
                Node* node;
                node = new Node;
                maxdepth = max(maxdepth, Depth);
                node -> data = maxdata;
                maxdata++;
                node -> depth = Depth;
                node -> left = GnerateNode(dpth);
                node -> right = GnerateNode(dpth); 

                return node;
            }
            else return nullptr;
        }

        void GnerateTree(){
            root = GnerateNode(0);  
            SCREEN = new char*[maxdepth];
            for(int i=0; i < maxdepth; i++) SCREEN[i] = new char[80];
        }

        void clearSCREEN(){
            for(int i = 0; i < maxdepth; i++)
                memset(SCREEN[i],'.',80);
        }

        void PrintNodes(Node* v, int r, int c){
            if(r && c && (c<80)) SCREEN[r-1][c-1] = v -> data;
            if(r < maxdepth){
                if (v -> left) PrintNodes(v -> left, r+1, c - (offset >> r));
                if (v -> right) PrintNodes(v -> right, r+1, c + (offset >> r));
            }
        }

        void PrintTree(){
            clearSCREEN();
            PrintNodes(root,1,offset);
            for(int i=0; i < maxdepth; i++){
                SCREEN[i][79] = 0;
                cout << '\n' << SCREEN[i];
            }
            cout << '\n';
        }

        int DFS(Node* node){
            if(node)
            {
                if(node == root){
                    return DFS(root -> left);
                }
                if(node and (node->left or node->right)){
                    return max(DFS(node->left),DFS(node->right));
                }
                else return node -> depth;
            }
            else return 0;
        }

};



