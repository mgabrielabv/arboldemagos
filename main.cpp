#include "arbol.h"
#include <iostream>
using namespace std;

int main() {
    ArbolMagico arbol;
    arbol.cargar_desde_csv();

   
    int id_prueba = 1;
    Mago* buscado = arbol.buscar_mago_por_id(arbol.get_raiz(), id_prueba);
    if (buscado) {
        cout << "Mago encontrado: " << buscado->nombre << " " << buscado->apellido << endl;
    } else {
        cout << "No se encontro el mago con id " << id_prueba << endl;
    }


    buscado = arbol.buscar_mago_por_nombre(arbol.get_raiz(), "Merlin", "Gales");
    if (buscado) {
        cout << "Mago encontrado por nombre: " << buscado->nombre << " " << buscado->apellido << endl;
    } else {
        cout << "No se encontro el mago por nombre." << endl;
    }

    arbol.mostrar_linea_sucesion();

    arbol.liberar_arbol(arbol.get_raiz());
    return 0;
}
