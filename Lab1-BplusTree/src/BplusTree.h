
#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#define MAX_KEYS 21-1

#include <vector>
#include <math.h>
#include <queue>

using namespace std;


struct Node{
    int keys[MAX_KEYS];
    Node* children[MAX_KEYS + 1];
    Node* parent;
    bool leaf;
    int size;
};

class BplusTree
{
private:
    Node* root;
    void insertarRecursive(int n, Node* node, Node* child) {
    
        //El no tiene menos de la cantidad maxima de keys 
        if (node->size < MAX_KEYS) {
            int i = 0;
    
            //Encontrar el indice del último elemento que se
            //Quedará en su lugar
            while (n > node->keys[i] && i < node->size) {
                i++;
            }
    
            //Shiftear desde atras hacia i todos los elementos e hijos 
            // para hacer espacio al nuevo elemento
            for (int j = node->size+1; j > i; j--) {
                if (j < node->size+1) 
                    node->keys[j]= node->keys[j-1];
                if (j > i +1) 
                    node->children[j] = node->children[j-1];

            }

            //Insertar el elemento y aumentar el dato del tamaño
            node->keys[i] = n;
            node->size++;
            node->children[i + 1] = child;

        } else {
    
            //Necesitaremos crear un nuevo nodo
            Node* newNode = new Node;
            int auxArray[MAX_KEYS + 1];
            Node* auxChildren[MAX_KEYS + 2];

            //Copiamos los elementos del nodo al
            //Array auxiliar
            for (int i = 0; i < MAX_KEYS + 1; i++) {
                if (i < MAX_KEYS)
                    auxArray[i] = node->keys[i];
                auxChildren[i] = node->children[i];
            }
    
            int i = 0;
            int j;
    
            // Econtrar el indicide done debe ir el nuevo elemento
            while (n > auxArray[i] && i < MAX_KEYS) {
                i++;
            }
    
            //Se colocan los elementos de atras hacia el indice
            for (int j = MAX_KEYS + 1; j > i; j--) {
                if (j < MAX_KEYS + 1)
                auxArray[j] = auxArray[j-1];
                if (j > i + 1)
                    auxChildren[j] = auxChildren[j-1];
            }

            //Se inserta el valor
            auxArray[i] = n;
    
            auxChildren[i + 1] = child;
            newNode->leaf = false;


            int newSize = (MAX_KEYS + 1)/2;

            node->size = newSize;
            newNode->size = MAX_KEYS - newSize;
    
            ///Copiar parte de los valores del array auxiliar que
            //Iran al nuevo nodo
            int k = newSize + 1;
            for (i = 0; i < MAX_KEYS - newSize + 1; i++) {
                if (i < MAX_KEYS - newSize) 
                    newNode->keys[i] = auxArray[k];
                newNode->children[i] = auxChildren[k];
                k++;
            }
    
    
            if (node == root) {
                //El nodo que se spliteo fue el root
                Node* newRoot = new Node;
    
                newRoot->keys[0] = node->keys[node->size];
                // Asignas los hijos spliteados
                newRoot->children[0] = node;
                newRoot->children[1] = newNode;
                newRoot->leaf = false;
                newRoot->size = 1;
                root = newRoot;
            } else {
                //El nuevo nodo va a tener que ser insertado entre los nodos internos
                insertarRecursive(node->keys[node->size],Parent(root, node), newNode);
            }
        }
    }
    Node* Parent(Node* node, Node* child) {
        Node* parent;

        if (node->leaf || (node->children[0])->leaf) {
            //No se encontro padre, es root
            return nullptr;
        }
    
        for (int i = 0; i < node->size + 1; i++) {
            if (node->children[i] == child) {
                //Se econtró el padre
                parent = node;
                return parent;
            } else {
                //Llamar recursivamente
                parent = Parent(node->children[i], child);
                if (parent != nullptr)
                    return parent;
            }
        }
        return parent;
    }

public:
    BplusTree() {};
    void insertar(int n) {
        if (root == nullptr) {
            //El árbol está vacio
            //Crear un nuevo nodo e insertarlo
            root = new Node;
            root->keys[0] = n;
            root->leaf = true;
            root->size = 1;
        } else {
            //El arbol no está vacio
            Node* node = root;
            Node* parent;
    
            //Iterar hasta que se encuentre el nodo
            //hoja donde tiene que estar el valor
            while (!node->leaf) {
                //Guardar referencia al padre
                parent = node;
    
                for (int i = 0; i < node->size; i++) {
                    if (n < node->keys[i]) {
                        //Se econtró por donde debe ir el node
                        // (hijo izquierdo)
                        node = node->children[i];
                        break;
                    }
                    if (i == node->size - 1) {
                        //Llego al final, debe bajar por el 
                        // Hijo derecho
                        node = node->children[i + 1];
                        break;
                    }
                }
            }

            //Se encontró el nodo donde debe ir el elemento
            if (node->size < MAX_KEYS) {
                //El no tiene menos de la cantidad maxima de keys 
                //(insert normal)

                int i = 0;
                //Encontrar el indice del último elemento que se
                //Quedará en su lugar
                while (n > node->keys[i] && i < node->size) {
                    i++;
                }
                //Shiftear desde atras hacia i todos los elementos para hacer espacio
                //Al nuevo elemento
                for (int j = node->size; j > i; j--) {
                    node->keys[j] = node->keys[j-1];
                }

                //Insertar el elemento y aumentar el dato del tamaño
                node->keys[i] = n;
                node->size++;
    
                node->children[node->size] = node->children[node->size-1];
                node->children[node->size-1] = nullptr;

            } else {
                //Necesitaremos crear un nuevo nodo
                Node* newNode = new Node;
    
                int auxArray[MAX_KEYS + 1];
    
                //Copiamos los elementos del nodo al
                //Array auxiliar
                for (int i = 0; i < MAX_KEYS; i++) {
                    auxArray[i] = node->keys[i];
                }
                int i = 0;
                int j;

                // Econtrar el indicide done debe ir el nuevo elemento
                while (n > auxArray[i] && i < MAX_KEYS) {
                    i++;
                }
    
                //Se colocan los elementos de atras hacia el indice
                for (int j = MAX_KEYS; j > i; j--) {
                    auxArray[j] = auxArray[j-1];
                }

                //Se inserta el valor
                auxArray[i] = n;
                newNode->leaf = true;

                int newSize = (MAX_KEYS + 1)/2;
                

                //Asignar nuevos hijos
                node->children[node->size] = newNode;
                newNode->children[newNode->size] = node->children[MAX_KEYS];
    
                node->children[MAX_KEYS] = nullptr;

                //Copiar la parte de los valores de nuevo arrays
                // que se quedarán en el nodo
                for (i = 0; i <  newSize; i++) {
                    node->keys[i] = auxArray[i];
                }
    
                //Copiar parte de los valores del array auxiliar que
                //Iran al nuevo nodo
                int k = newSize;
                for (i = 0; i < MAX_KEYS + 1 - newSize; i++) {
                    newNode->keys[i] = auxArray[k];
                    k++;
                }

                //Reasignan los tamaños para la particion
                node->size = newSize;
                newNode->size = MAX_KEYS + 1 - newSize;

                if (node == root) {
                    //El nodo que se spliteo fue el root

                    //Creas un nodo nuevo
                    Node* newRoot = new Node;

                    // Repites el valor
                    newRoot->keys[0] = newNode->keys[0];
                    // Asignas los hijos spliteados
                    newRoot->children[0] = node;
                    newRoot->children[1] = newNode;
                    newRoot->leaf = false;
                    newRoot->size = 1;
                    root = newRoot;
                }
                else {
                    //El nuevo nodo va a tener que ser insertado entre los nodos internos
                    insertarRecursive(newNode->keys[0], parent, newNode);
                }
            }
        }
    }

    vector<int> bfs(){
        std::vector<int> result;
        if(root == nullptr) return result;
        queue<Node*> q;
        q.push(root);
        while(!q.empty()){
            const Node* current = q.front();
            if (!current->leaf) {
                for(int i = 0; i< current->size+1; ++i){
                    if(current->children[i] != nullptr){
                        q.push(current->children[i]);
                    } 
                }
            }
            for(int i = 0; i<current->size; ++i){
                result.push_back(current->keys[i]);
            }
            q.pop();
        }
        return result;
    }
    // void borrar(int);

    // ~BplusTree();
};

#endif