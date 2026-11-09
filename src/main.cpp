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
        cout << "Encontrado: " << busqueda->valor << endl << endl;
    } else {
        cout << "Registro no encontrado" << endl << endl;
    }

    Registro* menor = avl.obtenerMinimo();
    Registro* mayor = avl.obtenerMaximo();

    if (menor) {
        cout << "MIN: " << menor->clave << endl;
    }

    if (mayor) {
        cout << "MAX: " << mayor->clave << endl << endl;
    }

    Registro* sig = avl.sucesor("lenin");
    Registro* ant = avl.predecesor("lenin");

    if (sig) {
        cout << "Sucesor: " << sig->clave << endl;
    } else {
        cout << "No existe sucesor" << endl;
    }

    if (ant) {
        cout << "Predecesor: " << ant->clave << endl;
    } else {
        cout << "No existe predecesor" << endl;
    }

    vector<Registro*> resultados = avl.rango("chucky", "salazar");
    cout << "----- RANGE ----" << endl;

    for (Registro* r : resultados) {
        cout << r->clave << ": " << r->valor << endl;
    }

    vector<Registro*> resultadosPref = avl.prefijo("ch");

    cout << "---- PREFIX ----" << endl;

    for (Registro* r : resultadosPref) {
        cout << r->clave << ": " << r->valor << endl;
    }

    avl.eliminar("chucky");
    avl.imprimir();

    delete r1;
    delete r2;
    delete r3;

    return 0;
}

