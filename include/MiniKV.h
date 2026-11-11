
#pragma once
#include <bits/stdc++.h>
#include "Registro.h"
#include "HashTable.h"
#include "ArbolAVL.h"
#include "MinHeap.h"

using namespace std;

class MiniKV {
private:
    HashTable hash;
    ArbolAVL arbol;
    MinHeap heapTTL;
    unsigned long long siguienteVersion = 0;
    int cantidadString = 0;
    int cantidadInt = 0;
    int cantidadFloat = 0;
    int cantidadBool = 0;

    void limpiarExpirados();

    void aumentarTipo(TipoDato tipo);
    void disminuirTipo(TipoDato tipo);
public:
    MiniKV();
    ~MiniKV();

    MiniKV(const MiniKV&) = delete;
    MiniKV& operator=(const MiniKV&) = delete;

    bool set(string clave, string valor, TipoDato tipo = TipoDato::STRING);
    bool setNX(string clave, string valor, TipoDato tipo = TipoDato::STRING);
    bool setEX(string clave, string valor, int segundos, TipoDato tipo = TipoDato::STRING);
    Registro* get(string clave);
    bool del(string clave);
    bool exists(string clave);

    vector<Registro*> range(string inicio, string fin, int limite = -1);
    vector<Registro*> prefix(string p, int limite = -1);

    Registro* successor(string clave);
    Registro* predecessor(string clave);

    Registro* min();
    Registro* max();

    int count(string inicio, string fin);

    vector<Registro*> dump(int limite = 50);

    bool expire(string clave, int segundos);
    int ttl(string clave);

    void stats();

    bool save(string archivo);
    bool load(string archivo);
    

    int cargarCSV(string archivo);

    bool validarTipo(string valor, TipoDato tipo);
};

