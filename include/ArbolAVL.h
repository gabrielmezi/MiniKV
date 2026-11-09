#pragma once

#include <bits/stdc++.h>
#include "Registro.h"

using namespace std;

class ArbolAVL {
private:
    struct NodoAVL {
        Registro* registro;
        NodoAVL* izquierda;
        NodoAVL* derecha;
        int altura;
        int tamano;

        NodoAVL(Registro* r);
    };

    NodoAVL* raiz = nullptr;

    int obtenerAltura(NodoAVL* nodo);
    int obtenerTamano(NodoAVL* nodo);
    int factorBalance(NodoAVL* nodo);
    void actualizar(NodoAVL* nodo);

    NodoAVL* rotacionDerecha(NodoAVL* nodo);
    NodoAVL* rotacionIzquierda(NodoAVL* nodo);
    NodoAVL* rebalancear(NodoAVL* nodo);

    NodoAVL* insertar(NodoAVL* nodo, Registro* r);
    NodoAVL* eliminar(NodoAVL* nodo, string clave);

    void imprimirInOrden(NodoAVL* nodo);
    void liberar(NodoAVL* nodo);

    NodoAVL* minimo(NodoAVL* nodo);
    NodoAVL* maximo(NodoAVL* nodo);

    void rango(NodoAVL* nodo, string inicio, string fin, vector<Registro*>& resultados, int limite);

    void prefijo(NodoAVL* nodo, string p, vector<Registro*>& resultados, int limite);
public:
    ArbolAVL();
    ~ArbolAVL();

    void insertar(Registro* r);
    Registro* buscar(string clave);
    void eliminar(string clave);
    void imprimir();

    Registro* obtenerMinimo();
    Registro* obtenerMaximo();

    Registro* sucesor(string clave);
    Registro* predecesor(string clave);
    vector<Registro*> rango(string inicio, string fin, int limite = -1);
    vector<Registro*> prefijo(string p, int limite = -1);
};


