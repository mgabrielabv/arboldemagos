#ifndef ARBOL_H
#define ARBOL_H

#include <fstream>
#include <string>
#include <iostream>
#include <stdexcept>

using namespace std;

struct hechizo {
    string nombre;
    int poder;
};

struct Mago {
    int id;
    string name;
    string last_name;
    char gender;
    int age;
    int id_father;
    bool is_dead;
    string type_magic;
    bool is_owner;
    hechizo hechizos[10];
    int num_hechizos;
    Mago* izquierdo;
    Mago* derecho;
    
    Mago() : id(0), gender(' '), age(0), id_father(0), 
             is_dead(false), is_owner(false), 
             num_hechizos(0), izquierdo(nullptr), derecho(nullptr) {}
};

class ArbolMagico {
public:
    ArbolMagico();
    ~ArbolMagico();
    
    void cargar_desde_csv();
    void guardar_a_csv() const;
    void cargar_hechizos_csv();
    void guardar_hechizos_csv() const;
    
    Mago* buscar_mago_por_id(Mago* nodo, int id) const;
    Mago* buscar_mago_por_nombre(Mago* nodo, const string& nombre, const string& apellido) const;
    Mago* buscar_padre(Mago* nodo, int id_padre) const;
    
    Mago* insertar_mago(Mago* nodo, Mago* nuevo);
    void modificar_mago(int id_mago, const Mago& datos);
    void agregar_hechizo(int id_mago, const hechizo& nuevo_hechizo);
    void reasignar_hechizo(int id_mago, const hechizo& hechizo);

    void mostrar_linea_sucesion() const;
    void mostrar_arbol() const;
    void mostrar_hechizos_mago(int id) const;
    void mostrar_arbol_genealogico() const;
    void mostrar_arbol_balanceado() const;

    static void liberar_arbol(Mago* nodo);
    Mago* get_raiz() const { return raiz; }
    Mago* obtener_dueno_actual() const;
    
private:
    Mago* raiz;
    string archivo_magos;
    string archivo_hechizos;
    
    Mago* crear_mago_desde_linea(const string& linea);
    void parsear_linea_hechizo(const string& linea, int& id_mago, hechizo& hechizo);
    void limpiar_hechizos_recursivo(Mago* nodo);
 
    void guardar_magos_recursivo(Mago* nodo, ofstream& archivo_m) const;
    void guardar_hechizos_recursivo(Mago* nodo, ofstream& archivo) const;
   
    bool es_magia_compatible(const string& tipo_magia) const;
    Mago* buscar_discipulo_magia(Mago* nodo, const string& magia1, const string& magia2) const;
    Mago* buscar_discipulo_mixed(Mago* nodo) const;
    Mago* buscar_primer_hombre_vivo(Mago* nodo) const;
    Mago* buscar_companero_discipulo(Mago* nodo) const;
    Mago* encontrar_mujer_mas_joven(Mago* nodo, Mago* mejor) const;
    Mago* buscar_maestro(Mago* discipulo) const;
    Mago* encontrar_sucesor(Mago* actual) const;
    Mago* buscar_dueno_recursivo(Mago* nodo) const;
    
    void mostrar_arbol_rec(Mago* nodo, int nivel) const;
    void mostrar_arbol_genealogico_rec(Mago* nodo, int nivel) const;
    
    int contar_magos(Mago* nodo) const;
    void recolectar_magos(Mago* nodo, Mago** magos, int& idx) const;
    void ordenar_por_edad(Mago** magos, int total) const;
    Mago* construir_balanceado(Mago** magos, int inicio, int fin) const;
};

#endif