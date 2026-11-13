
#include <bits/stdc++.h>
#include "../include/MiniKV.h"
using namespace std;
using namespace std::chrono;

volatile int comprobacion = 0;

void medirGET(MiniKV& kv, string nombre, string clave) {
    const int repeticiones = 10000;

    auto inicio = steady_clock::now();

    for (int i = 0; i < repeticiones; i++) {
        Registro* r = kv.get(clave);
        comprobacion += (r != nullptr);
    }

    auto fin = steady_clock::now();

    double tiempo = duration<double, milli>(fin - inicio).count();
    double promedio = tiempo / repeticiones;

    Registro* resultado = kv.get(clave);

    cout << left << setw(15) << nombre
         << setw(15) << clave
         << setw(18) << (resultado ? resultado->valor : "(nil)")
         << fixed << setprecision(6) << promedio << endl;
}

int main() {
    MiniKV kv;

    int cargados = kv.cargarCSV("kv_masivo.csv");

    if (cargados != 10000) {
        cout << "ERROR: No se cargaron 10000 registros" << endl;
        return 1;
    }

    cout << "\nGET con 10000 registros" << endl;
    cout << "Repeticiones por caso: 10000\n" << endl;

    cout << left << setw(15) << "Caso"
         << setw(15) << "Clave"
         << setw(18) << "Respuesta"
         << "Promedio (ms)" << endl;

    medirGET(kv, "Primera", "kv000001");
    medirGET(kv, "Intermedia", "kv005000");
    medirGET(kv, "Ultima", "kv010000");
    medirGET(kv, "Inexistente", "inexistente");

    return 0;
}
