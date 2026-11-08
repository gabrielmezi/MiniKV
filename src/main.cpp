/*  src/main.cpp */
#include <bits/stdc++.h>
#include "../include/HashTable.h"
#include "../include/Registro.h"

using namespace std;

int main() {
    HashTable ht(5);

    Registro* r1 = new Registro("lenin", "profesor", TipoDato::STRING, false, 0);

    ht.insertar(r1);
    ht.imprimir();

}