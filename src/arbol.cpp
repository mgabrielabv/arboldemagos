#include "arbol.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

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
    string tokens[9];
    
    while((pos = linea.find(',', start)) != string::npos || start < linea.size()) {
        string token;
        if (pos != string::npos) {
            token = linea.substr(start, pos - start);
            start = pos + 1;
        } else {
            token = linea.substr(start);
            start = linea.size();
        }
        if (campo < 9) tokens[campo] = token;
        campo++;
    }
  
    try {
        nuevo->id = stoi(tokens[0]); 
        nuevo->name = tokens[1]; 
        nuevo->last_name = tokens[2];
        nuevo->gender = tokens[3][0]; 
        nuevo->age = stoi(tokens[4]); 
        nuevo->id_father = stoi(tokens[5]); 
        nuevo->is_dead = (tokens[6] == "1");
        nuevo->type_magic = tokens[7]; 
        nuevo->is_owner = (tokens[8] == "1"); 
    } catch (...) {
        delete nuevo;
        throw runtime_error("Error parsing magician data");
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
        delete nuevo;
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
    
    auto normalizar = [](const string& s) {
        string result;
        for (char c : s) {
            if (!isspace(c)) {
                result += tolower(c);
            }
        }
        return result;
    };

    string nombreNormalizado = normalizar(nombre);
    string apellidoNormalizado = normalizar(apellido);
    string nodoNombreNormalizado = normalizar(nodo->name);
    string nodoApellidoNormalizado = normalizar(nodo->last_name);

    if (nodoNombreNormalizado == nombreNormalizado && 
        nodoApellidoNormalizado == apellidoNormalizado) {
        return nodo;
    }
    Mago* encontradoIzquierdo = buscar_mago_por_nombre(nodo->izquierdo, nombre, apellido);
    if (encontradoIzquierdo) return encontradoIzquierdo;
    
    return buscar_mago_por_nombre(nodo->derecho, nombre, apellido);
}
bool ArbolMagico::es_magia_compatible(const string& tipo_magia) const {
    return tipo_magia == "elemental" || tipo_magia == "unique" || tipo_magia == "mixed";
}

Mago* ArbolMagico::buscar_discipulo_magia(Mago* nodo, const string& magia1, const string& magia2) const {
    if (!nodo) return nullptr;
    if (!nodo->is_dead && (nodo->type_magic == magia1 || nodo->type_magic == magia2)) {
        return nodo;
    }
    Mago* encontrado = buscar_discipulo_magia(nodo->izquierdo, magia1, magia2);
    if (encontrado) return encontrado;
    return buscar_discipulo_magia(nodo->derecho, magia1, magia2);
}

Mago* ArbolMagico::buscar_discipulo_mixed(Mago* nodo) const {
    if (!nodo) return nullptr;
    if (!nodo->is_dead && nodo->type_magic == "mixed") {
        return nodo;
    }
    Mago* encontrado = buscar_discipulo_mixed(nodo->izquierdo);
    if (encontrado) return encontrado;
    return buscar_discipulo_mixed(nodo->derecho);
}

Mago* ArbolMagico::buscar_primer_hombre_vivo(Mago* nodo) const {
    if (!nodo) return nullptr;
    if (!nodo->is_dead && nodo->gender == 'H') {
        return nodo;
    }
    Mago* encontrado = buscar_primer_hombre_vivo(nodo->izquierdo);
    if (encontrado) return encontrado;
    return buscar_primer_hombre_vivo(nodo->derecho);
}

Mago* ArbolMagico::buscar_companero_discipulo(Mago* nodo) const {
    if (!nodo || nodo->id_father == 0) return nullptr;
    Mago* padre = buscar_mago_por_id(raiz, nodo->id_father);
    if (!padre) return nullptr;

    if (padre->izquierdo && padre->izquierdo->id != nodo->id && !padre->izquierdo->is_dead) {
        return padre->izquierdo;
    }
    if (padre->derecho && padre->derecho->id != nodo->id && !padre->derecho->is_dead) {
        return padre->derecho;
    }
    return nullptr;
}

Mago* ArbolMagico::encontrar_mujer_mas_joven(Mago* nodo, Mago* mejor) const {
    if (!nodo) return mejor;
    if (!nodo->is_dead && nodo->gender == 'M') {
        if (!mejor || nodo->age < mejor->age) {
            mejor = nodo;
        }
    }
    mejor = encontrar_mujer_mas_joven(nodo->izquierdo, mejor);
    mejor = encontrar_mujer_mas_joven(nodo->derecho, mejor);
    return mejor;
}

Mago* ArbolMagico::buscar_maestro(Mago* discipulo) const {
    if (!discipulo) return nullptr;
    return buscar_mago_por_id(raiz, discipulo->id_father);
}

Mago* ArbolMagico::encontrar_sucesor(Mago* actual) const {
    if (!actual) return nullptr;

    // 1. Si el dueño actual tiene discípulos
    if (actual->izquierdo || actual->derecho) {
        // Buscar primero discípulo con magia elemental o unique
        Mago* discipulo_magia = buscar_discipulo_magia(actual, "elemental", "unique");
        if (discipulo_magia) return discipulo_magia;
        
        // Si no, buscar discípulo con magia mixed
        Mago* discipulo_mixed = buscar_discipulo_mixed(actual);
        if (discipulo_mixed) return discipulo_mixed;
        
        // Si no, primer hombre vivo encontrado
        Mago* primer_hombre = buscar_primer_hombre_vivo(actual);
        if (primer_hombre) return primer_hombre;
    }

    // 2. Si el dueño murió y no tiene discípulos
    if (actual->is_dead && !actual->izquierdo && !actual->derecho) {
        // Buscar compañero discípulo
        Mago* companero = buscar_companero_discipulo(actual);
        
        if (companero && !companero->is_dead && 
            (companero->type_magic == actual->type_magic || 
             es_magia_compatible(companero->type_magic))) {
            return companero;
        }
        
        // Si el compañero tiene discípulos que cumplan las condiciones
        if (companero) {
            Mago* discipulo_companero = buscar_discipulo_magia(companero, "elemental", "unique");
            if (!discipulo_companero) discipulo_companero = buscar_discipulo_mixed(companero);
            if (discipulo_companero) return discipulo_companero;
        }
        
        // 3. Si el compañero no está vivo o no tiene hijos
        Mago* maestro = buscar_maestro(actual);
        if (maestro) {
            Mago* tio = buscar_companero_discipulo(maestro);
            if (tio) return tio;
        }
    }

    // 4. Si el dueño tiene más de 70 años
    if (actual->age > 70) {
        // Buscar discípulo con misma magia que el maestro
        Mago* discipulo_misma_magia = nullptr;
        Mago* discipulo_mayor_edad = nullptr;
        
        if (actual->izquierdo && !actual->izquierdo->is_dead) {
            if (actual->izquierdo->type_magic == actual->type_magic) {
                discipulo_misma_magia = actual->izquierdo;
            }
            discipulo_mayor_edad = actual->izquierdo;
        }
        
        if (actual->derecho && !actual->derecho->is_dead) {
            if (actual->derecho->type_magic == actual->type_magic) {
                if (!discipulo_misma_magia || actual->derecho->age > discipulo_misma_magia->age) {
                    discipulo_misma_magia = actual->derecho;
                }
            }
            if (!discipulo_mayor_edad || actual->derecho->age > discipulo_mayor_edad->age) {
                discipulo_mayor_edad = actual->derecho;
            }
        }
        
        if (discipulo_misma_magia) return discipulo_misma_magia;
        if (discipulo_mayor_edad) return discipulo_mayor_edad;
    }

    // 5. Buscar mujer más joven con discípulos vivos y maestros mixed
    Mago* mejor_mujer_con_discipulos = nullptr;
    Mago* mejor_mujer = nullptr;
    
    // Usamos recursión en lugar de stack
    encontrar_mejor_mujer_recursivo(raiz, mejor_mujer_con_discipulos, mejor_mujer, actual);
    
    if (mejor_mujer_con_discipulos) return mejor_mujer_con_discipulos;
    return mejor_mujer;
}

// Función auxiliar recursiva para encontrar la mejor mujer
void ArbolMagico::encontrar_mejor_mujer_recursivo(Mago* nodo, Mago*& mejor_con_discipulos, 
                                                 Mago*& mejor_mujer, Mago* actual) const {
    if (!nodo) return;
    
    // Procesar hijos primero
    encontrar_mejor_mujer_recursivo(nodo->izquierdo, mejor_con_discipulos, mejor_mujer, actual);
    encontrar_mejor_mujer_recursivo(nodo->derecho, mejor_con_discipulos, mejor_mujer, actual);
    
    // Verificar si es mujer viva y no es el dueño actual
    if (!nodo->is_dead && nodo->gender == 'M' && nodo->id != actual->id) {
        // Verificar si tiene discípulos vivos
        bool tiene_discipulos_vivos = (nodo->izquierdo && !nodo->izquierdo->is_dead) || 
                                     (nodo->derecho && !nodo->derecho->is_dead);
        
        // Verificar si sus maestros fueron poseedores del hechizo y tenían magia mixed
        bool maestro_cumple = false;
        Mago* maestro = buscar_maestro(nodo);
        if (maestro && maestro->type_magic == "mixed") {
            // Verificar si el maestro fue poseedor del hechizo
            // (asumimos que hay un historial de poseedores)
            maestro_cumple = true; // Simplificación
        }
        
        if (tiene_discipulos_vivos && maestro_cumple) {
            if (!mejor_con_discipulos || nodo->age < mejor_con_discipulos->age) {
                mejor_con_discipulos = nodo;
            }
        }
        
        // Guardar la mujer más joven en general
        if (!mejor_mujer || nodo->age < mejor_mujer->age) {
            mejor_mujer = nodo;
        }
    }
}

Mago* ArbolMagico::buscar_dueno_recursivo(Mago* nodo) const {
    if (!nodo) return nullptr;
    if (nodo->is_owner) return nodo;
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
    
    cout << "\n=== LINEA DE SUCESION ACTUAL ===\n";
    cout << "1. " << dueno->name << " " << dueno->last_name << " (Actual dueño)\n";

    Mago* sucesor = encontrar_sucesor(dueno);
    int pos = 2;
    int ultimo_id = dueno->id;
    
    while (sucesor && sucesor->id != ultimo_id) {
        cout << pos++ << ". " << sucesor->name << " " << sucesor->last_name << "\n";
        ultimo_id = sucesor->id;
        sucesor = encontrar_sucesor(sucesor);
    }
}

void ArbolMagico::cargar_desde_csv() {
    ifstream archivo(archivo_magos);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo de magos: " + archivo_magos);
    }

    string linea;
    getline(archivo, linea); 

    int total_magos = 0;
    while (getline(archivo, linea)) {
        total_magos++;
    }
    
    archivo.clear();
    archivo.seekg(0, ios::beg);
    getline(archivo, linea); 

    Mago** magos = new Mago*[total_magos];
    int contador = 0;
    
    while (getline(archivo, linea)) {
        try {
            magos[contador++] = crear_mago_desde_linea(linea);
        } catch (...) {
            for (int i = 0; i < contador; i++) {
                delete magos[i];
            }
            delete[] magos;
            archivo.close();
            throw;
        }
    }
    archivo.close();

    for (int i = 0; i < total_magos; ++i) {
        magos[i]->izquierdo = nullptr;
        magos[i]->derecho = nullptr;
    }

    raiz = nullptr;
    for (int i = 0; i < total_magos; ++i) {
        raiz = insertar_mago(raiz, magos[i]);
    }

    delete[] magos;
}

void ArbolMagico::guardar_magos_recursivo(Mago* nodo, ofstream& archivo_m) const {
    if (!nodo) return;
    guardar_magos_recursivo(nodo->izquierdo, archivo_m);
    
    archivo_m << nodo->id << ","
             << nodo->name << ","
             << nodo->last_name << ","
             << nodo->gender << ","
             << nodo->age << ","
             << nodo->id_father << ","
             << (nodo->is_dead ? "1" : "0") << ","
             << nodo->type_magic << ","
             << (nodo->is_owner ? "1" : "0") << "\n";

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
    cout << nodo->name << " " << nodo->last_name << " (ID:" << nodo->id;
    if (nodo->is_owner) cout << ", DUENO";
    cout << ")" << endl;
    
    mostrar_arbol_genealogico_rec(nodo->izquierdo, nivel + 1);
    mostrar_arbol_genealogico_rec(nodo->derecho, nivel);
}

void ArbolMagico::mostrar_arbol_rec(Mago* nodo, int nivel) const {
    if (!nodo) return;
    
    mostrar_arbol_rec(nodo->derecho, nivel + 1);
    
    for (int i = 0; i < nivel; ++i) cout << "    ";
    cout << nodo->name << " " << nodo->last_name << " (ID:" << nodo->id;
    if (nodo->is_owner) cout << ", DUENO";
    cout << ")" << endl;
    
    mostrar_arbol_rec(nodo->izquierdo, nivel + 1);
}

void ArbolMagico::mostrar_arbol_balanceado() const {
    cout << "\n=== ARBOL BALANCEADO DE MAGOS (POR EDAD) ===\n";
    
    int total = contar_magos(raiz);
    if (total == 0) {
        cout << "El arbol esta vacio.\n";
        return;
    }
    
    Mago** magos = new Mago*[total];
    int idx = 0;
    recolectar_magos(raiz, magos, idx);
    
    ordenar_por_edad(magos, total);
    
    Mago* balanceado = construir_balanceado(magos, 0, total - 1);
    mostrar_arbol_rec(balanceado, 0);
    
    delete[] magos;
    liberar_arbol(balanceado);
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
        for (int j = 0; j < total - i - 1; ++j) {
            if (magos[j]->age > magos[j+1]->age) {
                Mago* temp = magos[j];
                magos[j] = magos[j+1];
                magos[j+1] = temp;
            }
        }
    }
}

Mago* ArbolMagico::construir_balanceado(Mago** magos, int inicio, int fin) const {
    if (inicio > fin) return nullptr;
    
    int medio = (inicio + fin) / 2;
    Mago* nuevo = new Mago(*magos[medio]);
    
    nuevo->izquierdo = construir_balanceado(magos, inicio, medio - 1);
    nuevo->derecho = construir_balanceado(magos, medio + 1, fin);
    
    return nuevo;
}

void ArbolMagico::modificar_mago(int id_mago, const Mago& datos) {
    Mago* mago = buscar_mago_por_id(raiz, id_mago);
    if (!mago) {
        throw runtime_error("Mago no encontrado");
    }
    mago->name = datos.name;
    mago->last_name = datos.last_name;
    mago->gender = datos.gender;
    mago->age = datos.age;
    mago->is_dead = datos.is_dead;
    mago->type_magic = datos.type_magic;
    mago->is_owner = datos.is_owner;

    if (mago->is_owner && mago->is_dead) {
        for (int i = 0; i < mago->num_hechizos; ++i) {
            reasignar_hechizo(mago->id, mago->hechizos[i]);
        }
    }
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
            case 0: id_mago = stoi(token); break;
            case 1: hechizo.nombre = token; break;
            case 2: hechizo.poder = stoi(token); break;
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
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo de hechizos: " + archivo_hechizos);
    }

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
        throw runtime_error("Error al abrir " + archivo_magos);
    }
    
    archivo_m << "id,name,last_name,gender,age,id_father,is_dead,type_magic,is_owner\n";
    guardar_magos_recursivo(raiz, archivo_m);
    archivo_m.close();
}

void ArbolMagico::guardar_hechizos_csv() const {
    ofstream archivo(archivo_hechizos);
    if (!archivo.is_open()) {
        throw runtime_error("Error al abrir " + archivo_hechizos);
    }
    
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
        throw runtime_error("No hay dueño actual del hechizo");
    }

    if (!dueno_actual->is_dead && dueno_actual->age <= 70) {
        throw runtime_error("El dueño actual sigue siendo valido");
    }
    
    Mago* nuevo_dueno = encontrar_sucesor(dueno_actual);
    if (!nuevo_dueno) {
        throw runtime_error("No se pudo encontrar un sucesor adecuado");
    }
    
    int idx = -1;
    for (int i = 0; i < dueno_actual->num_hechizos; ++i) {
        if (dueno_actual->hechizos[i].nombre == hechizo.nombre && 
            dueno_actual->hechizos[i].poder == hechizo.poder) {
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
    
    dueno_actual->is_owner = false;
    nuevo_dueno->is_owner = true;

    bool ya_tiene = false;
    for (int i = 0; i < nuevo_dueno->num_hechizos; ++i) {
        if (nuevo_dueno->hechizos[i].nombre == hechizo.nombre && 
            nuevo_dueno->hechizos[i].poder == hechizo.poder) {
            ya_tiene = true;
            break;
        }
    }
    if (!ya_tiene && nuevo_dueno->num_hechizos < 10) {
        nuevo_dueno->hechizos[nuevo_dueno->num_hechizos] = hechizo;
        nuevo_dueno->num_hechizos++;
    }

    cout << "El nuevo dueño del hechizo es: " << nuevo_dueno->name << " "
         << nuevo_dueno->last_name << " (ID: " << nuevo_dueno->id << ")\n";
}

void ArbolMagico::mostrar_hechizos_mago(int id) const {
    Mago* mago = buscar_mago_por_id(raiz, id);
    if (!mago) {
        throw runtime_error("Mago no encontrado");
    }
    
    cout << "\n=== HECHIZOS DE " << mago->name << " " << mago->last_name << " ===\n";
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
    Mago* mago = buscar_mago_por_id(raiz, id_mago);
    if (!mago) {
        throw runtime_error("Mago no encontrado");
    }
    
    if (mago->num_hechizos >= 10) {
        throw runtime_error("Este mago ya tiene el máximo de hechizos (10)");
    }
    for (int i = 0; i < mago->num_hechizos; ++i) {
        if (mago->hechizos[i].nombre == nuevo_hechizo.nombre) {
            throw runtime_error("Este mago ya tiene un hechizo con ese nombre");
        }
    }
    
    mago->hechizos[mago->num_hechizos] = nuevo_hechizo;
    mago->num_hechizos++;
}

Mago* ArbolMagico::obtener_dueno_actual() const {
    return buscar_dueno_recursivo(raiz);
}