#include "arbol.h"
#include <iostream>
#include <limits>

using namespace std;

void mostrarMenu() {
    cout << "\n=== MENU DEL ARBOL MAGICO ===" << endl;
    cout << "1. Mostrar arbol" << endl;
    cout << "2. Mostrar arbol balanceado por edad" << endl;
    cout << "3. Mostrar linea de sucesion" << endl;
    cout << "4. Mostrar hechizos de un mago" << endl;
    cout << "5. Agregar hechizo a un mago" << endl;
    cout << "6. Modificar datos de un mago" << endl;
    cout << "7. Simular muerte del dueño actual" << endl;
    cout << "8. Mostrar dueño actual" << endl;
    cout << "9. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pausa() {
    cout << "\nPresione Enter para continuar...";
    limpiarBuffer();
}

int main() {
    ArbolMagico arbol;
    
    try {
        arbol.cargar_desde_csv();
        arbol.cargar_hechizos_csv();
        
        cout << "=== SISTEMA DE HECHIZOS MAGICOS ===" << endl;
        cout << "Datos cargados correctamente desde los archivos CSV." << endl;
        
        Mago* dueno = arbol.obtener_dueno_actual();
        if (dueno) {
            cout << "\nDueño actual del hechizo: " << dueno->name << " " << dueno->last_name 
                 << " (ID: " << dueno->id << ")" << endl;
        }
        
        int opcion;
        do {
            mostrarMenu();
            cin >> opcion;
            limpiarBuffer();
            
            try {
                switch(opcion) {
                    case 1: {
                        arbol.mostrar_arbol_genealogico();
                        pausa();
                        break;
                    }
                    case 2: {
                        arbol.mostrar_arbol_balanceado();
                        pausa();
                        break;
                    }
                    case 3: {
                        arbol.mostrar_linea_sucesion();
                        pausa();
                        break;
                    }
                    case 4: {
                        int id;
                        cout << "Ingrese el ID del mago: ";
                        cin >> id;
                        limpiarBuffer();
                        arbol.mostrar_hechizos_mago(id);
                        pausa();
                        break;
                    }
                    case 5: {
                        int id;
                        string nombre;
                        int poder;
                        
                        cout << "Ingrese el ID del mago: ";
                        cin >> id;
                        limpiarBuffer();
                        
                        cout << "Nombre del hechizo: ";
                        getline(cin, nombre);
                        
                        cout << "Poder del hechizo: ";
                        cin >> poder;
                        limpiarBuffer();
                        
                        hechizo nuevo = {nombre, poder};
                        arbol.agregar_hechizo(id, nuevo);
                        cout << "Hechizo agregado correctamente." << endl;
                        pausa();
                        break;
                    }
                    case 6: {
                        int id;
                        cout << "Ingrese el ID del mago a modificar: ";
                        cin >> id;
                        limpiarBuffer();
                        Mago* mago = arbol.buscar_mago_por_id(arbol.obtener_raiz(), id);
                        if (!mago) {
                            cout << "Mago no encontrado." << endl;
                            pausa();
                            break;
                        }
                        Mago datos = *mago;
                        cout << "Nuevo nombre (" << datos.name << "): ";
                        getline(cin, datos.name);
                        cout << "Nuevo apellido (" << datos.last_name << "): ";
                        getline(cin, datos.last_name);
                        cout << "Nuevo genero (H/M) (" << datos.gender << "): ";
                        cin >> datos.gender;
                        limpiarBuffer();
                        cout << "Nueva edad (" << datos.age << "): ";
                        cin >> datos.age;
                        limpiarBuffer();
                        string temp;
                        cout << "Esta muerto? (1/0) (" << datos.is_dead << "): ";
                        cin >> temp;
                        datos.is_dead = (temp == "1");
                        limpiarBuffer();
                        cout << "Tipo de magia (elemental/unique/mixed/no_magic) (" << datos.type_magic << "): ";
                        getline(cin, datos.type_magic);
                        cout << "Es dueño? (1/0) (" << datos.is_owner << "): ";
                        cin >> temp;
                        datos.is_owner = (temp == "1");
                        limpiarBuffer();
                        arbol.modificar_mago(id, datos);
                        cout << "Mago modificado correctamente." << endl;
                        pausa();
                        break;
                    }
                    case 7: {
                        Mago* dueno = arbol.obtener_dueno_actual();
                        if (!dueno) {
                            cout << "No hay dueño actual." << endl;
                            pausa();
                            break;
                        }
                        
                        cout << "Simulando muerte del dueño actual: " 
                             << dueno->name << " " << dueno->last_name << endl;
                             
                        Mago datos = *dueno;
                        datos.is_dead = true;
                        datos.is_owner = false;
                        
                        arbol.modificar_mago(dueno->id, datos);
                        
                        for (int i = 0; i < dueno->num_hechizos; ++i) {
                            arbol.reasignar_hechizo(dueno->id, dueno->hechizos[i]);
                        }
                        
                        cout << "Hechizos reasignados al nuevo dueño." << endl;
                        pausa();
                        break;
                    }
                    case 8: {
                        Mago* dueno = arbol.obtener_dueno_actual();
                        if (dueno) {
                            cout << "\nDueño actual del hechizo: " << dueno->name << " " << dueno->last_name 
                                 << " (ID: " << dueno->id << ")" << endl;
                            cout << "Edad: " << dueno->age << endl;
                            cout << "Tipo de magia: " << dueno->type_magic << endl;
                            cout << "Hechizos: " << dueno->num_hechizos << endl;
                        } else {
                            cout << "No hay dueño actual del hechizo." << endl;
                        }
                        pausa();
                        break;
                    }
                    case 9: {
                        arbol.guardar_a_csv();
                        arbol.guardar_hechizos_csv();
                        cout << "Datos guardados. Saliendo del programa..." << endl;
                        break;
                    }
                    default: {
                        cout << "Opcion no valida. Intente de nuevo." << endl;
                        pausa();
                        break;
                    }
                }
            } catch (const exception& e) {
                cout << "\nError: " << e.what() << endl;
                pausa();
            }
            
        } while (opcion != 9);
        
    } catch (const exception& e) {
        cerr << "Error al cargar datos iniciales: " << e.what() << endl;
        system("pause");
        return 1;
    }
    
    return 0;
}