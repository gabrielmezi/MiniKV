#include "../include/MiniKV.h"

using namespace std;

MiniKV::MiniKV() {
}

bool MiniKV::set(string clave, string valor, TipoDato tipo) {
    Registro* existente = hash.buscar(clave);

    if (existente != nullptr) {
        existente->valor = valor;
        existente->tipo = tipo;
        return false;
    }

    Registro* nuevo = new Registro(clave, valor, tipo, false, 0);

    hash.insertar(nuevo);
    arbol.insertar(nuevo);

    return true;
}

Registro* MiniKV::get(string clave) {
    Registro* r = hash.buscar(clave);

    if (r == nullptr) return nullptr;

    if (r->estadoTTL && time(nullptr) >= r->expira) {
        del(clave);
        return nullptr;
    }

    return r;
}

bool MiniKV::del(string clave) {
    Registro* registro = hash.buscar(clave);

    if (registro == nullptr) {
        return false;
    }

    arbol.eliminar(clave);
    hash.eliminar(clave);

    delete registro;

    return true;
}

MiniKV::~MiniKV() {
    vector<Registro*> registros = arbol.dump(-1);

    for (Registro* r : registros) {
        delete r;
    }
}

bool MiniKV::exists(string clave) {
    return hash.buscar(clave) != nullptr;
}

vector<Registro*> MiniKV::range(string inicio, string fin, int limite) {
    return arbol.rango(inicio, fin, limite);
}

vector<Registro*> MiniKV::prefix(string p, int limite) {
    return arbol.prefijo(p, limite);
}

Registro* MiniKV::successor(string clave) {
    return arbol.sucesor(clave);
}

Registro* MiniKV::predecessor(string clave) {
    return arbol.predecesor(clave);
}

Registro* MiniKV::min() {
    return arbol.obtenerMinimo();
}

Registro* MiniKV::max() {
    return arbol.obtenerMaximo();
}

int MiniKV::count(string inicio, string fin) {
    return arbol.contarRango(inicio, fin);
}

vector<Registro*> MiniKV::dump(int limite) {
    return arbol.dump(limite);
}

bool MiniKV::expire(string clave, int segundos) {
    Registro* r = hash.buscar(clave);

    if (r == nullptr) return false;

    if (segundos <= 0) {
        del(clave);
        return true;
    }

    r->estadoTTL = true;
    r->expira = time(nullptr) + segundos;

    return true;
}