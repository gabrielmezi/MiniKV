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
    bool TieneTTL = false;
    time_t expiresAt = 0;
};