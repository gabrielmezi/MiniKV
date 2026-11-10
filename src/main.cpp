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

    cout << endl << "--- PRUEBA TTL ---" << endl;

    kv.set("usuario", "Gabriel");

    cout << "TTL sin EXPIRE: " << kv.ttl("usuario") << endl;

    kv.expire("usuario", 5);

    cout << "TTL inicial: " << kv.ttl("usuario") << endl;

    this_thread::sleep_for(chrono::seconds(2));

    cout << "TTL despues de 2 segundos: "
        << kv.ttl("usuario") << endl;

    this_thread::sleep_for(chrono::seconds(4));

    cout << "TTL despues de expirar: "
        << kv.ttl("usuario") << endl;

    cout << "TTL inexistente: "
        << kv.ttl("cualquierclave") << endl;

    cout << endl << "--- TTL CON AVL ---" << endl;

    kv.set("temporal2", "prueba");
    kv.expire("temporal2", 2);

    this_thread::sleep_for(chrono::seconds(3));

    cout << "COUNT temporal2: "
        << kv.count("temporal2", "temporal2") << endl;

    cout << "EXISTS temporal2: "
        << kv.exists("temporal2") << endl;
    

    cout << endl << "-- MINHEAP TTL ---" << endl;

    kv.set("clave1", "hola");
    kv.set("clave2", "mundo");

    kv.expire("clave1", 2);
    kv.expire("clave2", 5);

    cout << "Antes: " << kv.count("clave1", "clave2") << endl;

    this_thread::sleep_for(chrono::seconds(3));

    cout << "Despues de 3 segundos: "
        << kv.count("clave1", "clave2") << endl;

    cout << "EXISTS clave1: " << kv.exists("clave1") << endl;
    cout << "EXISTS clave2: " << kv.exists("clave2") << endl;

    cout << endl << "--- prueba ttl 2 ---" << endl;

    kv.set("renovable", "dato");

    kv.expire("renovable", 1);
    kv.expire("renovable", 10);

    this_thread::sleep_for(chrono::seconds(2));

    cout << "EXISTS renovable: "
        << kv.exists("renovable") << endl;

    cout << "TTL renovable: "
        << kv.ttl("renovable") << endl;

    cout << endl << "--- SET NX ---" << endl;

    cout << "Primera insercion: "
        << kv.setNX("curso", "EDA2") << endl;

    cout << "Segunda insercion: "
        << kv.setNX("curso", "Programacion") << endl;

    cout << "Valor final: " << kv.get("curso")->valor << endl;


    cout << endl << "--- SET EX ---" << endl;

    cout << "SET EX: "
        << kv.setEX("sesion", "activa", 2) << endl;

    cout << "TTL inicial: " << kv.ttl("sesion") << endl;

    this_thread::sleep_for(chrono::seconds(3));

    cout << "EXISTS sesion: "
        << kv.exists("sesion") << endl;
    
    cout << endl << "--- prueba hash colisiones ---" << endl;

    HashTable pruebaHash(5);

    cout << "Elementos: " << pruebaHash.obtenerElementos() << endl;
    cout << "Capacidad: " << pruebaHash.obtenerCapacidad() << endl;
    cout << "Colisiones: " << pruebaHash.contarColisiones() << endl;
    
    cout << endl << "--- stats ---" << endl;

    kv.set("edad", "20", TipoDato::INT);
    kv.set("promedio", "16.5", TipoDato::FLOAT);
    kv.set("activo", "true", TipoDato::BOOL);
    kv.set("nombre", "Gabriel", TipoDato::STRING);

    kv.stats();

    cout << endl << "--- CAMBIO DE TIPOS ---" << endl;

    kv.set("edad", "veinte", TipoDato::STRING);
    kv.del("promedio");

    kv.stats();

    cout << endl << "--- PRUEBA DE COLISIONES ---" << endl;

    HashTable pruebaHash2(2);

    Registro* a = new Registro("ana", "1", TipoDato::STRING, false, 0);
    Registro* b = new Registro("carlos", "2", TipoDato::STRING, false, 0);
    Registro* c = new Registro("lenin", "3", TipoDato::STRING, false, 0);

    pruebaHash2.insertar(a);
    pruebaHash2.insertar(b);
    pruebaHash2.insertar(c);

    cout << "Elementos: " << pruebaHash2.obtenerElementos() << endl;
    cout << "Capacidad: " << pruebaHash2.obtenerCapacidad() << endl;
    cout << "Colisiones: " << pruebaHash2.contarColisiones() << endl;

    pruebaHash2.imprimir();

    pruebaHash2.eliminar("carlos");

    cout << "Colisiones despues de eliminar: "
        << pruebaHash2.contarColisiones() << endl;

    delete a;
    delete b;
    delete c;

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


