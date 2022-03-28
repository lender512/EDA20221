#ifndef RBTREE_H
#define RBTREE_H

#include "vector"

using namespace std;

// Lista de colores del arbol
enum Color {RED,BLACK,DOUBLE};

struct Node{
    // Atributos
    int data;
    Color color;

    // Relacion con otros Nodos
    Node *left;
    Node *right;
    Node *parent;

    // Constructor
    Node(int);
};

class RBtree
{
private:
    Node *root;
    Node *insertNode(Node* &, Node* &);
    void checkTree(Node* &);

    void swapColor(Node*, Node*);
    void clearTree(Node*);
    

    Color getColor(Node* );
    void setColor(Node* &, Color );

    Node *getSibiling(Node*);

    void rotateLeft(Node* &);
    void rotateRight(Node* &);

    void postOrderUtil(Node* &, vector<int>*);
    void inOrderUtil(Node* &, vector<int>*);
    void preOrderUtil(Node* &, vector<int>*);


public:
    RBtree();
    void insert(int);
    void eliminarNodo(Node*);
    void remove(int);
    Node* search(int);

    vector<int> posorden();
    vector<int> inOrden();
    vector<int> preorden();

    ~RBtree();
};






#endif