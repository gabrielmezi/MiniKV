/*  include/HashTable.h */
#pragma once
#include <bits/stdc++.h>
#include "Registro.h"

using namespace std;

struct Nodo {
    Registro* registro;
    Nodo*  siguiente;

    Nodo(Registro* r);
};

class HashTable {
private:
    vector<Nodo*> tabla; 
    int capacidad;
    int elementos = 0;
    int colisiones = 0;

    int hashFuncion(string clave);
    void rehash(int nuevaCapacidad);

public:
    HashTable(int cap = 8);
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    ~HashTable();

    void insertar(Registro* r);

    Registro* buscar(string clave);

    bool eliminar(string clave);

    void imprimir();

    int obtenerElementos();
    int obtenerCapacidad();
    int contarColisiones();

};