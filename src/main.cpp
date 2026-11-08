/*  src/main.cpp */
#include <bits/stdc++.h>
#include "../include/HashTable.h"

using namespace std;

int main() {
    HashTable ht(5);

    cout << "=== 1. Insertar apple, banana, cherry, date ===\n";
    ht.insertar("apple", 1);
    ht.insertar("banana", 2);
    ht.insertar("cherry", 3);
    ht.insertar("date", 4);
    ht.imprimir();

    cout << "=== 2. Actualizar banana a 99 ===\n";
    ht.insertar("banana", 99);
    ht.imprimir();

    cout << "=== 3. Buscar ===\n";
    int* v = ht.buscar("banana");
    if (v) cout << "banana = " << *v << "\n";

    v = ht.buscar("kiwi");
    if (!v) cout << "kiwi NO existe\n";
    cout << "\n";

    cout << "=== 4. Eliminar apple y banana ===\n";
    ht.eliminar("apple");
    ht.eliminar("banana");
    ht.imprimir();

    cout << "=== 5. cherry sigue accesible ===\n";
    v = ht.buscar("cherry");
    if (v) cout << "cherry = " << *v << "\n";

    return 0;
}