#include "arbol.h"
#include <iostream>
using namespace std;

int main()
{
    ArbolMagico arbol;
    arbol.cargar_desde_csv();
    arbol.cargar_hechizos_csv();
    Mago nuevo_mago;
    int opcion;
    do
    {
        cout << "\n=== MENU ARBOL DE MAGOS ===\n";
        cout << "1. Ingresar nuevo mago\n";
        cout << "2. Buscar mago por ID\n";
        cout << "3. Buscar mago por nombre\n";
        cout << "4. Mostrar linea de sucesion\n";
        cout << "5. Mostrar arbol completo\n";
        cout << "6. Modificar mago\n";
        cout << "7. Mostrar hechizos de un mago\n";
        cout << "8. Agregar hechizo a un mago\n";
        cout << "9. Reasignar hechizo\n";
        cout << "10. Guardar magos y hechizos en CSV\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion)
        {
        case 1:
        {
            Mago *nuevo_mago = new Mago();
            cout << "Ingrese ID: ";
            cin >> nuevo_mago->id;
            cin.ignore();
            cout << "Ingrese Nombre: ";
            getline(cin, nuevo_mago->nombre);
            cout << "Ingrese Apellido: ";
            getline(cin, nuevo_mago->apellido);
            cout << "Ingrese Genero (H/M): ";
            cin >> nuevo_mago->genero;
            cout << "Ingrese Edad: ";
            cin >> nuevo_mago->edad;
            cout << "Tipo de magia: ";
            cin >> nuevo_mago->tipo_magia;
            nuevo_mago->num_hechizos = 0;
            nuevo_mago->esta_muerto = false;
            nuevo_mago->es_dueno = false;
            nuevo_mago->izquierdo = nullptr;
            nuevo_mago->derecho = nullptr;

            arbol.insertar_mago(arbol.get_raiz(), nuevo_mago); 
            cout << "Mago ingresado.\n";
            break;
        }
        case 2:
        {
            int id;
            cout << "Ingrese ID a buscar: ";
            cin >> id;
            Mago *buscado = arbol.buscar_mago_por_id(arbol.get_raiz(), id);
            if (buscado)
                cout << "Mago encontrado: " << buscado->nombre << " " << buscado->apellido << endl;
            else
                cout << "No se encontro el mago con id " << id << endl;
            break;
        }
        case 3:
        {
            string nombre, apellido;
            cout << "Ingrese nombre: ";
            cin >> nombre;
            cout << "Ingrese apellido: ";
            cin >> apellido;
            Mago *buscado = arbol.buscar_mago_por_nombre(arbol.get_raiz(), nombre, apellido);
            if (buscado)
                cout << "Mago encontrado: " << buscado->nombre << " " << buscado->apellido << endl;
            else
                cout << "No se encontro el mago por nombre." << endl;
            break;
        }
        case 4:
        {
            arbol.mostrar_linea_sucesion();
            break;
        }
        case 5:
        {
            arbol.mostrar_arbol();
            break;
        }
        case 6:
        {
            int id_mod;
            cout << "Ingrese el id de un mago para modificar: ";
            cin >> id_mod;
            cin.ignore();
            arbol.modificar_mago(id_mod, Mago());
            break;
        }
        case 7:
        {
            int id_hech;
            cout << "Ingrese el id de un mago para ver sus hechizos: ";
            cin >> id_hech;
            arbol.mostrar_hechizos_mago(id_hech);
            break;
        }
        case 8:
        {
            int id_agregar;
            cout << "Ingrese el id de un mago para agregarle un hechizo: ";
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
            arbol.mostrar_hechizos_mago(id_agregar);
            break;
        }
        case 9:
        {
            int id_reasignar;
            cout << "Ingrese el id del mago para reasignar un hechizo: ";
            cin >> id_reasignar;
            cin.ignore();
            hechizo hechizo_reasignar;
            cout << "Nombre del hechizo a reasignar: ";
            getline(cin, hechizo_reasignar.nombre);
            cout << "Poder del hechizo a reasignar: ";
            cin >> hechizo_reasignar.poder;
            cin.ignore();
            arbol.reasignar_hechizo(id_reasignar, hechizo_reasignar);
            break;
        }
        case 10:
        {
            arbol.guardar_a_csv();
            arbol.guardar_hechizos_csv();
            cout << "Archivos CSV guardados.\n";
            break;
        }
        case 0:
        {
            cout << "Saliendo...\n";
            break;
        }
        default:
            cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);

    arbol.liberar_arbol(arbol.get_raiz());
    cout << "Arbol liberado.\n";
    return 0;
}