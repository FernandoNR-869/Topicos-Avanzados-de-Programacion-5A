#include <iostream>
#include "Analizador.h"

using namespace std;

int main() {
    AnalizadorAlgoritmos miAnalizador; // Instancia POO
    int opcionPrincipal, n, m, formato, tipoDato, metodo, valor;
    double tiempo;

    do {
        cout << "\n=== MENU ===" << endl;
        cout << "1. Busqueda Secuencial" << endl;
        cout << "2. Busqueda Binaria (ordena automaticamente)" << endl;
        cout << "3. Ordenar Arreglo" << endl;
        cout << "4. Salir" << endl;
        cout << "Seleccione opcion: ";
        cin >> opcionPrincipal;

        if (opcionPrincipal == 4) break;

        // --- Interfaz Original de Dimensiones ---
        if (opcionPrincipal == 3 || opcionPrincipal == 1 || opcionPrincipal == 2) {
            cout << "\nSeleccione formato:\n1. Lineal (N)\n2. Cuadratico (N*N)\n3. Rectangular (N*M)\nSeleccion: ";
            cin >> formato;

            cout << "Ingrese el valor de N: ";
            cin >> n;
            m = 0;
            if (formato == 3) {
                cout << "Ingrese el valor de M: ";
                cin >> m;
            }

            cout << "1. Repetidos\n2. Unicos\nSeleccion: ";
            cin >> tipoDato;

            // El objeto se encarga de la generacion y los mensajes [PROCESO]
            miAnalizador.generarNuevosDatos(n, formato, m, (tipoDato == 1));

            if (opcionPrincipal == 1 || opcionPrincipal == 2) {
                cout << "Ingrese el valor a buscar: ";
                cin >> valor;
                int pos = miAnalizador.medirBusqueda(opcionPrincipal, valor, tiempo);
                cout << (pos != -1 ? "\n[INFO] Valor encontrado en la posicion: " : "\n[INFO] Valor no encontrado. ") << pos << endl;
                cout << ">>> TIEMPO: " << tiempo << " ms <<<" << endl;
            }
            else if (opcionPrincipal == 3) {
                cout << "\n=== METODOS DE ORDENAMIENTO ===" << endl;
                cout << "1. Burbuja\n2. Seleccion\n3. Insercion\n4. QuickSort\n5. MergeSort" << endl;
                cout << "Seleccione metodo: ";
                cin >> metodo;

                tiempo = miAnalizador.medirOrdenamiento(metodo);
                cout << ">>> TIEMPO: " << tiempo << " ms <<<" << endl;
            }
        }

    } while (opcionPrincipal != 4);

    return 0;
}