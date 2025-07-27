#include "arbol.h"
#include <iostream>
using namespace std;

int main() {
    ArbolMagico arbol;

    arbol.cargar_desde_csv();
    arbol.cargar_hechizos_csv();

    cout << "\n--- Prueba buscar_mago_por_id ---\n";
    int id_prueba = 1;
    Mago* buscado = arbol.buscar_mago_por_id(arbol.get_raiz(), id_prueba);
    if (buscado) {
        cout << "Mago encontrado: " << buscado->nombre << " " << buscado->apellido << endl;
    } else {
        cout << "No se encontro el mago con id " << id_prueba << endl;
    }

    cout << "\n--- Prueba buscar_mago_por_nombre ---\n";
    buscado = arbol.buscar_mago_por_nombre(arbol.get_raiz(), "Merlin", "Gales");
    if (buscado) {
        cout << "Mago encontrado por nombre: " << buscado->nombre << " " << buscado->apellido << endl;
    } else {
        cout << "No se encontro el mago por nombre." << endl;
    }

    cout << "\n--- Prueba mostrar_linea_sucesion ---\n";
    arbol.mostrar_linea_sucesion();

    cout << "\n--- Prueba mostrar_arbol ---\n";
    arbol.mostrar_arbol();

    cout << "\n--- Prueba modificar_mago (interactivo) ---\n";
    cout << "Ingrese el id de un mago para modificar: ";
    int id_mod;
    cin >> id_mod;
    cin.ignore();
    arbol.modificar_mago(id_mod, Mago());

    cout << "\n--- Prueba obtener_dueno_actual ---\n";
    Mago* dueno = arbol.obtener_dueno_actual();
    if (dueno) {
        cout << "Dueno actual: " << dueno->nombre << " " << dueno->apellido << endl;
    } else {
        cout << "No hay dueno actual." << endl;
    }

    cout << "\n--- Prueba mostrar_hechizos_mago ---\n";
    cout << "Ingrese el id de un mago para ver sus hechizos: ";
    int id_hech;
    cin >> id_hech;
    arbol.mostrar_hechizos_mago(id_hech);

    
cout << "\n--- Prueba agregar_hechizo ---\n";
cout << "Ingrese el id de un mago para agregarle un hechizo: ";
int id_agregar;
cin >> id_agregar;
cin.ignore();

hechizo nuevo_hechizo;
cout << "Nombre del hechizo: ";
getline(cin, nuevo_hechizo.nombre);
cout << "Poder del hechizo: ";
cin >> nuevo_hechizo.poder;
cin.ignore();

arbol.agregar_hechizo(id_agregar, nuevo_hechizo);
cout << "Hechizo agregado.\n";

    cout << "\n--- Prueba reasignar_hechizo ---\n";
    cout << "Ingrese el id del mago para reasignar un hechizo: ";
    int id_reasignar;
    cin >> id_reasignar;
    cin.ignore();

    hechizo hechizo_reasignar;
    cout << "Nombre del hechizo a reasignar: ";
    getline(cin, hechizo_reasignar.nombre);
    cout << "Poder del hechizo a reasignar: ";
    cin >> hechizo_reasignar.poder;

    arbol.reasignar_hechizo(id_reasignar, hechizo_reasignar);

arbol.mostrar_hechizos_mago(id_agregar);

    cout << "\n--- Guardando magos y hechizos en CSV ---\n";
    arbol.guardar_a_csv();
    arbol.guardar_hechizos_csv();
    cout << "Archivos CSV guardados.\n";

    arbol.liberar_arbol(arbol.get_raiz());
    cout << "Arbol liberado.\n";
    return 0;
}