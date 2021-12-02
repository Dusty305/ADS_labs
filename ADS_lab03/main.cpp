#include <iostream>

#include "tree.h"


int main(){
    Tree tree;

    tree.GnerateTree();
    tree.PrintTree();

    cout << "\nMax left depth: "<< tree.DFS(tree.GetRoot()) - 1 << '\n';

    return 0;
}