/*  src/main.cpp */
#include <bits/stdc++.h>
#include "../include/HashTable.h"
#include "../include/Registro.h"
#include "../include/ArbolAVL.h"

using namespace std;

int main() {
    HashTable ht(5);
    ArbolAVL avl;

    Registro* r1 = new Registro{
        "lenin", 
        "profesor", 
        TipoDato::STRING, 
        false,
        0
    };

    Registro* r2 = new Registro{
        "salazar", 
        "imbecil", 
        TipoDato::STRING, 
        false,
        0
    };

    Registro* r3 = new Registro{
        "chucky", 
        "ure", 
        TipoDato::STRING, 
        false,
        0
    };

    avl.insertar(r1);
    avl.insertar(r2);
    avl.insertar(r3);
    avl.imprimir();

    Registro* busqueda2 = avl.buscar("carlos");
    if (!busqueda2) {
        cout << "Carlos no existe" << endl;
    }

    Registro* busqueda = avl.buscar("chucky");
    if (busqueda) {
        cout << "Encontrado: " << busqueda->valor << endl;
    } else {
        cout << "Registro no encontrado" << endl;
    }

    avl.eliminar("chucky");
    avl.imprimir();

    delete r1;
    delete r2;
    delete r3;

    return 0;
}