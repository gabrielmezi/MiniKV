
#include <bits/stdc++.h>
#include "../include/MiniKV.h"
using namespace std;
using namespace std::chrono;

volatile unsigned long long comprobacion = 0;

string claveNumero(int i) {
    stringstream ss;
    ss << "kv" << setw(6) << setfill('0') << i;
    return ss.str();
}

template <typename F>
double medir(int repeticiones, F operacion) {
    auto inicio = steady_clock::now();
    for (int i = 0; i < repeticiones; i++) operacion(i);
    auto fin = steady_clock::now();
    return duration<double, milli>(fin - inicio).count() / repeticiones;
}

void mostrar(string nombre, int repeticiones, double ms) {
    cout << left << setw(30) << nombre
         << right << setw(12) << repeticiones
         << setw(18) << fixed << setprecision(6) << ms << endl;
}

void ejecutar(string csv, int n, string etiqueta) {
    MiniKV kv;
    int operaciones = min(100, n);

    string primera = claveNumero(1);
    string mitad = claveNumero(n / 2);
    string ultima = claveNumero(n);
    string estrechoFin = claveNumero(n / 2 + 9);
    string amplioFin = claveNumero(n / 10);
    string archivoGuardado = "prueba_guardado_" + etiqueta + ".txt";

    cout << "\nRegistros: " << n << endl;
    cout << left << setw(30) << "Operacion"
         << right << setw(12) << "Repeticiones"
         << setw(18) << "Promedio (ms)" << endl;

    double carga = medir(1, [&](int) {
        int filas = kv.cargarCSV(csv);
        if (filas != n) {
            cerr << "ERROR: CSV esperaba " << n << " filas, recibio " << filas << endl;
            exit(1);
        }
    });

    mostrar("Carga CSV completa", 1, carga);

    if (kv.count(primera, ultima) != n) {
        cerr << "ERROR: conteo luego de cargar" << endl;
        exit(1);
    }

    const int rep = 10000;

    mostrar("GET", rep, medir(rep, [&](int i) {
        Registro* r = kv.get(claveNumero(1 + (i % n)));
        comprobacion += (r != nullptr);
    }));

    mostrar("RANGE estrecho (10)", 1000, medir(1000, [&](int) {
        auto registros = kv.range(mitad, estrechoFin);
        comprobacion += registros.size();
    }));

    mostrar("RANGE amplio (10%)", 20, medir(20, [&](int) {
        auto registros = kv.range(primera, amplioFin);
        comprobacion += registros.size();
    }));

    mostrar("RANGE vacio", 1000, medir(1000, [&](int) {
        auto registros = kv.range("zzzzzz", "zzzzzzz");
        comprobacion += registros.size();
    }));

    mostrar("PREFIX kv000", 200, medir(200, [&](int) {
        auto registros = kv.prefix("kv000");
        comprobacion += registros.size();
    }));

    mostrar("SUCCESSOR", rep, medir(rep, [&](int i) {
        Registro* r = kv.successor(claveNumero(1 + (i % (n - 1))));
        comprobacion += (r != nullptr);
    }));

    mostrar("PREDECESSOR", rep, medir(rep, [&](int i) {
        Registro* r = kv.predecessor(claveNumero(2 + (i % (n - 1))));
        comprobacion += (r != nullptr);
    }));

    mostrar("COUNT rango", rep, medir(rep, [&](int) {
        comprobacion += kv.count(primera, amplioFin);
    }));

    mostrar("SAVE", 3, medir(3, [&](int) {
        if (!kv.save(archivoGuardado)) {
            cerr << "ERROR en SAVE" << endl;
            exit(1);
        }
    }));

    mostrar("LOAD", 3, medir(3, [&](int) {
        if (!kv.load(archivoGuardado)) {
            cerr << "ERROR en LOAD" << endl;
            exit(1);
        }
    }));

    mostrar("SET nuevas", operaciones, medir(operaciones, [&](int i) {
        bool nueva = kv.set("benchmark_nueva_" + to_string(i), "nuevo");
        comprobacion += nueva;
    }));

    mostrar("Actualizacion SET", operaciones, medir(operaciones, [&](int i) {
        kv.set(claveNumero(i + 1), "actualizado");
        comprobacion += (kv.get(claveNumero(i + 1)) != nullptr);
    }));

    mostrar("DEL", operaciones, medir(operaciones, [&](int i) {
        comprobacion += kv.del(claveNumero(n - i));
    }));

    if (kv.count(primera, ultima) != n - operaciones) {
        cerr << "ERROR: conteo final inesperado" << endl;
        exit(1);
    }

    cout << "Conteo final: OK" << endl;
    remove(archivoGuardado.c_str());
}

int main() {
    ejecutar("kv_pequeno.csv", 30, "30");
    ejecutar("kv_masivo.csv", 10000, "10k");
    ejecutar("kv_muy_masivo.csv", 100000, "100k");

    cout << "Comprobacion: " << comprobacion << endl;
    return 0;
}
