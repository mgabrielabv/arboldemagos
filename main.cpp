#include "arbol.h"
#include <iostream>
#include <limits>

using namespace std;

void mostrar_menu() {
    cout << "\n=== MENU ARBOL DE MAGOS ===" << endl;
    cout << "1. Ingresar nuevo mago" << endl;
    cout << "2. Buscar mago por ID" << endl;
    cout << "3. Buscar mago por nombre" << endl;
    cout << "4. Mostrar linea de sucesion" << endl;
    cout << "5. Mostrar arbol completo" << endl;
    cout << "6. Modificar mago" << endl;
    cout << "7. Mostrar hechizos de un mago" << endl;
    cout << "8. Agregar hechizo a un mago" << endl;
    cout << "9. Reasignar hechizo" << endl;
    cout << "10. Guardar magos y hechizos en CSV" << endl;
    cout << "0. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

int main() {
    ArbolMagico arbol;
    
    try {
        arbol.cargar_desde_csv();
        arbol.cargar_hechizos_csv();
    } catch (const exception& e) {
        cerr << "Error al cargar datos iniciales: " << e.what() << endl;
        return 1;
    }

    int opcion;
    do {
        mostrar_menu();
        
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada invalida. Por favor ingrese un numero." << endl;
            continue;
        }
        cin.ignore();

        try {
            switch (opcion) {
                case 1: {
                    Mago* nuevo = new Mago();
                    
                    cout << "Ingrese ID: ";
                    if (!(cin >> nuevo->id)) throw runtime_error("ID debe ser un numero");
                    cin.ignore();
                    
                    cout << "Ingrese Nombre: ";
                    getline(cin, nuevo->name);
                    if (nuevo->name.empty()) throw runtime_error("Nombre no puede estar vacio");
                    
                    cout << "Ingrese Apellido: ";
                    getline(cin, nuevo->last_name);
                    if (nuevo->last_name.empty()) throw runtime_error("Apellido no puede estar vacio");
                    
                    cout << "Ingrese Genero (H/M): ";
                    cin >> nuevo->gender;
                    if (nuevo->gender != 'H' && nuevo->gender != 'M') throw runtime_error("Genero debe ser H o M");
                    
                    cout << "Ingrese Edad: ";
                    if (!(cin >> nuevo->age) || nuevo->age < 0) throw runtime_error("Edad debe ser un numero positivo");
                    
                    cout << "ID del padre: ";
                    if (!(cin >> nuevo->id_father) || nuevo->id_father < 0) throw runtime_error("ID del padre debe ser positivo");
                    cin.ignore();
                    
                    cout << "Tipo de magia (elemental/unique/mixed/no_magic): ";
                    getline(cin, nuevo->type_magic);
                    if (nuevo->type_magic != "elemental" && nuevo->type_magic != "unique" && 
                        nuevo->type_magic != "mixed" && nuevo->type_magic != "no_magic") {
                        throw runtime_error("Tipo de magia invalido");
                    }
                    
                    nuevo->is_dead = false;
                    nuevo->is_owner = false;
                    nuevo->num_hechizos = 0;
                    nuevo->izquierdo = nullptr;
                    nuevo->derecho = nullptr;
                    
                    arbol.insertar_mago(arbol.get_raiz(), nuevo);
                    cout << "Mago ingresado correctamente." << endl;
                    break;
                }
                case 2: {
                    int id;
                    cout << "Ingrese ID a buscar: ";
                    if (!(cin >> id)) throw runtime_error("ID debe ser un numero");
                    
                    Mago* buscado = arbol.buscar_mago_por_id(arbol.get_raiz(), id);
                    if (buscado) {
                        cout << "\n=== INFORMACION DEL MAGO ===" << endl;
                        cout << "ID: " << buscado->id << endl;
                        cout << "Nombre: " << buscado->name << " " << buscado->last_name << endl;
                        cout << "Genero: " << buscado->gender << endl;
                        cout << "Edad: " << buscado->age << endl;
                        cout << "ID Padre: " << buscado->id_father << endl;
                        cout << "Estado: " << (buscado->is_dead ? "Muerto" : "Vivo") << endl;
                        cout << "Tipo de magia: " << buscado->type_magic << endl;
                        cout << "Dueño del hechizo: " << (buscado->is_owner ? "Si" : "No") << endl;
                    } else {
                        cout << "No se encontro el mago con id " << id << endl;
                    }
                    break;
                }
                case 3: {
                    string nombre, apellido;
                    cout << "Ingrese nombre: ";
                    getline(cin, nombre);
                    cout << "Ingrese apellido: ";
                    getline(cin, apellido);
                    
                    Mago* buscado = arbol.buscar_mago_por_nombre(arbol.get_raiz(), nombre, apellido);
                    if (buscado) {
                        cout << "\n=== INFORMACION DEL MAGO ===" << endl;
                        cout << "ID: " << buscado->id << endl;
                        cout << "Nombre: " << buscado->name << " " << buscado->last_name << endl;
                        cout << "Genero: " << buscado->gender << endl;
                        cout << "Edad: " << buscado->age << endl;
                        cout << "ID Padre: " << buscado->id_father << endl;
                        cout << "Estado: " << (buscado->is_dead ? "Muerto" : "Vivo") << endl;
                        cout << "Tipo de magia: " << buscado->type_magic << endl;
                        cout << "Dueño del hechizo: " << (buscado->is_owner ? "Si" : "No") << endl;
                    } else {
                        cout << "No se encontro el mago por nombre." << endl;
                    }
                    break;
                }
                case 4:
                    arbol.mostrar_linea_sucesion();
                    break;
                case 5:
                    arbol.mostrar_arbol();
                    break;
                case 6: {
                    int id;
                    cout << "Ingrese ID del mago a modificar: ";
                    if (!(cin >> id)) throw runtime_error("ID debe ser un numero");
                    cin.ignore();
                    
                    Mago* mago = arbol.buscar_mago_por_id(arbol.get_raiz(), id);
                    if (!mago) {
                        cout << "Mago no encontrado." << endl;
                        break;
                    }
                    
                    Mago datos = *mago;

                    cout << "\nEditando mago: " << mago->name << " " << mago->last_name << endl;
                    cout << "1. Nombre" << endl;
                    cout << "2. Apellido" << endl;
                    cout << "3. Genero" << endl;
                    cout << "4. Edad" << endl;
                    cout << "5. Estado" << endl;
                    cout << "6. Tipo de magia" << endl;
                    cout << "Seleccione campo a editar: ";
                    
                    int campo;
                    if (!(cin >> campo) || campo < 1 || campo > 6) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Opcion no valida." << endl;
                        break;
                    }
                    cin.ignore();
                    
                    switch (campo) {
                        case 1: {
                            cout << "Nuevo nombre: ";
                            getline(cin, datos.name);
                            break;
                        }
                        case 2: {
                            cout << "Nuevo apellido: ";
                            getline(cin, datos.last_name);
                            break;
                        }
                        case 3: {
                            cout << "Nuevo genero (H/M): ";
                            cin >> datos.gender;
                            if (datos.gender != 'H' && datos.gender != 'M') {
                                cout << "Genero no valido. No se modifico." << endl;
                                datos.gender = mago->gender;
                            }
                            cin.ignore();
                            break;
                        }
                        case 4: {
                            cout << "Nueva edad: ";
                            if (!(cin >> datos.age) || datos.age < 0) {
                                cout << "Edad no valida. No se modifico." << endl;
                                datos.age = mago->age;
                                cin.clear();
                            }
                            cin.ignore();
                            break;
                        }
                        case 5: {
                            cout << "Esta muerto? (1=Si, 0=No): ";
                            if (!(cin >> datos.is_dead)) {
                                cout << "Valor no valido. No se modifico." << endl;
                                datos.is_dead = mago->is_dead;
                                cin.clear();
                            }
                            cin.ignore();
                            break;
                        }
                        case 6: {
                            cout << "Nuevo tipo de magia (elemental/unique/mixed/no_magic): ";
                            getline(cin, datos.type_magic);
                            if (datos.type_magic != "elemental" && datos.type_magic != "unique" && 
                                datos.type_magic != "mixed" && datos.type_magic != "no_magic") {
                                cout << "Tipo de magia no valido. No se modifico." << endl;
                                datos.type_magic = mago->type_magic;
                            }
                            break;
                        }
                    }
                    
                    arbol.modificar_mago(id, datos);
                    cout << "Mago modificado correctamente." << endl;
                    break;
                }
                case 7: {
                    int id;
                    cout << "Ingrese ID del mago para ver sus hechizos: ";
                    if (!(cin >> id)) throw runtime_error("ID debe ser un numero");
                    
                    arbol.mostrar_hechizos_mago(id);
                    break;
                }
                case 8: {
                    int id;
                    cout << "Ingrese ID del mago para agregar hechizo: ";
                    if (!(cin >> id)) throw runtime_error("ID debe ser un numero");
                    cin.ignore();
                    
                    hechizo nuevo;
                    cout << "Nombre del hechizo: ";
                    getline(cin, nuevo.nombre);
                    if (nuevo.nombre.empty()) throw runtime_error("Nombre no puede estar vacio");
                    
                    cout << "Poder del hechizo (1-100): ";
                    if (!(cin >> nuevo.poder) || nuevo.poder < 1 || nuevo.poder > 100) {
                        throw runtime_error("Poder debe ser entre 1 y 100");
                    }
                    cin.ignore();
                    
                    arbol.agregar_hechizo(id, nuevo);
                    cout << "Hechizo agregado correctamente." << endl;
                    break;
                }
                case 9: {
                    int id;
                    cout << "Ingrese ID del mago para reasignar hechizo: ";
                    if (!(cin >> id)) throw runtime_error("ID debe ser un numero");
                    cin.ignore();
                    
                    Mago* mago = arbol.buscar_mago_por_id(arbol.get_raiz(), id);
                    if (!mago) {
                        cout << "Mago no encontrado." << endl;
                        break;
                    }
                    
                    if (mago->num_hechizos == 0) {
                        cout << "Este mago no tiene hechizos." << endl;
                        break;
                    }
                    
                    cout << "Hechizos disponibles:" << endl;
                    for (int i = 0; i < mago->num_hechizos; i++) {
                        cout << i+1 << ". " << mago->hechizos[i].nombre 
                             << " (Poder: " << mago->hechizos[i].poder << ")" << endl;
                    }
                    
                    cout << "Seleccione hechizo a reasignar (1-" << mago->num_hechizos << "): ";
                    int seleccion;
                    if (!(cin >> seleccion) || seleccion < 1 || seleccion > mago->num_hechizos) {
                        cout << "Seleccion no valida." << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }
                    cin.ignore();
                    
                    arbol.reasignar_hechizo(id, mago->hechizos[seleccion-1]);
                    break;
                }
                case 10: {
                    try {
                        arbol.guardar_a_csv();
                        arbol.guardar_hechizos_csv();
                        cout << "Datos guardados correctamente." << endl;
                    } catch (const exception& e) {
                        cerr << "Error al guardar: " << e.what() << endl;
                    }
                    break;
                }
                case 0:
                    cout << "Saliendo..." << endl;
                    break;
                default:
                    cout << "Opcion no valida." << endl;
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (opcion != 0);

    return 0;
}