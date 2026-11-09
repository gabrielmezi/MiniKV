#include "../include/ArbolAVL.h"

using namespace std;

ArbolAVL::NodoAVL::NodoAVL(Registro* r) {
    registro = r;
    izquierda = nullptr;
    derecha = nullptr;
    altura = 1;
    tamano = 1;
}

ArbolAVL::ArbolAVL() {
    raiz = nullptr;
}

int ArbolAVL::obtenerAltura(NodoAVL* nodo) {
    if (nodo == nullptr) return 0;
    return nodo->altura;
}

int ArbolAVL::obtenerTamano(NodoAVL* nodo) {
    if (nodo == nullptr) return 0;
    return nodo->tamano;
}

int ArbolAVL::factorBalance(NodoAVL* nodo) {
    if (nodo == nullptr) return 0;

    return obtenerAltura(nodo->izquierda) - obtenerAltura(nodo->derecha);
}

void ArbolAVL::actualizar(NodoAVL* nodo) {
    if (nodo == nullptr) return;

    nodo->altura = 1 + max(
        obtenerAltura(nodo->izquierda),
        obtenerAltura(nodo->derecha)
    );

    nodo->tamano = 1 + obtenerTamano(nodo->izquierda)
                    + obtenerTamano(nodo->derecha);
    
}

ArbolAVL::NodoAVL* ArbolAVL::rotacionDerecha(NodoAVL* nodo) {
    NodoAVL* nuevaRaiz = nodo->izquierda;
    NodoAVL* temporal = nuevaRaiz->derecha;

    nuevaRaiz->derecha = nodo;
    nodo->izquierda = temporal;

    actualizar(nodo);
    actualizar(nuevaRaiz);

    return nuevaRaiz;
}

ArbolAVL::NodoAVL* ArbolAVL::rotacionIzquierda(NodoAVL* nodo) {
    NodoAVL* nuevaRaiz = nodo->derecha;
    NodoAVL* temporal = nuevaRaiz->izquierda;

    nuevaRaiz->izquierda = nodo;
    nodo->derecha = temporal;

    actualizar(nodo);
    actualizar(nuevaRaiz);

    return nuevaRaiz;
}

ArbolAVL::NodoAVL* ArbolAVL::rebalancear(NodoAVL* nodo) {
    if (nodo == nullptr) return nullptr;

    actualizar(nodo);
    int balance = factorBalance(nodo);

    if (balance > 1) {
        if (factorBalance(nodo->izquierda) >= 0) {
            return rotacionDerecha(nodo);
        } else {
            nodo->izquierda = rotacionIzquierda(nodo->izquierda);
            return rotacionDerecha(nodo);
        }
    }

    if (balance < -1) {
        if (factorBalance(nodo->derecha) <= 0) {
            return rotacionIzquierda(nodo);
        } else {
            nodo->derecha = rotacionDerecha(nodo->derecha);
            return rotacionIzquierda(nodo);
        }
    }

    return nodo;
}

ArbolAVL::NodoAVL* ArbolAVL::insertar(NodoAVL* nodo, Registro* r) {
    if (nodo == nullptr) {
        return new NodoAVL(r);
    }

    if (r->clave < nodo->registro->clave) {
        nodo->izquierda = insertar(nodo->izquierda, r);
    } else if (r->clave > nodo->registro->clave) {
        nodo->derecha = insertar(nodo->derecha, r);
    } else {
        return nodo;
    }

    return rebalancear(nodo);
}

void ArbolAVL::insertar(Registro* r) {
    if (r == nullptr) return;
    raiz = insertar(raiz, r);
}

void ArbolAVL::imprimirInOrden(NodoAVL* nodo) {
    if (nodo == nullptr) return;
    imprimirInOrden(nodo->izquierda);

    cout << "(\"" << nodo->registro->clave
        << "\"," << nodo->registro->valor << ")" << endl;

    imprimirInOrden(nodo->derecha);
}

void ArbolAVL::imprimir() {
    cout << "---- Arbol AVL ----" << endl;
    imprimirInOrden(raiz);
    cout << endl;
}

void ArbolAVL::liberar(NodoAVL* nodo) {
    if (nodo == nullptr) return;

    liberar(nodo->izquierda);
    liberar(nodo->derecha);

    delete nodo;
}

ArbolAVL::~ArbolAVL() {
    liberar(raiz);
}

Registro* ArbolAVL::buscar(string clave) {
    NodoAVL* actual = raiz;

    while (actual != nullptr) {
        if (clave == actual->registro->clave) {
            return actual->registro;
        } 
        else if (clave < actual->registro->clave) {
            actual = actual->izquierda;
        } 
        else {
            actual = actual->derecha;
        }
    }

    return nullptr;
}

ArbolAVL::NodoAVL* ArbolAVL::minimo(NodoAVL* nodo) {
    while (nodo->izquierda != nullptr) {
        nodo = nodo->izquierda;
    }
    return nodo;
}


ArbolAVL::NodoAVL* ArbolAVL::eliminar(NodoAVL* nodo, string clave) {
    if (nodo == nullptr) {
        return nullptr;
    }

    if (clave < nodo->registro->clave) {
        nodo->izquierda = eliminar(nodo->izquierda, clave);
    }
    else if (clave > nodo->registro->clave) {
        nodo->derecha = eliminar(nodo->derecha, clave);
    }
    else {
        if (nodo->izquierda == nullptr) {
            NodoAVL* temp = nodo->derecha;
            delete nodo;
            return temp;
        }
        else if (nodo->derecha == nullptr) {
            NodoAVL* temp = nodo->izquierda;
            delete nodo;
            return temp;
        }
        else {
            NodoAVL* sucesor = minimo(nodo->derecha);
            nodo->registro = sucesor->registro;
            nodo->derecha = eliminar(nodo->derecha, sucesor->registro->clave);
        }
    }

    return rebalancear(nodo);
}

void ArbolAVL::eliminar(string clave) {
    raiz = eliminar(raiz, clave);
}