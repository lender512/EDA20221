#include <algorithm> 
#include "rbtree.h"

Node::Node(int dato){
    this->data = dato;
    color = RED;
    
    left   = nullptr;
    right  = nullptr;
    parent = nullptr;
}




Color RBtree::getColor(Node* puntero){
    if (puntero == nullptr) return BLACK;
    return puntero->color;
}

void RBtree::setColor(Node* &puntero, Color color){
    if (puntero == nullptr) return;
    puntero->color = color;
}

/*          
 *        X                            Y
 *       / \     Rotar izquierda      / \
 *      A   Y    --------------->    X   C
 *         / \                      / \
 *        B   C                    A   B
 * 
 * La función se aplica a X
 */
void RBtree::rotateLeft(Node* &puntero){
    // Nodo Y
    Node *rightSon = puntero->right;
    
    
    puntero->right = rightSon->left;         // Asignar como hijo derecho de X a B
    if (puntero->right != nullptr)
        puntero->right->parent = puntero;       // Asignar como nuevo padre de B a X


    rightSon->parent = puntero->parent;      // Asignar que el nuevo padre de Y sea el padre de X
                                                // Esto es para conectar con el resto del arbol
    // Asignar la Y como hijo derecho o izquierdo 
    // En el paso anterior contectamos a Y con el resto del arbol, ahora Y será asignado como hijo del nodo superior
    if (puntero->parent == nullptr)
        root = rightSon;                     // Si no hay padre, significa que llegamos a la raiz
    else if (puntero == puntero->parent->left)  
        puntero->parent->left = rightSon;    // Antes de la rotacion, X era el hijo izquiero
    else
        puntero->parent->right = rightSon;   // Antes de la rotacion, X era el hijo derecho

    rightSon->left = puntero;                // Asignar a X como hijo izquierdo de Y
    puntero->parent = rightSon;              // Asignar a Y como padre de X
}

/*         
 *        Y                        X
 *       / \     Rotar derecha    / \
 *      X   C    ------------->  A   Y
 *     / \                          / \
 *    A   B                        B   C
 * 
 * La función se aplica a Y
 */
void RBtree::rotateRight(Node* &puntero){
    // Nodo X
    Node *leftSon = puntero->left;        


    puntero->left = leftSon->right;       // Asignar como hijo izquierdo de Y a B
    if (puntero->left != nullptr)
        puntero->left->parent = puntero;        // Asignar como nuevo padre de B a Y


    leftSon->parent = puntero->parent;    // Asignar que el nuevo padre de X sea el padre Y
                                                // Esto es para conectar con el resto del arbol
    // Asignar la X como hijo derecho o izquierdo 
    // En el paso anterior contectamos a X con el resto del arbol, ahora X será asignado como hijo del nodo superior
    if (puntero->parent == nullptr)
        root = leftSon;                   // Si no hay padre, significa que llegamos a la raiz
    else if (puntero == puntero->parent->left)
        puntero->parent->left = leftSon;  // Antes de la rotacion, Y era el hijo izquiero
    else
        puntero->parent->right = leftSon;  // Antes de la rotacion, Y era el hijo derecho


    leftSon->right = puntero;              // Asignar a Y como hijo derecho de X
    puntero->parent = leftSon;             // Asignar a X como padre de Y
}



Node* RBtree::insertNode(Node* &padre, Node* &puntero){
    // Arbol vacio
    if (padre == nullptr) return puntero;
    
    // Si el nuevo numero es menor al padre
    if(puntero->data < padre->data){
        padre->left = insertNode(padre->left,puntero);
        padre->left->parent = padre;
    }
    // Si el nuevo numero es mayo al padre
    else if (puntero->data > padre->data){
        padre->right = insertNode(padre->right,puntero);
        padre->right->parent = padre;
    }
    return padre;
}


void RBtree::checkTree(Node* &puntero){
    Node* father  = nullptr;
    Node* granny = nullptr;
    while ( puntero != root &&  getColor(puntero)==RED && getColor(puntero->parent)==RED ){
        father  = puntero->parent;
        granny = father  ->parent;

        // El padre esta a la izquierda
        if (father == granny->left){
            Node* tio = granny->right;

            // CASO I: padre y tio son rojos
            if(  getColor(tio) ==RED ){
                setColor(father ,BLACK);
                setColor(tio   ,BLACK);
                setColor(granny,RED  );
                puntero = granny;
            }
            else{
                // CASO II: padre y el hijo tienen distintas direcciones
                if(puntero == father->right){
                    rotateLeft(father);
                    puntero = father;
                    father = puntero->parent;
                }

                // CASO III: padre y el hijo tienen la misma dirección
                rotateRight(granny);
                std::swap(father->color,granny->color);
                puntero = father;
            }
        }

        // El padre esta a la derecha
        else {
            Node* tio = granny->left;

            // CASO I: padre y tio son rojos
            if(  getColor(tio) ==RED ){
                setColor(father ,BLACK);
                setColor(tio   ,BLACK);
                setColor(granny,RED  );
                puntero = granny;
            }
            else{
                // CASO II: padre y el hijo tienen distintas direcciones
                if(puntero == father->left){
                    rotateRight(father);
                    puntero = father;
                    father = puntero->parent;
                }

                // CASO III: padre y el hijo tienen la misma dirección
                rotateLeft(granny);
                std::swap(father->color,granny->color);
                puntero = father;
            }
        }
    }
    setColor(root   ,BLACK);   
}

void RBtree::insert(int dato){
    Node *ptr = new Node(dato);
    root = insertNode(root,ptr);
    checkTree(ptr);
}

RBtree::RBtree(){
    root = nullptr;
}


Node* RBtree::getSibiling(Node* node) {
    if (node->parent->left == node) {
        return node->parent->right;
    }
    return node->parent->left;

}


// Just changing the order of the recursion to make each function
/*
    3
   / \
  1   2
*/
void RBtree::postOrderUtil(Node* & n, vector<int>* v){
    if (n == nullptr) return;
    postOrderUtil(n->left, v);
    postOrderUtil(n->right, v);
    v->push_back(n->data);
    return;
}
/*
    2
   / \
  1   3
*/
void RBtree::inOrderUtil(Node* & n, vector<int>* v){
    if (n == nullptr) return;
    inOrderUtil(n->left, v);
    v->push_back(n->data);
    inOrderUtil(n->right, v);
    return;
}
/*
    1
   / \
  2   3
*/
void RBtree::preOrderUtil(Node* & n, vector<int>* v){
    if (n == nullptr) return;
    v->push_back(n->data);
    preOrderUtil(n->left, v);
    preOrderUtil(n->right, v);
    return;
}

vector<int> RBtree::posorden(){
    vector<int> v;
    postOrderUtil(root, &v);
    return v;
}

vector<int> RBtree::preorden(){
    vector<int> v;
    preOrderUtil(root, &v);
    return v;
}

vector<int> RBtree::inOrden(){
    vector<int> v;
    inOrderUtil(root, &v);
    return v;
}

Node* RBtree::search(int val) {
    Node* temp = root;
    while (temp != nullptr) {
        if (temp->data == val) {
            return temp;
        } else if (temp->data < val) {
            temp = temp->right;
        } else {
            temp = temp->left;
        }
    }
    return temp;
}

// Swap two node's color
void RBtree::swapColor(Node* a, Node* b) {
    Color temp = getColor(a);
    setColor(a, getColor(b));
    setColor(b, temp);
}

//Deletes a node (db = doubleblack) based on chart from: https://medium.com/analytics-vidhya/deletion-in-red-black-rb-tree-92301e1474ea
void RBtree::eliminarNodo(Node* node) {
    if (getColor(node) == Color::RED) {
        //Standard BTS delete 
        if (node->left == nullptr && node->right == nullptr) {
            Node* parent = node->parent;
            //No children

            if (parent->left == node)
                parent->left = nullptr;
            if (parent->right == node)
                parent->right = nullptr;        

        } else if (node->left != nullptr && node->right == nullptr){
            //No left child
            Node* temp = node->left;
            Node* par = node->parent;
            if (par->data > temp->data)
                par->left = temp;
            if (par->data < temp->data)
                par->right = temp;
            
        } else if (node->left == nullptr && node->right != nullptr){
            //No right child
            Node* temp = node->right;
            Node* par = node->parent;
            if (par->data > temp->data)
                par->left = temp; 
            if (par->data < temp->data)
                par->right = temp;
        } else { 
            //Replace with successor 
        Node* temp = node->right;
        while (temp->left != nullptr) {
            temp = temp->left;
        }
        int tempData = temp->data;
        eliminarNodo(temp);
        node->data = tempData;
        }

    } else if (getColor(node) == Color::BLACK && node->left == nullptr && node->right == nullptr) {
        //Black leaf
        setColor(node, Color::DOUBLE);
        eliminarNodo(node);
    } else if (getColor(node) == Color::DOUBLE) {
        if (node == root) {
            //if DB node is root
            setColor(node, Color::BLACK);
        } else if (getColor(getSibiling(node)) == Color::BLACK) {
            //sibiling is black
            Node* sibiling = getSibiling(node);
            if (getColor(sibiling->left) == Color::BLACK && getColor(sibiling->right) == Color::BLACK) {
                //Sibiling's children are black
                //Remove double black sign
                setColor(node, Color::BLACK);
                Node* parent = node->parent;
                if (node->left == nullptr && node->right) {
                    //If db is nullptr delete it
                    node = nullptr;
                }
                //Make db's sibiling red
                setColor(sibiling, Color::RED);
                if (getColor(parent) == Color::BLACK) {
                    //if db parent is black make it double black
                    setColor(parent, Color::DOUBLE);
                    eliminarNodo(parent);

                } else {
                    //if not, make it it black
                    setColor(parent, Color::BLACK);
                }
            } else if ((node->parent->left == node && getColor(sibiling->right) == Color::BLACK && getColor(sibiling->left) == Color::RED) || 
                       (node->parent->right == node && getColor(sibiling->left) == Color::BLACK && getColor(sibiling->right) == Color::RED)){
                //bs 's sibiling's child that is far from db is black and near one is red

                //swap color between sibiling and its red child
                getColor(sibiling->right) == Color::RED ? swapColor(sibiling, sibiling->right) : swapColor(sibiling, sibiling->left);
                //rotate the sibiling in the db opposite direction
                if (sibiling->left == node) {
                    rotateRight(sibiling);
                } else {
                    rotateLeft(sibiling);
                }
                eliminarNodo(node);
            } else if ((node->parent->left == node && getColor(sibiling->right) == Color::RED) || (node->parent->right == node && getColor(sibiling->left) == Color::RED)) {
                //sibiling's child that is far from db is red
                //swap color between sibiling and db parent
                swapColor(node->parent, sibiling);
                //rotate parent in db direction
                if (node->parent->left == node) {
                    rotateLeft(node->parent);
                } else {
                    rotateRight(node->parent);
                }
                //remove db sign
                setColor(node, Color::BLACK);

                //change db's sibiling's red child's color to black
                if (getColor(sibiling->right) == Color::RED) {
                    setColor(sibiling->right, Color::BLACK);
                } else {
                    setColor(sibiling->left, Color::BLACK);
                }
            }
        } else if (getColor(getSibiling(node)) == Color::RED) {
            //sibiling is red
            Node* sibiling = getSibiling(node);
            swapColor(sibiling, node->parent);

            if (node->parent->left == node) {
                //perform right rotation
                rotateRight(node->parent);
            } else {
                //perform left rotation
                rotateLeft(node->parent);
            }
            //Check new tree strucure
            eliminarNodo(node);
        }
    } else {
        //Replace with successor 
        Node* temp = node->right;
        while (temp->left != nullptr) {
            temp = temp->left;
        }
        int tempData = temp->data;
        eliminarNodo(temp);
        node->data = tempData;
        }
    
}

void RBtree::remove(int n) {
    Node* node = search(n);
    eliminarNodo(node);
}

//Recursive Cleaner
void RBtree::clearTree(Node* node) {
    if (node != nullptr) {
        clearTree(node->left);
        clearTree(node->right);
        delete node; 
    }

}

RBtree::~RBtree(){
    clearTree(root);
}
