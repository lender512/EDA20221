#include <iostream>
#include "BplusTree.h"

#include <fstream>
#include <chrono>
#include <vector>

using namespace std;

int main() {
    // Read file
    std::ifstream texto;
    texto.open("./output.txt");

    int datos[ 1000000 ];
    int element;
    if (texto.is_open()) {
        int i = 0;
        while (texto >> element) {
            datos[i++] = element;
        }
    }

    int64_t tiempoInseccion = 0;
    // for (int t=0; t<10; ++t){
    //     BplusTree* tree = new BplusTree();

    //     std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    //     for (int i=0; i < 1000000; ++i){
    //         tree->insertar(datos[i]);
    //     }

    //     std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    //     // Calcular tiempo
    //     auto tiempo = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    //     tiempoInseccion += tiempo;

    //     delete tree;
    //     // :C
    // }

    std::cout << "Tiempo Inseccion: " << tiempoInseccion << std::endl;

    BplusTree* tree = new BplusTree();

    tree->insertar(1);
    tree->insertar(4);
    tree->insertar(7);
    tree->insertar(10);
    tree->insertar(17);
    tree->insertar(21);
    tree->insertar(31);
    tree->insertar(25);
    tree->insertar(19);
    tree->insertar(20);
    tree->insertar(28);
    tree->insertar(42);

    tree->borrar(21);
    tree->borrar(31);
    tree->borrar(20);
    tree->borrar(10);
    tree->borrar(7);
    tree->borrar(25);
    tree->borrar(42);
    tree->borrar(17);
    tree->borrar(4);

    auto v = tree->bfs();
    for (int i=0; i < v.size(); i++) {
        cout << v[i] << "\n";
    }

}