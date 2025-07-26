#include "arbol.h"
#include <iostream>
#include <sstream>

using namespace std;

ArbolMagico::ArbolMagico() : raiz(nullptr), archivo_magos("magos.csv") {}

ArbolMagico::~ArbolMagico() {
    liberar_arbol(raiz);
}

Mago* ArbolMagico::crear_mago_desde_linea(const string& linea) {
    Mago* nuevo = new Mago();
    size_t pos = 0;
    size_t start = 0;
    int campo = 0;

    while((pos = linea.find(',', start)) != string::npos || start < linea.size()) {
        string token;
        if (pos != string::npos) {
            token = linea.substr(start, pos - start);
            start = pos + 1;
        } else {
            token = linea.substr(start);
            start = linea.size();
        }

        switch (campo) {
            case 0: nuevo->id = stoi(token); break;
            case 1: nuevo->nombre = token; break;
            case 2: nuevo->apellido = token; break;
            case 3: nuevo->genero = token[0]; break;
            case 4: nuevo->edad = stoi(token); break;
            case 5: nuevo->id_padre = stoi(token); break;
            case 6: nuevo->esta_muerto = (token == "1"); break;
            case 7: nuevo->tipo_magia = token; break;
            case 8: nuevo->es_dueno = (token == "1"); break;
            default: break;
        }
        campo++;
    }
    return nuevo;
}

void ArbolMagico::liberar_arbol(Mago* nodo) {
    if (nodo) {
        liberar_arbol(nodo->izquierdo);
        liberar_arbol(nodo->derecho);
        delete nodo;
    }
}
Mago* ArbolMagico::insertar_mago(Mago* nodo, Mago* nuevo) {
    if (!nodo) return nuevo;

    if (nuevo->id_padre < nodo->id) {
        nodo->izquierdo = insertar_mago(nodo->izquierdo, nuevo);
    } else if (nuevo->id_padre > nodo->id) {
        nodo->derecho = insertar_mago(nodo->derecho, nuevo);
    } else {
        // discipulo
        if (!nodo->izquierdo) {
            nodo->izquierdo = nuevo;
        } else if (!nodo->derecho) {
            nodo->derecho = nuevo;
        }
    }
    
    return nodo;
}

Mago* ArbolMagico::buscar_mago_por_id(Mago* nodo, int id) const {
    if (!nodo) return nullptr;
    if (id == nodo->id) return nodo;
    if (id < nodo->id) return buscar_mago_por_id(nodo->izquierdo, id);
    return buscar_mago_por_id(nodo->derecho, id);
}

Mago* ArbolMagico::buscar_padre(Mago* nodo, int id_padre) const {
    if (!nodo) return nullptr;
    if (nodo->id == id_padre) return nodo;
    
    Mago* encontrado = buscar_padre(nodo->izquierdo, id_padre);
    if (encontrado) return encontrado;
    return buscar_padre(nodo->derecho, id_padre);
}

Mago* ArbolMagico::buscar_mago_por_nombre(Mago* nodo, const string& nombre, const string& apellido) const {
    if (!nodo) return nullptr;
    if (nodo->nombre == nombre && nodo->apellido == apellido) return nodo;
    
    Mago* encontrado = buscar_mago_por_nombre(nodo->izquierdo, nombre, apellido);
    if (encontrado) return encontrado;
    return buscar_mago_por_nombre(nodo->derecho, nombre, apellido);
}

bool ArbolMagico::es_magia_compatible(const string& tipo_magia) const {
    return tipo_magia == "elemental" || tipo_magia == "unique" || tipo_magia == "mixed";
}

Mago* ArbolMagico::buscar_discipulo_magia(Mago* nodo, const string& magia1, const string& magia2) const {
    if (!nodo) return nullptr;
    if (!nodo->esta_muerto && (nodo->tipo_magia == magia1 || nodo->tipo_magia == magia2)) {
        return nodo;
    }
    Mago* encontrado = buscar_discipulo_magia(nodo->izquierdo, magia1, magia2);
    if (encontrado) return encontrado;
    return buscar_discipulo_magia(nodo->derecho, magia1, magia2);
}

Mago* ArbolMagico::buscar_discipulo_mixed(Mago* nodo) const {
    if (!nodo) return nullptr;
    if (!nodo->esta_muerto && nodo->tipo_magia == "mixed") {
        return nodo;
    }
    Mago* encontrado = buscar_discipulo_mixed(nodo->izquierdo);
    if (encontrado) return encontrado;
    return buscar_discipulo_mixed(nodo->derecho);
}

Mago* ArbolMagico::buscar_primer_hombre_vivo(Mago* nodo) const {
    if (!nodo) return nullptr;
    if (!nodo->esta_muerto && nodo->genero == 'H') {
        return nodo;
    }
    Mago* encontrado = buscar_primer_hombre_vivo(nodo->izquierdo);
    if (encontrado) return encontrado;
    return buscar_primer_hombre_vivo(nodo->derecho);
}

Mago* ArbolMagico::buscar_companero_discipulo(Mago* nodo) const {
    if (!nodo || nodo->id_padre == 0) return nullptr;
    Mago* padre = buscar_mago_por_id(raiz, nodo->id_padre);
    if (!padre) return nullptr;
    if (padre->izquierdo && padre->izquierdo->id != nodo->id && !padre->izquierdo->esta_muerto) {
        return padre->izquierdo;
    }
    if (padre->derecho && padre->derecho->id != nodo->id && !padre->derecho->esta_muerto) {
        return padre->derecho;
    }
    return nullptr;
}

Mago* ArbolMagico::encontrar_mujer_mas_joven(Mago* nodo, Mago* mejor) const {
    if (!nodo) return mejor;
    if (!nodo->esta_muerto && nodo->genero == 'M') {
        if (!mejor || nodo->edad < mejor->edad) {
            mejor = nodo;
        }
    }
    mejor = encontrar_mujer_mas_joven(nodo->izquierdo, mejor);
    mejor = encontrar_mujer_mas_joven(nodo->derecho, mejor);
    return mejor;
}

Mago* ArbolMagico::buscar_maestro(Mago* discipulo) const {
    if (!discipulo) return nullptr;
    return buscar_mago_por_id(raiz, discipulo->id_padre);
}

Mago* ArbolMagico::encontrar_sucesor(Mago* actual) const {
    if (!actual) return nullptr;
    if (actual->edad > 70) {
        Mago* discipulo_misma_magia = nullptr;
        Mago* discipulo_mayor_edad = nullptr;
        if (actual->izquierdo && !actual->izquierdo->esta_muerto) {
            if (actual->izquierdo->tipo_magia == actual->tipo_magia) {
                discipulo_misma_magia = actual->izquierdo;
            }
            discipulo_mayor_edad = actual->izquierdo;
        }
        if (actual->derecho && !actual->derecho->esta_muerto) {
            if (actual->derecho->tipo_magia == actual->tipo_magia) {
                if (!discipulo_misma_magia || actual->derecho->edad > discipulo_misma_magia->edad) {
                    discipulo_misma_magia = actual->derecho;
                }
            }
            if (!discipulo_mayor_edad || actual->derecho->edad > discipulo_mayor_edad->edad) {
                discipulo_mayor_edad = actual->derecho;
            }
        }
        if (discipulo_misma_magia) return discipulo_misma_magia;
        return discipulo_mayor_edad;
    }
    Mago* sucesor = buscar_discipulo_magia(actual, "elemental", "unique");
    if (sucesor) return sucesor;
    sucesor = buscar_discipulo_mixed(actual);
    if (sucesor) return sucesor;
    sucesor = buscar_primer_hombre_vivo(actual);
    if (sucesor) return sucesor;
    Mago* companero = buscar_companero_discipulo(actual);
    if (companero && es_magia_compatible(companero->tipo_magia)) {
        return companero;
    }
    if (companero) {
        sucesor = buscar_discipulo_magia(companero, "elemental", "unique");
        if (!sucesor) sucesor = buscar_discipulo_mixed(companero);
        if (!sucesor) sucesor = buscar_primer_hombre_vivo(companero);
        if (sucesor) return sucesor;
    }
    Mago* maestro = buscar_maestro(actual);
    if (maestro && !maestro->esta_muerto) {
        Mago* tio = buscar_companero_discipulo(maestro);
        if (tio) return tio;
    }
    Mago* mejor_mujer = nullptr;
    mejor_mujer = encontrar_mujer_mas_joven(raiz, mejor_mujer);
    return mejor_mujer;
}

Mago* ArbolMagico::buscar_dueno_recursivo(Mago* nodo) const {
    if (!nodo) return nullptr;
    if (nodo->es_dueno) return nodo;
    Mago* encontrado = buscar_dueno_recursivo(nodo->izquierdo);
    if (encontrado) return encontrado;
    return buscar_dueno_recursivo(nodo->derecho);
}

void ArbolMagico::mostrar_linea_sucesion() const {
    Mago* dueno = buscar_dueno_recursivo(raiz);
    if (!dueno) {
        cout << "No hay dueño actual del hechizo.\n";
        return;
    }
    cout << "\nLINEA DE SUCESION ACTUAL\n";
    cout << "1. " << dueno->nombre << " " << dueno->apellido << " (Actual dueño)\n";
    Mago* sucesor = encontrar_sucesor(dueno);
    int pos = 2;
    while (sucesor) {
        cout << pos++ << ". " << sucesor->nombre << " " << sucesor->apellido << "\n";
        sucesor = encontrar_sucesor(sucesor);
    }
}

void ArbolMagico::cargar_desde_csv() {
    ifstream archivo(archivo_magos);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo de magos: " << archivo_magos << endl;
        return;
    }

    string linea;
    getline(archivo, linea); 

    while (getline(archivo, linea)) {
        Mago* nuevo = crear_mago_desde_linea(linea);
        if (!raiz && nuevo->id_padre == 0) {
            raiz = nuevo;
        } else {
            insertar_mago(raiz, nuevo);
        }
    }
    archivo.close();
}