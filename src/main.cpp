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

    return 0;
}