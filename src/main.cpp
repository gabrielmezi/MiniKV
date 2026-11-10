/*  src/main.cpp */
#include <bits/stdc++.h>
#include "../include/HashTable.h"
#include "../include/Registro.h"
#include "../include/ArbolAVL.h"
#include "../include/MiniKV.h"

using namespace std;

int main() {
    MiniKV kv;

    kv.set("lenin", "profesor");
    kv.set("salazar", "imbecil");
    kv.set("chucky", "ure");

    Registro* r = kv.get("chucky");

    if (r != nullptr) {
        cout << "GET chucky: " << r->valor << endl;
    }

    kv.set("chucky", "actualizado");

    r = kv.get("chucky");

    if (r != nullptr) {
        cout << "GET actualizado: " << r->valor << endl;
    }

    kv.del("chucky");

    r = kv.get("chucky");

    if (r == nullptr) {
        cout << "Chucky fue eliminado" << endl;
    }


    kv.set("lenin", "profesor");
    kv.set("chucky", "ure");

    cout << "EXISTS lenin: " << kv.exists("lenin") << endl;
    cout << "EXISTS carlos: " << kv.exists("carlos") << endl;

    kv.del("lenin");

    cout << "EXISTS lenin despues de DEL: "
        << kv.exists("lenin") << endl;

    kv.set("ana", "estudiante");
    kv.set("carlos", "ingeniero");
    kv.set("pedro", "docente");

    cout << endl << "--- RANGE ---" << endl;

    vector<Registro*> resultados = kv.range("ana", "pedro");

    for (Registro* r : resultados) {
        cout << r->clave << ": " << r->valor << endl;
    }

    cout << endl << "--- COUNT ---" << endl;
    cout << kv.count("ana", "pedro") << endl;

    cout << endl << "--- MIN Y MAX ---" << endl;

    Registro* menor = kv.min();
    Registro* mayor = kv.max();

    if (menor) cout << "MIN: " << menor->clave << endl;
    if (mayor) cout << "MAX: " << mayor->clave << endl;

    cout << endl << "--- ELIMINACION ---" << endl;

    kv.del("carlos");

    cout << "EXISTS carlos: " << kv.exists("carlos") << endl;
    cout << "COUNT ana-pedro: " << kv.count("ana", "pedro") << endl;

    vector<Registro*> prueba = kv.range("ana", "pedro");

    for (Registro* r : prueba) {
        cout << r->clave << ": " << r->valor << endl;
    }

    cout << endl << "--- TTL ---" << endl;

    kv.set("temporal", "hola");
    kv.expire("temporal", 3);

    cout << "Antes: " << (kv.get("temporal") != nullptr) << endl;

    this_thread::sleep_for(chrono::seconds(4));

    cout << "Despues: " << (kv.get("temporal") != nullptr) << endl;
    cout << "EXISTS: " << kv.exists("temporal") << endl;

    return 0;
}

/*
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

    cout << "--- COUNT ---" << endl;
    cout << "COUNT chucky-salazar: "
     << avl.contarRango("chucky", "salazar") << endl;

    cout << "COUNT chucky-lenin: "
        << avl.contarRango("chucky", "lenin") << endl;

    cout << "COUNT a-b: "
        << avl.contarRango("a", "b") << endl;

    

    avl.eliminar("chucky");
    avl.imprimir();

    vector<Registro*> resultadosDump = avl.dump(2);

    cout << "---- DUMP ----" << endl;

    for (Registro* r : resultadosDump) {
        cout << r->clave << ": " << r->valor << endl;
    }

    delete r1;
    delete r2;
    delete r3;

    return 0;
}
*/


