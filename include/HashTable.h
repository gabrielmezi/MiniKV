/*  include/HashTable.h */
#pragma once
#include <bits/stdc++.h>

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

    int hashFuncion(string clave);
    void rehash(int nuevaCapacidad);

public:
    HashTable(int cap = 8);

    ~HashTable();

    void insertar(Registro* r);

    Registro* buscar(string clave);

    bool eliminar(string clave);

    void imprimir();
};