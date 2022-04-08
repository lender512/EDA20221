
#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#define MAX_KEYS 3

#include <vector>
#include <math.h>
#include <queue>
#include <set>
#include <stack>

using namespace std;

int MIN_KEYS = ceil((MAX_KEYS+1)/2.0-1);


struct Node{
    int keys[MAX_KEYS];
    Node* children[MAX_KEYS + 1];
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

            //Copiar parte de los valores del array auxiliar que
            //se mantendrán en los nodos
            for (int j = 0; j < newSize + 1; j++) {
                if (j < newSize)
                    node->keys[j] = auxArray[j];
                node->children[j] = auxChildren[j];
            }
            node->size = newSize;
            
            //Copiar parte de los valores del array auxiliar que
            //Iran al nuevo nodo
            int k = newSize + 1;
            for (i = 0; i < MAX_KEYS - newSize + 1; i++) {
                if (i < MAX_KEYS - newSize) 
                    newNode->keys[i] = auxArray[k];
                newNode->children[i] = auxChildren[k];
                k++;
            }
            newNode->size = MAX_KEYS - newSize;
    
            if (node != root) {
                //El nuevo nodo va a tener que ser insertado entre los nodos internos
                insertarRecursive(auxArray[node->size],Parent(root, node), newNode);
                return;
            } 
            //El nodo que se spliteo fue el root
            Node* newRoot = new Node;

            newRoot->keys[0] = auxArray[node->size];
            // Asignas los hijos spliteados
            newRoot->children[0] = node;
            newRoot->children[1] = newNode;
            newRoot->leaf = false;
            newRoot->size = 1;
            root = newRoot;
            
        }
    }
    Node* Parent(Node* node, Node* child) {
        Node* parent = nullptr;

        if (node->leaf || (node->children[0])->leaf) {
            //No se encontro padre
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
            Node* parent = nullptr;
    
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

                if (node != root) {
                    //El nuevo nodo va a tener que ser insertado entre los nodos internos
                    insertarRecursive(newNode->keys[0], parent, newNode);
                    return;
                }
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
        }
    }

    void insertOrdered(Node* node, int value) {
        int place = 0;
        while (node->keys[place] < value && place < node->size) {
            place++;
        }
        for (int i = node->size; i > place; i--) {
            node->keys[i] = node->keys[i-1];
        }
        node->keys[place] = value;
        node->size++;
    }
    void remove(Node* node, int value) {
        int place = 0;
        while (node->keys[place] != value  && place < node->size) {
            place++;
        }
        for (int i = place; i < node->size; i++) {
            node->keys[i] = node->keys[i+1];
        }
        node->size--;
    }

    void merge(Node* a, Node* b){
        for (int i = 0; i < b->size; i++){
            a->keys[a->size + i] = b->keys[i];
        }
        a->size += b->size;
    }

    void deleteChildren(Node* node, int childIdx) {
        int i;
        for(i = childIdx; i < node->size+1; i++) {
            node->children[i] = node->children[i+1];
        }
        // while (i <= MAX_KEYS) {
        //     node->children[i] = nullptr;
        // }
    }

    void passChildLeftRight(Node* leftChild, Node* node){

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
    
    void deleteNode(Node * node, Node* parent, int nodeIndex, int savedSize, int n, int savedKeys[], stack<Node*> &path) {
        if (node == root && node->size == 0) {
                        root = node->children[0];
                        }
        for (int i = 0; i < savedSize; i++) {
                if (n == savedKeys[i]) {
                    // node->size--;
                    //El valor se encuentra en el nodo
                    
                    if (node == root) {
                        remove(node, n);

                        if (savedSize > 1) {
                            //El nodo se queda uno o más elementos
                            //Done
                        } else {
                            if (node->children[1]->size > MIN_KEYS) {
                                //Presta del hijo derecho
                                Node* rightChild = node->children[1];
                                insertOrdered(node, rightChild->keys[0]);
                                remove(rightChild, rightChild->keys[0]);
                            } else if (node->children[0]->size > MIN_KEYS) {
                                Node* leftChild = node->children[0];
                                insertOrdered(node, leftChild->keys[leftChild->size-1]);
                                remove(leftChild, leftChild->keys[leftChild->size-1]);
                            } else {
                            }
                        }
                    } else if (!node->leaf) {
                        remove(node, n);
                        if (node->size >= MIN_KEYS) {
                            //DONE
                        } else {
                            if ((nodeIndex-1 >= 0 && parent->children[nodeIndex-1]->size > MIN_KEYS) || 
                                (parent->children[nodeIndex+1] == node && parent->children[nodeIndex]->size > MIN_KEYS)) {
                                //Hermano izquierdo puede prestar
                                Node* leftSibiling;
                                if (nodeIndex-1 >= 0 && parent->children[nodeIndex-1]->size > MIN_KEYS) {
                                    leftSibiling = parent->children[nodeIndex-1];
                                } else {
                                    leftSibiling = parent->children[nodeIndex];
                                }
                                insertOrdered(node, parent->keys[nodeIndex]);
                                parent->keys[nodeIndex] = leftSibiling->keys[leftSibiling->size-1];
                                //shift node children to the right;
                                for (int j = node->size+1; j >= 0; j--){
                                    node->children[j] = node->children[j-1];
                                }
                                node->children[0] = leftSibiling->children[leftSibiling->size];
                                //Remove last element of leftSibiling
                                leftSibiling->size--;

                            }
                            else if (nodeIndex+1 <= parent->size && parent->children[nodeIndex+1]->size > MIN_KEYS) {
                                //Hermano derecho puede prestar
                                Node* rightSibiling = parent->children[nodeIndex+1];
                                insertOrdered(node, parent->keys[nodeIndex]);
                                parent->keys[nodeIndex] = rightSibiling->keys[0];

                                node->children[node->size] = rightSibiling->children[0];

                                for (int j = 0; j < rightSibiling->size+1; j++){
                                    rightSibiling->children[j] = rightSibiling->children[j+1];
                                }
                                remove(rightSibiling, rightSibiling->keys[0]);


                            } else {
                                if ((nodeIndex-1 >= 0 > MIN_KEYS) || parent->children[nodeIndex+1] == node) {
                                    //Puede hacer merge con el hermano izquierdo
                                    Node* leftSibiling;
                                    if (nodeIndex-1 >= 0 && parent->children[nodeIndex-1]->size > MIN_KEYS) {
                                        leftSibiling = parent->children[nodeIndex-1];
                                    } else {
                                        leftSibiling = parent->children[nodeIndex];
                                    }
                                    insertOrdered(node,  parent->keys[nodeIndex]);
                                    parent->keys[nodeIndex] = n;
                                    parent->size--;
                                    for (int j = 0; j < node->size+1; j++) {
                                        leftSibiling->children[leftSibiling->size+j] = node->children[j];
                                    }
                                    merge(leftSibiling, node);
                                    deleteChildren(parent, nodeIndex+1);

                                } else if (nodeIndex+1 <= parent->size) {
                                    //Puede hacer merge con el hermano derecho
                                    Node* rightSibiling = parent->children[nodeIndex+1];
                                    insertOrdered(node,  parent->keys[nodeIndex]);
                                    parent->keys[nodeIndex] = n;
                                    parent->size--;
                                    merge(node, rightSibiling);
                                    for (int j = 0; j < rightSibiling->size+1; j++) {
                                        node->children[savedSize+j] = rightSibiling->children[j];
                                    }
                                    deleteChildren(parent, nodeIndex+1);

                                    // deleteNode(parent, nullptr, 0, parent->size, )
                                }
                            }
                        }
                    } else if (node->leaf) {
                        //Eliminar n del nodo
                        remove(node, n);


                        if (node->size >= MIN_KEYS) {
                            //nodo tiene mas o el minimo de keys
                            if (i==0) {
                                bool found = false;
                                //Si el eliminado fue el menor, push up el siguiente
                                while (!found) {
                                    Node* parent = path.top();
                                    for (int j = 0; j < parent->size; j++) {
                                        if (n == parent->keys[j]) {
                                            found = true;
                                            parent->keys[j] = node->keys[0];
                                        }
                                    }
                                    path.pop();
                                }
                                
                            }
                            return;
                        } else {
                            //nodo tiene menos del minimo de keys
                            if ((nodeIndex-1 >= 0 && parent->children[nodeIndex-1]->size > MIN_KEYS) || 
                                (parent->children[nodeIndex+1] == node && parent->children[nodeIndex]->size > MIN_KEYS)) {
                                //Hermano izquierdo puede prestar
                                Node* leftSibiling;
                                if (nodeIndex-1 >= 0 && parent->children[nodeIndex-1]->size > MIN_KEYS) {
                                    leftSibiling = parent->children[nodeIndex-1];
                                } else {
                                    leftSibiling = parent->children[nodeIndex];
                                }
                                
                                //hermano izquierdo presta al nodo
                                // node->size++;
                                insertOrdered(node, leftSibiling->keys[leftSibiling->size-1]);
                                insertOrdered(parent, leftSibiling->keys[leftSibiling->size-1]);
                                leftSibiling->size--;   

                            } else if (nodeIndex+1 <= parent->size && parent->children[nodeIndex+1]->size > MIN_KEYS) {
                                //hermano derecho puede prestar
                                Node* rightSibiling = parent->children[nodeIndex+1];
                                //hermano derecho presta al nodo
                                node->size++;
                                node->keys[node->size-1] = rightSibiling->keys[0];

                                //Shiftear keys del hermano derecho a la izquierda
                                rightSibiling->size--;
                                for (int j = 0; j < rightSibiling->size; j++) {
                                    rightSibiling->keys[j] = rightSibiling->keys[j+1];
                                }
                                insertOrdered(parent, rightSibiling->keys[0]);
                                // parent->keys[0] = rightSibiling->keys[0];
                                
                            } else {
                                //Ningun hermano le puede prestar
                                if (nodeIndex-1 >= 0 || parent->children[nodeIndex+1] == node) {
                                    //Pude hacer merge con el hermano izquierdo
                                    Node* leftSibiling;
                                    if (nodeIndex-1 >= 0) {
                                        leftSibiling = parent->children[nodeIndex-1];
                                    } else {
                                        leftSibiling = parent->children[nodeIndex];
                                    }
                                    merge(leftSibiling, node);
                                    if (nodeIndex-1 >= 0) {
                                        deleteChildren(parent, nodeIndex);
                                    } else {
                                        deleteChildren(parent, nodeIndex+1);
                                    }

                                }
                                else if (nodeIndex+1 <= parent->size) {
                                    //Merge con el hermano derecho
                                    Node* rightSibiling = parent->children[nodeIndex+1];
                                    merge(node, rightSibiling);
                                    deleteChildren(parent, nodeIndex+1);
                                }
                                
                            }
                        }
                    }
                }
            }
    }

    void borrar(int n) {
        if (root == nullptr) {
            //El arbol está vacio
            return;
        }
        bool reachEnd = false;
        Node* node = root;
        stack<Node*> path;
        path.push(node);
        if (node == nullptr) {
            return;
        }
        while (!node->leaf) {

            for (int i = 0; i < node->size; i++) {
                if (n < node->keys[i]) {
                    //Se econtró por donde debe ir el node
                    // (hijo izquierdo)
                    node = node->children[i];
                    path.push(node);
                    break;
                }
                if (i == node->size - 1) {
                    //Llego al final, debe bajar por el 
                    // Hijo derecho
                    node = node->children[i + 1];
                    path.push(node);
                    break;
                }
            }
            
        }

        while (!path.empty()) {
            Node* node = path.top();
            int nodeIndex = 0;
            path.pop();
            Node* parent = nullptr;
            if (!path.empty()) {
                parent = path.top();
                for (int i = 0; i < parent->size; i++) {
                    if (parent->children[i] == node) {
                        nodeIndex = i;
                        break;
                    }
                }
            }
            int savedSize = node->size;

            //Guardar una referencia a los nodos sin alterar
            int savedKeys[MAX_KEYS];
            for (int i = 0; i < node->size; i++) {
                savedKeys[i] = node->keys[i]; 
            }

            deleteNode(node, parent, nodeIndex, savedSize, n, savedKeys, path);
            
        }
        return;

        // int foundIdx = 0;
        // bool found = false;
        // for (int i = 0; i < node->size; i++) {
        //     if (node->keys[i] == n){
        //         found = true;
        //         foundIdx = i;
        //         if (!repeatedNode) {
        //             repeatedNode = node;
        //         }
        //     }
                
        // }
        // if (!found) {
        //     //El valor a eliminar no existe en el arbol.
        //     return;
        // }

        // node->size--;
        // if (repeatedNode->leaf) {
            
        // }
    }

    void deleteRecursive(Node* node) {
        if (node) {
            if (!node->leaf){
                for (int i = 0; i < node->size+1; ++i) {
                    deleteRecursive(node->children[i]);
                }
            }
            delete node;
        }
    }

    ~BplusTree() {
        deleteRecursive(root);
        return;
    
    }
};

#endif
