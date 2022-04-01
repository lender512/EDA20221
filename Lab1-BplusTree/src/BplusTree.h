#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <vector>

using namespace std;


struct Node{
    int* keys;
    Node** children;
    Node* parent;
    bool leaf;
    int size;

    Node(int key);
};

class BplusTree
{
public:
    BplusTree();
    void insertar(int);
    void borrar(int);

    ~BplusTree();
};

#endif