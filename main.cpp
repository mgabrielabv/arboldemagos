#include "arbol.h"
#include <iostream>
using namespace std;

int main() {
    ArbolMagico arbol;
    arbol.cargar_desde_csv();

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

    cout << "\n--- Prueba guardar_a_csv ---\n";
    arbol.guardar_a_csv();
    cout << "Archivo CSV guardado.\n";

    cout << "\n--- Prueba obtener_dueno_actual ---\n";
    Mago* dueno = arbol.obtener_dueno_actual();
    if (dueno) {
        cout << "Dueno actual: " << dueno->nombre << " " << dueno->apellido << endl;
    } else {
        cout << "No hay dueno actual." << endl;
    }

    arbol.liberar_arbol(arbol.get_raiz());
    return 0;
}
