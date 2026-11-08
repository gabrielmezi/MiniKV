/*  src/HashTable.cpp */
#include <bits/stdc++.h>
#include "../include/HashTable.h"
#include "../include/Registro.h"

using namespace std;

Nodo::Nodo(Registro* r) {
    registro = r;
    siguiente = nullptr;
}


int HashTable::hashFuncion(string clave) {
    int h = 5381;
    for (char c : clave) {
        h = h * 33 + (int)c;
    }
    return abs(h) % capacidad;
}


void HashTable::rehash(int nuevaCapacidad) {
    vector<Nodo*> viejaTabla = tabla;
    int viejaCapacidad = capacidad;
    capacidad = nuevaCapacidad;
    tabla.assign(capacidad, nullptr);
    elementos = 0;
    for (int i = 0; i < viejaCapacidad; i++) {
        Nodo* actual = viejaTabla[i];
        while (actual != nullptr) {
            insertar(actual->registro);
            Nodo* aBorrar = actual;
            actual = actual->siguiente;
            delete aBorrar;
        }
    }
}


HashTable::HashTable(int cap) {
    capacidad = cap;
    tabla.assign(capacidad, nullptr);
    }


HashTable::~HashTable() {
    for (int i = 0; i < capacidad; i++) {
        Nodo* actual = tabla[i];
        while (actual != nullptr) {
            Nodo* aBorrar = actual;
            actual = actual->siguiente;
            delete aBorrar;
        }
    }
}


void HashTable::insertar(Registro* r) {
    if ((double)elementos / capacidad >= 0.75) {
        rehash(capacidad * 2);
    }
    int index = hashFuncion(r->clave);
    Nodo* actual = tabla[index];
    while (actual != nullptr) {
        if (actual->registro->clave == r->clave) {
            actual->registro->valor = r->valor;
            return;
        }
        actual = actual->siguiente;
    }
    Nodo* nuevo = new Nodo(r);
    nuevo->siguiente = tabla[index];
    tabla[index] = nuevo;
    elementos++;
}



Registro* HashTable::buscar(string clave) {
    int index = hashFuncion(clave);
    Nodo* actual = tabla[index];
    while (actual != nullptr) {
        if (actual->registro->clave == clave) {
            return actual->registro->valor;
        }
        actual = actual->siguiente;
    }
    return nullptr; 
}


bool HashTable::eliminar(string clave) {
    int index = hashFuncion(clave);
    Nodo* actual = tabla[index];
    Nodo* anterior = nullptr;

    while (actual != nullptr) {
        if (actual->registro->clave == clave) {
            if (anterior == nullptr) {
                tabla[index] = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }

            delete actual;
            elementos--;
            return true;
        }

        anterior = actual;
        actual = actual->siguiente;
    }
    return false;
}


void HashTable::imprimir() {
    cout << "Tabla (capacidad=" << capacidad
            << ", elementos=" << elementos << ")\n";

    for (int i = 0; i < capacidad; i++) {
        cout << "  [" << i << "] ";

        if (tabla[i] == nullptr) {
            cout << "(vacio)\n";
        } else {
            Nodo* actual = tabla[i];
            while (actual != nullptr) {
                cout << "(\"" << actual->registro->clave
                        << "\"," << actual->registro->valor << ")";
                if (actual->siguiente != nullptr) cout << " -> ";
                actual = actual->siguiente;
            }
            cout << "\n";
        }
    }
    cout << "\n";
}