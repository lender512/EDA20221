#include "iostream"
#include "rbtree.cpp"

using namespace std;

int main(int argc, char const *argv[])
{
    RBtree tree = RBtree();
    tree.insert(10);
    tree.insert(5);
    tree.insert(30);
    tree.insert(1);
    tree.insert(7);
    tree.insert(25);
    tree.insert(40);
    tree.insert(20);
    tree.insert(28);
    tree.insert(26);


    cout << "Postorder:\n";
    vector<int> po = tree.posorden();
    for (auto n : po) {
        cout << n << " ";
    }
    cout << endl;
    cout << "Inorder:\n";
    vector<int> in = tree.inOrden();
    for (auto n : in) {
        cout << n << " ";
    }
    cout << endl;
    cout << "Preorder:\n";
    vector<int> pre = tree.preorden();
    for (auto n : pre) {
        cout << n << " ";
    }
    cout << endl;

    
    tree.remove(25);
    tree.remove(7);
    tree.remove(26);
    

    cout << "Postorder:\n";
    po = tree.posorden();
    for (auto n : po) {
        cout << n << " ";
    }
    cout << endl;
    cout << "Inorder:\n";
    in = tree.inOrden();
    for (auto n : in) {
        cout << n << " ";
    }
    cout << endl;
    cout << "Preorder:\n";
    pre = tree.preorden();
    for (auto n : pre) {
        cout << n << " ";
    }
    cout << endl;

    return 0;
}
