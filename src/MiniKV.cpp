
#include "../include/MiniKV.h"

using namespace std;

MiniKV::MiniKV() {
}


bool MiniKV::set(string clave, string valor, TipoDato tipo) {
    Registro* existente = get(clave);

    if (existente != nullptr) {
        if (existente->tipo != tipo) {
            disminuirTipo(existente->tipo);
            aumentarTipo(tipo);
        }

        existente->valor = valor;
        existente->tipo = tipo;

        existente->estadoTTL = false;
        existente->expira = 0;
        existente->versionTTL = 0;

        return false;
    }

    Registro* nuevo = new Registro(clave, valor, tipo, false, 0);

    hash.insertar(nuevo);
    arbol.insertar(nuevo);

    aumentarTipo(tipo);

    return true;
}


bool MiniKV::setNX(string clave, string valor, TipoDato tipo) {
    if (get(clave) != nullptr) {
        return false;
    }

    set(clave, valor, tipo);
    return true;
}

bool MiniKV::setEX(string clave, string valor, int segundos, TipoDato tipo) {
    if (segundos <= 0) {
        return false;
    }

    set(clave, valor, tipo);
    return expire(clave, segundos);
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

    disminuirTipo(registro->tipo);

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
    return get(clave) != nullptr;
}

vector<Registro*> MiniKV::range(string inicio, string fin, int limite) {
    limpiarExpirados();
    return arbol.rango(inicio, fin, limite);
}

vector<Registro*> MiniKV::prefix(string p, int limite) {
    limpiarExpirados();
    return arbol.prefijo(p, limite);
}

Registro* MiniKV::successor(string clave) {
    limpiarExpirados();
    return arbol.sucesor(clave);
}

Registro* MiniKV::predecessor(string clave) {
    limpiarExpirados();
    return arbol.predecesor(clave);
}

Registro* MiniKV::min() {
    limpiarExpirados();
    return arbol.obtenerMinimo();
}

Registro* MiniKV::max() {
    limpiarExpirados();
    return arbol.obtenerMaximo();
}

int MiniKV::count(string inicio, string fin) {
    limpiarExpirados();
    return arbol.contarRango(inicio, fin);
}

vector<Registro*> MiniKV::dump(int limite) {
    limpiarExpirados();
    return arbol.dump(limite);
}


bool MiniKV::expire(string clave, int segundos) {
    Registro* r = get(clave);

    if (r == nullptr) return false;

    if (segundos <= 0) {
        del(clave);
        return true;
    }

    r->estadoTTL = true;
    r->expira = time(nullptr) + segundos;

    siguienteVersion++;
    r->versionTTL = siguienteVersion;

    EventoTTL evento;
    evento.clave = clave;
    evento.expira = r->expira;
    evento.version = r->versionTTL;

    heapTTL.insert(evento);

    return true;
}


int MiniKV::ttl(string clave) {
    Registro* r = get(clave);

    if (r == nullptr) return -2;

    if (!r->estadoTTL) return -1;

    time_t restante = r->expira - time(nullptr);

    if (restante <= 0) {
        del(clave);
        return -2;
    }

    return (int)restante;
}


void MiniKV::limpiarExpirados() {
    time_t ahora = time(nullptr);

    while (!heapTTL.empty() && heapTTL.getMin().expira <= ahora) {
        EventoTTL evento = heapTTL.extractMin();

        Registro* r = hash.buscar(evento.clave);

        if (r == nullptr) continue;

        if (r->estadoTTL &&
            r->versionTTL == evento.version &&
            r->expira == evento.expira) {

            del(evento.clave);
        }
    }
}

void MiniKV::stats() {
    limpiarExpirados();

    cout << "----- STATS -----" << endl;
    cout << "Elementos: " << hash.obtenerElementos() << endl;
    cout << "Capacidad HashTable: " << hash.obtenerCapacidad() << endl;
    cout << "Colisiones actuales: " << hash.contarColisiones() << endl;

    cout << "STRING: " << cantidadString << endl;
    cout << "INT: " << cantidadInt << endl;
    cout << "FLOAT: " << cantidadFloat << endl;
    cout << "BOOL: " << cantidadBool << endl;
}

void MiniKV::aumentarTipo(TipoDato tipo) {
    switch (tipo) {
        case TipoDato::STRING: cantidadString++; break;
        case TipoDato::INT: cantidadInt++; break;
        case TipoDato::FLOAT: cantidadFloat++; break;
        case TipoDato::BOOL: cantidadBool++; break;
    }
}

void MiniKV::disminuirTipo(TipoDato tipo) {
    switch (tipo) {
        case TipoDato::STRING: cantidadString--; break;
        case TipoDato::INT: cantidadInt--; break;
        case TipoDato::FLOAT: cantidadFloat--; break;
        case TipoDato::BOOL: cantidadBool--; break;
    }
}


bool MiniKV::save(string archivo) {
    limpiarExpirados();

    ofstream salida(archivo);

    if (!salida.is_open()) {
        return false;
    }

    vector<Registro*> registros = arbol.dump(-1);

    salida << "MINIKV1" << endl;
    salida << registros.size() << endl;

    for (Registro* r : registros) {
        salida << quoted(r->clave) << " "
               << quoted(r->valor) << " "
               << (int)r->tipo << " "
               << r->estadoTTL << " "
               << r->expira << endl;
    }

    salida.close();

    return salida.good();
}

bool MiniKV::load(string archivo) {
    ifstream entrada(archivo);

    if (!entrada.is_open()) return false;

    string cabecera;
    int cantidad;

    if (!(entrada >> cabecera >> cantidad)) return false;

    if (cabecera != "MINIKV1" || cantidad < 0)
        return false;

    struct DatoCarga {
        string clave;
        string valor;
        TipoDato tipo;
        bool tieneTTL;
        time_t expira;
    };

    vector<DatoCarga> datos;

    for (int i = 0; i < cantidad; i++) {
        string clave, valor;
        int tipo, ttl;
        long long expira;

        if (!(entrada >> quoted(clave)
                     >> quoted(valor)
                     >> tipo >> ttl >> expira)) {
            return false;
        }

        if (tipo < 0 || tipo > 3) return false;
        if (ttl != 0 && ttl != 1) return false;
        if (expira < 0) return false;
        if (ttl == 0 && expira != 0) return false;
        if (ttl == 1 && expira == 0) return false;

        if (!datos.empty() && clave <= datos.back().clave)
            return false;

        DatoCarga dato;

        dato.clave = clave;
        dato.valor = valor;
        dato.tipo = (TipoDato)tipo;
        dato.tieneTTL = (ttl == 1);
        dato.expira = (time_t)expira;

        if ((long long)dato.expira != expira)
            return false;

        datos.push_back(dato);
    }

    string extra;
    if (entrada >> extra) return false;
    if (entrada.bad()) return false;

    entrada.close();

    vector<Registro*> anteriores = arbol.dump(-1);
    vector<string> claves;

    for (Registro* r : anteriores) {
        claves.push_back(r->clave);
    }

    for (string clave : claves) {
        del(clave);
    }

    heapTTL = MinHeap();

    time_t ahora = time(nullptr);

    for (DatoCarga& dato : datos) {
        if (dato.tieneTTL && dato.expira <= ahora)
            continue;

        set(dato.clave, dato.valor, dato.tipo);

        if (dato.tieneTTL) {
            Registro* r = hash.buscar(dato.clave);

            r->estadoTTL = true;
            r->expira = dato.expira;

            siguienteVersion++;
            r->versionTTL = siguienteVersion;

            EventoTTL evento;
            evento.clave = dato.clave;
            evento.expira = dato.expira;
            evento.version = r->versionTTL;

            heapTTL.insert(evento);
        }
    }

    return true;
}
