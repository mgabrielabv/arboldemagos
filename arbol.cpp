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