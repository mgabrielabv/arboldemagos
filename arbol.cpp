#include "arbol.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

ArbolMagico::ArbolMagico() : raiz(nullptr), archivo_magos("magos.csv"), archivo_hechizos("hechizos.csv") {}

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

    if (nuevo->id < nodo->id) {
        nodo->izquierdo = insertar_mago(nodo->izquierdo, nuevo);
    } else if (nuevo->id > nodo->id) {
        nodo->derecho = insertar_mago(nodo->derecho, nuevo);
    } else {
        return nodo;
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
        cout << "No hay dueno actual del hechizo.\n";
        return;
    }
    cout << "\nLINEA DE SUCESION ACTUAL\n";
    cout << "1. " << dueno->nombre << " " << dueno->apellido << " (Actual dueno)\n";
    Mago* sucesor = encontrar_sucesor(dueno);
    int pos = 2;
    int ultimo_id = dueno->id;
    while (sucesor) {
        cout << pos++ << ". " << sucesor->nombre << " " << sucesor->apellido << "\n";
        if (sucesor->id == ultimo_id) break;
        ultimo_id = sucesor->id;
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

    int capacidad = 100;
    int total = 0;
    Mago** magos = new Mago*[capacidad];

    while (getline(archivo, linea)) {
        if (total == capacidad) {
            capacidad *= 2;
            Mago** temp = new Mago*[capacidad];
            for (int i = 0; i < total; ++i) temp[i] = magos[i];
            delete[] magos;
            magos = temp;
        }
        magos[total++] = crear_mago_desde_linea(linea);
    }
    archivo.close();

    for (int i = 0; i < total; ++i) {
        Mago* padre = magos[i];

        int hijos_count = 0;
        Mago* hijos[10]; 
        for (int j = 0; j < total; ++j) {
            if (magos[j]->id_padre == padre->id) {
                hijos[hijos_count++] = magos[j];
            }
        }
 
        for (int a = 0; a < hijos_count - 1; ++a) {
            for (int b = a + 1; b < hijos_count; ++b) {
                if (hijos[a]->edad > hijos[b]->edad) {
                    Mago* tmp = hijos[a];
                    hijos[a] = hijos[b];
                    hijos[b] = tmp;
                }
            }
        }
        for (int h = 0; h < hijos_count; ++h) {
            if (h == 0) padre->izquierdo = hijos[h];
            if (h > 0) hijos[h-1]->derecho = hijos[h];
        }
        if (padre->id_padre == 0) raiz = padre;
    }

    delete[] magos;

    cargar_hechizos_csv();
}

void ArbolMagico::guardar_magos_recursivo(Mago* nodo, ofstream& archivo_m) const {
    if (!nodo) return;
    guardar_magos_recursivo(nodo->izquierdo, archivo_m);
    archivo_m << nodo->id << "," 
             << nodo->nombre << "," 
             << nodo->apellido << "," 
             << nodo->genero << "," 
             << nodo->edad << "," 
             << nodo->id_padre << "," 
             << (nodo->esta_muerto ? "1" : "0") << "," 
             << nodo->tipo_magia << "," 
             << (nodo->es_dueno ? "1" : "0") << "\n";
    guardar_magos_recursivo(nodo->derecho, archivo_m);
}

void ArbolMagico::mostrar_arbol() const {
    mostrar_arbol_genealogico();
    mostrar_arbol_balanceado();
}

void ArbolMagico::mostrar_arbol_genealogico() const {
    cout << "\n=== ARBOL GENEALOGICO DE MAGOS ===\n";
    mostrar_arbol_genealogico_rec(raiz, 0);
}

void ArbolMagico::mostrar_arbol_genealogico_rec(Mago* nodo, int nivel) const {
    if (!nodo) return;
    for (int i = 0; i < nivel; ++i) cout << "    ";
    cout << nodo->nombre << " " << nodo->apellido << " (ID:" << nodo->id;
    if (nodo->es_dueno) cout << ", DUENO";
    cout << ")" << endl;
    mostrar_arbol_genealogico_rec(nodo->izquierdo, nivel + 1); 
    mostrar_arbol_genealogico_rec(nodo->derecho, nivel); 
}

void ArbolMagico::mostrar_arbol_rec(Mago* nodo, int nivel) const {
    if (!nodo) return;
    mostrar_arbol_rec(nodo->derecho, nivel + 1);
    for (int i = 0; i < nivel; ++i) cout << "    ";
    cout << nodo->nombre << " " << nodo->apellido << " (ID:" << nodo->id;
    if (nodo->es_dueno) cout << ", DUENO";
    cout << ")" << endl;
    mostrar_arbol_rec(nodo->izquierdo, nivel + 1);
}

void ArbolMagico::mostrar_arbol_balanceado() const {
    cout << "\n=== ARBOL BALANCEADO DE MAGOS (POR EDAD) ===\n";
    int total = contar_magos(raiz);
    Mago** magos = new Mago*[total];
    int idx = 0;
    recolectar_magos(raiz, magos, idx);
    ordenar_por_edad(magos, total);
    Mago* balanceado = construir_balanceado(magos, 0, total - 1);
    mostrar_arbol_rec(balanceado, 0);
    delete[] magos;
    ArbolMagico::liberar_arbol(balanceado);
}

int ArbolMagico::contar_magos(Mago* nodo) const {
    if (!nodo) return 0;
    return 1 + contar_magos(nodo->izquierdo) + contar_magos(nodo->derecho);
}

void ArbolMagico::recolectar_magos(Mago* nodo, Mago** magos, int& idx) const {
    if (!nodo) return;
    magos[idx++] = nodo;
    recolectar_magos(nodo->izquierdo, magos, idx);
    recolectar_magos(nodo->derecho, magos, idx);
}

void ArbolMagico::ordenar_por_edad(Mago** magos, int total) const {
    for (int i = 0; i < total - 1; ++i) {
        for (int j = i + 1; j < total; ++j) {
            if (magos[i]->edad > magos[j]->edad) {
                Mago* tmp = magos[i];
                magos[i] = magos[j];
                magos[j] = tmp;
            }
        }
    }
}

Mago* ArbolMagico::construir_balanceado(Mago** magos, int inicio, int fin) const {
    if (inicio > fin) return nullptr;
    int mid = (inicio + fin) / 2;
    Mago* nuevo = new Mago(*magos[mid]);
    nuevo->izquierdo = construir_balanceado(magos, inicio, mid - 1);
    nuevo->derecho = construir_balanceado(magos, mid + 1, fin);
    return nuevo;
}

void ArbolMagico::modificar_mago(int id_mago, const Mago& datos) {
    Mago* mago = buscar_mago_por_id(raiz, id_mago);
    if (!mago) {
        cout << "Mago no encontrado.\n";
        return;
    }
    cout << "\nEditando mago: " << mago->nombre << " " << mago->apellido << "\n";
    cout << "1. Nombre\n2. Apellido\n3. Genero\n4. Edad\n5. Estado\n6. Tipo de magia\n";
    cout << "Seleccione campo a editar: ";
    int opcion;
    cin >> opcion;
    cin.ignore();
    switch (opcion) {
        case 1: {
            cout << "Nuevo nombre: ";
            getline(cin, mago->nombre);
            break;
        }
        case 2: {
            cout << "Nuevo apellido: ";
            getline(cin, mago->apellido);
            break;
        }
        case 3: {
            cout << "Nuevo genero (H/M): ";
            cin >> mago->genero;
            cin.ignore();
            break;
        }
        case 4: {
            cout << "Nueva edad: ";
            cin >> mago->edad;
            cin.ignore();
            break;
        }
        case 5: {
            cout << "Esta muerto? (1=Si, 0=No): ";
            cin >> mago->esta_muerto;
            cin.ignore();
            break;
        }
        case 6: {
            cout << "Nuevo tipo de magia (elemental/unique/mixed/no_magic): ";
            getline(cin, mago->tipo_magia);
            break;
        }
        default:
            cout << "Opcion no valida.\n";
    }
    guardar_a_csv();
}

Mago* ArbolMagico::obtener_dueno_actual() const {
    return buscar_dueno_recursivo(raiz);
}

void ArbolMagico::parsear_linea_hechizo(const string& linea, int& id_mago, hechizo& hechizo) {
    size_t pos = 0;
    size_t start = 0;
    int campo = 0;

    while ((pos = linea.find(',', start)) != string::npos || start < linea.size()) {
        string token;
        if (pos != string::npos) {
            token = linea.substr(start, pos - start);
            start = pos + 1;
        } else {
            token = linea.substr(start);
            start = linea.size();
        }
        switch (campo) {
            case 0:
                if (!token.empty())
                    id_mago = stoi(token);
                break;
            case 1:
                hechizo.nombre = token;
                break;
            case 2:
                if (!token.empty())
                    hechizo.poder = stoi(token);
                break;
            default:
                break;
        }
        campo++;
    }
}

void ArbolMagico::limpiar_hechizos_recursivo(Mago* nodo) {
    if (!nodo) return;
    nodo->num_hechizos = 0;
    limpiar_hechizos_recursivo(nodo->izquierdo);
    limpiar_hechizos_recursivo(nodo->derecho);
}

void ArbolMagico::cargar_hechizos_csv() {
    ifstream archivo(archivo_hechizos);
    if (!archivo.is_open()) return;

    limpiar_hechizos_recursivo(raiz); 

    string linea;
    getline(archivo, linea); 

    while (getline(archivo, linea)) {
        int id_mago;
        hechizo nuevo_hechizo;
        parsear_linea_hechizo(linea, id_mago, nuevo_hechizo);
        Mago* mago = buscar_mago_por_id(raiz, id_mago);
        if (mago && mago->num_hechizos < 10) {
            mago->hechizos[mago->num_hechizos] = nuevo_hechizo;
            mago->num_hechizos++;
        }
    }
    archivo.close();
}

void ArbolMagico::guardar_a_csv() const {
    ofstream archivo_m(archivo_magos);
    if (!archivo_m.is_open()) {
        cerr << "Error al abrir " << archivo_magos << endl;
        return;
    }
    archivo_m << "id,nombre,apellido,genero,edad,id_padre,esta_muerto,tipo_magia,es_dueno\n";
    guardar_magos_recursivo(raiz, archivo_m);
    archivo_m.close();
}

void ArbolMagico::guardar_hechizos_csv() const {
    ofstream archivo(archivo_hechizos, ios::out | ios::trunc);
    if (!archivo.is_open()) return;
    archivo << "id_mago,nombre_hechizo,poder\n";
    guardar_hechizos_recursivo(raiz, archivo);
    archivo.close();
}

void ArbolMagico::guardar_hechizos_recursivo(Mago* nodo, ofstream& archivo) const {
    if (!nodo) return;
    for (int i = 0; i < nodo->num_hechizos; ++i) {
        archivo << nodo->id << ","
                << nodo->hechizos[i].nombre << ","
                << nodo->hechizos[i].poder << "\n";
    }
    guardar_hechizos_recursivo(nodo->izquierdo, archivo);
    guardar_hechizos_recursivo(nodo->derecho, archivo);
}

void ArbolMagico::reasignar_hechizo(int id_mago, const hechizo& hechizo) {
    Mago* dueno_actual = buscar_mago_por_id(raiz, id_mago);
    if (!dueno_actual) {
        cout << "No hay dueño actual del hechizo.\n";
        return;
    }
    if (!dueno_actual->esta_muerto && dueno_actual->edad <= 70) {
        cout << "El dueño actual sigue siendo válido.\n";
        return;
    }
    Mago* nuevo_dueno = encontrar_sucesor(dueno_actual);
    if (!nuevo_dueno) {
        cout << "No se pudo encontrar un sucesor adecuado.\n";
        return;
    }

    int idx = -1;
    for (int i = 0; i < dueno_actual->num_hechizos; ++i) {
        if (dueno_actual->hechizos[i].nombre == hechizo.nombre && dueno_actual->hechizos[i].poder == hechizo.poder) {
            idx = i;
            break;
        }
    }
    if (idx != -1) {
        for (int j = idx; j < dueno_actual->num_hechizos - 1; ++j) {
            dueno_actual->hechizos[j] = dueno_actual->hechizos[j + 1];
        }
        dueno_actual->num_hechizos--;
    }
    dueno_actual->es_dueno = false;
    nuevo_dueno->es_dueno = true;
    bool ya_tiene = false;
    for (int i = 0; i < nuevo_dueno->num_hechizos; ++i) {
        if (nuevo_dueno->hechizos[i].nombre == hechizo.nombre && nuevo_dueno->hechizos[i].poder == hechizo.poder) {
            ya_tiene = true;
            break;
        }
    }
    if (!ya_tiene && nuevo_dueno->num_hechizos < 10) {
        nuevo_dueno->hechizos[nuevo_dueno->num_hechizos] = hechizo;
        nuevo_dueno->num_hechizos++;
    }
    cout << "El nuevo dueño del hechizo es: " << nuevo_dueno->nombre << " "
         << nuevo_dueno->apellido << " (ID: " << nuevo_dueno->id << ")\n";
    guardar_a_csv();
}

void ArbolMagico::mostrar_hechizos_mago(int id) const {
    Mago* mago = buscar_mago_por_id(raiz, id);
    if (!mago) {
        cout << "Mago no encontrado.\n";
        return;
    }
    
    cout << "\n=== HECHIZOS DE " << mago->nombre << " " << mago->apellido << " ===\n";
    if (mago->num_hechizos == 0) {
        cout << "Este mago no tiene hechizos registrados.\n";
        return;
    }
    
    for (int i = 0; i < mago->num_hechizos; ++i) {
        cout << i+1 << ". " << mago->hechizos[i].nombre 
             << " (Poder: " << mago->hechizos[i].poder << ")\n";
    }
}

void ArbolMagico::agregar_hechizo(int id_mago, const hechizo& nuevo_hechizo) {

    cargar_hechizos_csv();

    Mago* mago = buscar_mago_por_id(raiz, id_mago);
    if (!mago) {
        cout << "Mago no encontrado.\n";
        return;
    }
    if (mago->num_hechizos >= 10) {
        cout << "Este mago ya tiene el máximo de hechizos.\n";
        return;
    }

    mago->hechizos[mago->num_hechizos] = nuevo_hechizo;
    mago->num_hechizos++;
    cout << "Hechizo agregado con éxito.\n";

    guardar_hechizos_csv();

}