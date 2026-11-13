#include <bits/stdc++.h>
#include "../include/HashTable.h"
#include "../include/Registro.h"
#include "../include/ArbolAVL.h"
#include "../include/MiniKV.h"

using namespace std;

static bool leerLimite(stringstream& ss, int& limite) {
    string primero, numero, extra;
    if (!(ss >> primero)) return true;

    if (primero == "LIMIT") {
        if (!(ss >> numero)) return false;
    } else {
        numero = primero;
    }

    if (ss >> extra) return false;

    try {
        size_t pos = 0;
        int n = stoi(numero, &pos);
        if (pos != numero.size() || n < 0) return false;
        limite = n;
        return true;
    } catch (...) {
        return false;
    }
}

int main() {
    MiniKV kv;

    string linea;

    cout << "MINIKV"  << endl;
    cout << "Comando EXIT para salir" << endl;

    while (true) {
        cout << endl << "MiniKV> ";

        if (!getline(cin, linea)) {
            break;
        }

        stringstream ss(linea);
        string comando;

        ss >> comando;

        if (comando.empty()) {
            continue;
        }

        if (comando == "EXIT") {
            cout << "Terminado" << endl;
            break;
        }


        else if (comando == "SET") {
            string clave, valor, opcion;
            if (!(ss >> clave >> valor)) {
                cout << "Uso: SET clave valor [STRING|INT|FLOAT|BOOL] [EX segundos] [NX]" << endl;
                continue;
            }

            TipoDato tipo = TipoDato::STRING;
            bool tieneTipo = false;
            bool tieneEX = false;
            bool tieneNX = false;
            int segundos = 0;
            bool valido = true;

            while (ss >> opcion) {
                if (opcion == "NX") {
                    if (tieneNX) { valido = false; break; }
                    tieneNX = true;
                }
                else if (opcion == "EX") {
                    if (tieneEX) { valido = false; break; }
                    tieneEX = true;
                    string texto;
                    if (!(ss >> texto)) { valido = false; break; }
                    try {
                        size_t pos = 0;
                        segundos = stoi(texto, &pos);
                        if (pos != texto.size() || segundos <= 0) valido = false;
                    } catch (...) {
                        valido = false;
                    }
                    if (!valido) break;
                }
                else if (opcion == "STRING" || opcion == "INT" ||
                         opcion == "FLOAT" || opcion == "BOOL") {
                    if (tieneTipo) { valido = false; break; }
                    tieneTipo = true;
                    if (opcion == "INT") tipo = TipoDato::INT;
                    else if (opcion == "FLOAT") tipo = TipoDato::FLOAT;
                    else if (opcion == "BOOL") tipo = TipoDato::BOOL;
                }
                else {
                    valido = false;
                    break;
                }
            }

            if (!valido) {
                cout << "Uso: SET clave valor [tipo] [EX segundos] [NX]" << endl;
                continue;
            }
            if (!kv.validarTipo(valor, tipo)) {
                cout << "ERROR: valor incompatible con el tipo" << endl;
                continue;
            }
            if (tieneNX && kv.exists(clave)) {
                cout << "(nil)" << endl;
                continue;
            }

            if (tieneEX) {
                cout << (kv.setEX(clave, valor, segundos, tipo) ? "OK" : "ERROR") << endl;
            }
            else if (tieneNX) {
                cout << (kv.setNX(clave, valor, tipo) ? "OK" : "(nil)") << endl;
            }
            else {
                kv.set(clave, valor, tipo);
                cout << "OK" << endl;
            }
        }

        else if (comando == "GET") {
            string clave;
            if (!(ss >> clave)) {
                cout << "Uso: GET clave" << endl;
                continue;
            }
            Registro* r = kv.get(clave);
            if (r == nullptr) {
                cout << "(nil)" << endl;
            }
            else {
                cout << r->valor << endl;
            }
        }

        else if (comando == "DEL") {
            string clave;
            if (!(ss >> clave)) {
                cout << "Uso: DEL clave" << endl;
                continue;
            }
            cout << kv.del(clave) << endl;
        }

        else if (comando == "EXISTS") {
            string clave;

            if (!(ss >> clave)) {
                cout << "Uso: EXISTS clave" << endl;
                continue;
            }

            cout << kv.exists(clave) << endl;
        
        } else if (comando == "RANGE") {
            string inicio, fin;
            int limite = -1;
            if (!(ss >> inicio >> fin) || !leerLimite(ss, limite)) {
                cout << "Uso: RANGE inicio fin [LIMIT n]" << endl;
                continue;
            }

            vector<Registro*> resultados = kv.range(inicio, fin, limite);
            for (Registro* r : resultados) {
                cout << r->clave << ": " << r->valor << endl;
            }
            cout << "Total mostrado: " << resultados.size() << endl;
        }

        else if (comando == "PREFIX") {
            string prefijo;
            int limite = -1;
            if (!(ss >> prefijo) || !leerLimite(ss, limite)) {
                cout << "Uso: PREFIX prefijo [LIMIT n]" << endl;
                continue;
            }
            if (prefijo == "\"\"" || prefijo == "''") prefijo = "";

            vector<Registro*> resultados = kv.prefix(prefijo, limite);
            for (Registro* r : resultados) {
                cout << r->clave << ": " << r->valor << endl;
            }
            cout << "Total mostrado: " << resultados.size() << endl;
        }

        else if (comando == "COUNT") {
            string inicio, fin;

            if (!(ss >> inicio >> fin)) {
                cout << "Uso: COUNT inicio fin" << endl;
                continue;
            }

            cout << kv.count(inicio, fin) << endl;
        }

        else if (comando == "MIN") {
            Registro* r = kv.min();

            if (r != nullptr) {
                cout << r->clave << ": " << r->valor << endl;
            }
            else {
                cout << "(nil)" << endl;
            }
        }

        else if (comando == "MAX") {
            Registro* r = kv.max();

            if (r != nullptr) {
                cout << r->clave << ": " << r->valor << endl;
            }
            else {
                cout << "(nil)" << endl;
            }
        }

        else if (comando == "SUCCESSOR") {
            string clave;

            if (!(ss >> clave)) {
                cout << "Uso: SUCCESSOR clave" << endl;
                continue;
            }

            Registro* r = kv.successor(clave);

            if (r != nullptr) {
                cout << r->clave << ": " << r->valor << endl;
            }
            else {
                cout << "(nil)" << endl;
            }
        }

        else if (comando == "PREDECESSOR") {
            string clave;

            if (!(ss >> clave)) {
                cout << "Uso: PREDECESSOR clave" << endl;
                continue;
            }

            Registro* r = kv.predecessor(clave);

            if (r != nullptr) {
                cout << r->clave << ": " << r->valor << endl;
            }
            else {
                cout << "(nil)" << endl;
            }
        }

        else if (comando == "EXPIRE") {
            string clave, extra;
            int segundos;

            if (!(ss >> clave >> segundos) || (ss >>extra)) {
                cout << "USO: EXPIRE clave segundos" << endl;
                continue;
            }

            cout << kv.expire(clave, segundos) << endl;
        }

        else if (comando == "TTL") {
            string clave, extra;

            if (!(ss >> clave) || (ss >> extra)) {
                cout << "Uso: TTL clave" << endl;
                continue;
            }

            cout << kv.ttl(clave) << endl;
        } else if (comando == "DUMP") {
            int limite = 50;
            if (!leerLimite(ss, limite)) {
                cout << "Uso: DUMP [LIMIT n]" << endl;
                continue;
            }

            vector<Registro*> registros = kv.dump(limite);
            for (Registro* r : registros) {
                cout << r->clave << ": " << r->valor << endl;
            }
            cout << "Total mostrado: " << registros.size() << endl;
        }

        else if (comando == "STATS") {
            string extra;

            if (ss >> extra) {
                cout << "Uso: STATS" << endl;
                continue;
            }

            kv.stats();
        } else if (comando == "ALTURA") {
            cout << "Altura AVL: " << kv.alturaAVL() << endl;
        }

        else if (comando == "SAVE") {
            string archivo, extra;

            if (!(ss >> archivo) || (ss >> extra)) {
                cout << "Uso: SAVE archivo" << endl;
                continue;
            }

            if (kv.save(archivo)) {
                cout << "OK" << endl;
            }
            else {
                cout << "ERROR: no se pudo guardar" << endl;
            }
        }

        else if (comando == "LOAD") {
            string archivo, extra;

            if (!(ss >> archivo) || (ss >> extra)) {
                cout << "Uso: LOAD archivo" << endl;
                continue;
            }

            if (kv.load(archivo)) {
                cout << "OK" << endl;
            }
            else {
                cout << "ERROR: no se pudo cargar" << endl;
            }
        }

        else if (comando == "CSV") {
            string archivo, extra;
            if (!(ss >> archivo) || (ss >> extra)) {
                cout << "Uso: CSV archivo" << endl;
                continue;
            }

            auto inicioCarga = chrono::steady_clock::now();
            int cargados = kv.cargarCSV(archivo);
            auto finCarga = chrono::steady_clock::now();

            if (cargados == -1) {
                cout << "ERROR: no se pudo cargar el CSV" << endl;
            }
            else {
                double ms = chrono::duration<double, milli>(finCarga - inicioCarga).count();
                int hashDespues = kv.cantidadHash();
                int avlDespues = kv.cantidadAVL();
                cout << "Filas aceptadas: " << cargados << endl;
                cout << "Claves nuevas en HashTable: " << kv.clavesNuevasCSV() << endl;
                cout << "Claves nuevas en AVL: " << kv.clavesNuevasCSV() << endl;
                cout << "Total HashTable: " << hashDespues << endl;
                cout << "Total AVL: " << avlDespues << endl;
                cout << "Tiempo de carga: " << ms << " ms" << endl;
            }
        }

        else {
            cout << "Comando desconocido: " << comando << endl;
        }

       
    }
    return 0;
}

