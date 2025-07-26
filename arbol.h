#ifndef Arbol_h
#define Arbol_h

#include <fstream>
#include <string>
using namespace std;

struct hechizo {
    string nombre;
    int poder;
};

struct Mago {
    int id;
    string nombre;
    string apellido;
    char genero;
    int edad;
    int id_padre;
    bool esta_muerto;
    string tipo_magia;
    bool es_dueno;
    hechizo hechizos[10];
    int num_hechizos;
    Mago* izquierdo;
    Mago* derecho;
    Mago() : id(0), genero(' '), edad(0), id_padre(0), esta_muerto(false), 
             es_dueno(false), num_hechizos(0), izquierdo(nullptr), derecho(nullptr) {}
};
class ArbolMagico {
public:
    ArbolMagico();
    ~ArbolMagico();

    Mago* crear_mago_desde_linea(const string& linea);
    void liberar_arbol(Mago* nodo);
    Mago* insertar_mago(Mago* nodo, Mago* nuevo);
    Mago* buscar_mago_por_id(Mago* nodo, int id) const;
    Mago* buscar_padre(Mago* nodo, int id_padre) const;
    Mago* buscar_mago_por_nombre(Mago* nodo, const string& nombre, const string& apellido) const;
    bool es_magia_compatible(const string& tipo_magia) const;
    Mago* buscar_discipulo_magia(Mago* nodo, const string& magia1, const string& magia2) const;
    Mago* buscar_discipulo_mixed(Mago* nodo) const;
    Mago* buscar_primer_hombre_vivo(Mago* nodo) const;
    Mago* buscar_companero_discipulo(Mago* nodo) const;
    Mago* encontrar_mujer_mas_joven(Mago* nodo, Mago* mejor) const;
    Mago* buscar_maestro(Mago* discipulo) const;
    Mago* encontrar_sucesor(Mago* actual) const;

    void cargar_desde_csv();
    void guardar_a_csv() const;

    void mostrar_linea_sucesion() const;
    
    Mago* get_raiz() const { return raiz; }

private:
    Mago* raiz;
    string archivo_magos;
    string archivo_hechizos;

    void parsear_linea_hechizo(const string& linea, int& id_mago, hechizo& hechizo);
    void guardar_hechizos_recursivo(Mago* nodo, std::ofstream& archivo) const;
    void guardar_magos_recursivo(Mago* nodo, std::ofstream& archivo_m) const;
    Mago* buscar_dueno_recursivo(Mago* nodo) const;
};

#endif

