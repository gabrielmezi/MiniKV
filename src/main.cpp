#include <bits/stdc++.h>
#include "../include/HashTable.h"
#include "../include/Registro.h"
#include "../include/ArbolAVL.h"
#include "../include/MiniKV.h"

using namespace std;

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
            cout << "Borrando C:/Windows/System32" << endl;
            break;
        }


        else if (comando == "SET") {
            string clave, valor;
            string opcion, extra;

            if (!(ss >> clave >> valor)) {
                cout << "Uso: SET clave valor [NX | EX segundos | tipo]" << endl;
                continue;
            }

            if (!(ss >> opcion)) {
                kv.set(clave, valor);
                cout << "OK" << endl;
            }
            else if (opcion == "NX") {
                if (ss >> extra) {
                    cout << "Demasiados argumentos" << endl;
                    continue;
                }

                if (kv.setNX(clave, valor)) {
                    cout << "OK" << endl;
                }
                else {
                    cout << "(nil)" << endl;
                }
            }
            else if (opcion == "EX") {
                string textoSegundos;

                if (!(ss >> textoSegundos) || (ss >> extra)) {
                    cout << "Uso: SET clave valor EX segundos" << endl;
                    continue;
                }

                int segundos;

                try {
                    size_t pos = 0;
                    segundos = stoi(textoSegundos, &pos);

                    if (pos != textoSegundos.size() || segundos <= 0) {
                        cout << "Segundos invalidos" << endl;
                        continue;
                    }
                }
                catch (...) {
                    cout << "Segundos invalidos" << endl;
                    continue;
                }

                if (kv.setEX(clave, valor, segundos)) {
                    cout << "OK" << endl;
                }
                else {
                    cout << "ERROR" << endl;
                }
            }
            else {
                TipoDato tipo;

                if (opcion == "STRING") {
                    tipo = TipoDato::STRING;
                }
                else if (opcion == "INT") {
                    tipo = TipoDato::INT;
                }
                else if (opcion == "FLOAT") {
                    tipo = TipoDato::FLOAT;
                }
                else if (opcion == "BOOL") {
                    tipo = TipoDato::BOOL;
                }
                else {
                    cout << "Opcion o tipo invalido" << endl;
                    continue;
                }

                if (ss >> extra) {
                    cout << "Demasiados argumentos" << endl;
                    continue;
                }

                if (!kv.validarTipo(valor, tipo)) {
                    cout << "ERROR: valor incompatible con el tipo" << endl;
                    continue;
                }

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

            if (!(ss >> inicio >> fin)) {
                cout << "Uso: RANGE inicio fin [limite]" << endl;
                continue;
            }

            if (ss >> limite && limite < 0) {
                cout << "Limite invalido" << endl;
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

            if (!(ss >> prefijo)) {
                cout << "Uso: PREFIX prefijo [limite]" << endl;
                continue;
            }

            if (ss >> limite && limite < 0) {
                cout << "Limite invalido" << endl;
                continue;
            }

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
            string argumento, extra;
            int limite = 50;

            if (ss >> argumento) {
                try {
                    size_t pos = 0;
                    limite = stoi(argumento, &pos);

                    if (pos != argumento.size() || limite < 0) {
                        cout << "Limite invalido" << endl;
                        continue;
                    }
                }
                catch (...) {
                    cout << "Limite invalido" << endl;
                    continue;
                }
            }

            if (ss >> extra) {
                cout << "Demasiados argumentos" << endl;
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

            int cargados = kv.cargarCSV(archivo);

            if (cargados == -1) {
                cout << "ERROR: no se pudo cargar el CSV" << endl;
            }
            else {
                cout << "Filas cargadas: " << cargados << endl;
            }
        }

        else {
            cout << "Comando desconocido: " << comando << endl;
        }

       
    }
    return 0;
}
