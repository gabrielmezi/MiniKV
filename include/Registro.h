/*  include/Registro.h */
#pragma once
#include <bits/stdc++.h>

using namespace std;

enum class TipoDato {
    STRING,
    INT,
    FLOAT,
    BOOL
};

struct Registro {
    string clave;
    string valor;
    TipoDato tipo = TipoDato::STRING;
    bool estadoTTL = false;
    time_t expira = 0;
    unsigned long long versionTTL = 0;

    Registro(string c, string v, TipoDato t, bool ttl, time_t ex) {
        clave = c;
        valor = v;
        tipo = t;
        estadoTTL = ttl;
        expira = ex;
    }
};

