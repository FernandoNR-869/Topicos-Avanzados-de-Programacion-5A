/**
 * @file Analizador.cpp
 * @brief Implementacion de la clase con todos los algoritmos de la practica.
 * @author Fernando Navarro Rodríguez
 * @date 2026
 */

#include "../include/Analizador.h" // Navegación modular a la carpeta de encabezados
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Constructor: Inicializa el generador de números aleatorios [cite: 328]
AnalizadorAlgoritmos::AnalizadorAlgoritmos() : tamanoActual(0) {
    srand(static_cast<unsigned>(time(NULL)));
}

// Interfaz de Usuario: Centraliza la lógica para dejar el main limpio  [cite: 316-317, 622-624]
void AnalizadorAlgoritmos::ejecutarInterfaz() {
    int opcion, n, m, formato, genTipo, valBusq;
    double tEjecucion = 0;

    do {
        cout << "\n================================================" << endl;
        cout << "   ANALIZADOR DE ALGORITMOS - FERNANDO NAVARRO " << endl;
        cout << "================================================" << endl;
        cout << "1. Busqueda Secuencial" << endl;
        cout << "2. Busqueda Binaria (ordena automaticamente)" << endl;
        cout << "3. Ordenar Arreglo" << endl;
        cout << "4. Salir" << endl;
        cout << "Seleccione opcion: "; cin >> opcion;

        if (opcion >= 1 && opcion <= 3) {
            // Configuración de Dimensiones
            cout << "\n--- Configuracion de Dimensiones ---" << endl;
            cout << "1. Lineal (N)         -> Ejemplo: 100" << endl;
            cout << "2. Cuadratico (N*N)   -> Ejemplo: 10x10 = 100" << endl;
            cout << "3. Rectangular (N*M)  -> Ejemplo: 10x5  = 50" << endl;
            cout << "Seleccione formato: "; cin >> formato;
            cout << "Ingrese N: "; cin >> n;
            m = (formato == 3) ? (cout << "Ingrese M: ", cin >> m, m) : 0;

            // Selección de tipo de generación [cite: 244-248]
            cout << "\n1. Con repeticion\n2. Sin repeticion" << endl;
            cout << "Seleccione tipo de generacion: "; cin >> genTipo;

            generarNuevosDatos(n, formato, m, (genTipo == 1));
            mostrarArreglo();

            if (opcion == 1 || opcion == 2) {
                cout << "Ingrese valor a buscar: "; cin >> valBusq;
                int pos = medirBusqueda(opcion, valBusq, tEjecucion);
                if (pos != -1) cout << "Valor encontrado en posicion: " << pos << endl;
                else cout << "Valor no encontrado." << endl;
            }
            else {
                cout << "Metodo (1:Burbuja, 2:Seleccion, 3:Insercion, 4:Quick, 5:Merge): ";
                int met; cin >> met;
                tEjecucion = medirOrdenamiento(met);
            }

            // Formato de salida de tiempo
            cout << "----------------------------------------" << endl;
            cout << ">>> TIEMPO DE EJECUCION: " << tEjecucion << " ms <<<" << endl;
            cout << "----------------------------------------" << endl;
        }
    } while (opcion != 4);
}

// Gestión de datos y memoria dinámica (Heap) [cite: 237-242, 267-268]
void AnalizadorAlgoritmos::generarNuevosDatos(int n, int tipoDim, int m, bool permitirRep) {
    if (tipoDim == 1) tamanoActual = n;
    else if (tipoDim == 2) tamanoActual = n * n;
    else if (tipoDim == 3) tamanoActual = n * m;

    cout << "[INFO] Tamano total calculado: " << tamanoActual << " elementos." << endl;
    datos.assign(tamanoActual, 0); // Redimensionamiento dinámico [cite: 241, 331]

    cout << "[PROCESO] Iniciando generacion de " << tamanoActual << " elementos..." << endl;
    if (permitirRep) {
        for (int i = 0; i < tamanoActual; i++) datos[i] = rand() % (tamanoActual * 2);
    }
    else {
        cout << "[PROCESO] Creando secuencia de numeros unicos..." << endl;
        for (int i = 0; i < tamanoActual; i++) datos[i] = i + 1;
        cout << "[PROCESO] Mezclando elementos (Shuffle) para eliminar orden..." << endl;
        shuffle(datos.begin(), datos.end(), default_random_engine(random_device{}()));
    }
    cout << "[PROCESO] Datos generados correctamente." << endl;
}

// Medición de rendimiento con <chrono> [cite: 257-261, 315]
double AnalizadorAlgoritmos::medirOrdenamiento(int opcionMetodo) {
    auto start = high_resolution_clock::now();

    if (opcionMetodo == 1) burbuja();
    else if (opcionMetodo == 2) seleccion();
    else if (opcionMetodo == 3) insercion();
    else if (opcionMetodo == 4) quickSort(0, tamanoActual - 1);
    else if (opcionMetodo == 5) mergeSort(0, tamanoActual - 1);

    auto end = high_resolution_clock::now();
    return duration<double, milli>(end - start).count();
}

// --- ALGORITMOS DE ORDENAMIENTO [cite: 174-188, 194] ---

void AnalizadorAlgoritmos::burbuja() {
    for (int i = 0; i < tamanoActual - 1; i++)
        for (int j = 0; j < tamanoActual - i - 1; j++)
            if (datos[j] > datos[j + 1]) swap(datos[j], datos[j + 1]);
}

void AnalizadorAlgoritmos::seleccion() {
    for (int i = 0; i < tamanoActual - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < tamanoActual; j++)
            if (datos[j] < datos[minIdx]) minIdx = j;
        swap(datos[i], datos[minIdx]);
    }
}

void AnalizadorAlgoritmos::insercion() {
    for (int i = 1; i < tamanoActual; i++) {
        int clave = datos[i], j = i - 1;
        while (j >= 0 && datos[j] > clave) { datos[j + 1] = datos[j]; j--; }
        datos[j + 1] = clave;
    }
}

void AnalizadorAlgoritmos::quickSort(int izq, int der) {
    if (izq < der) {
        int pivote = datos[(izq + der) / 2], i = izq, j = der;
        while (i <= j) {
            while (datos[i] < pivote) i++;
            while (datos[j] > pivote) j--;
            if (i <= j) { swap(datos[i], datos[j]); i++; j--; }
        }
        quickSort(izq, j); quickSort(i, der);
    }
}

void AnalizadorAlgoritmos::mergeSort(int izq, int der) {
    if (izq < der) {
        int m = izq + (der - izq) / 2;
        mergeSort(izq, m); mergeSort(m + 1, der);
        merge(izq, m, der);
    }
}

void AnalizadorAlgoritmos::merge(int izq, int m, int der) {
    int n1 = m - izq + 1, n2 = der - m;
    vector<int> L(n1), R(n2); // Memoria dinámica segura [cite: 265-266]
    for (int i = 0; i < n1; i++) L[i] = datos[izq + i];
    for (int j = 0; j < n2; j++) R[j] = datos[m + 1 + j];
    int i = 0, j = 0, k = izq;
    while (i < n1 && j < n2) datos[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) datos[k++] = L[i++];
    while (j < n2) datos[k++] = R[j++];
}

// --- ALGORITMOS DE BUSQUEDA [cite: 194, 373-380] ---

int AnalizadorAlgoritmos::medirBusqueda(int opt, int val, double& t) {
    auto s = high_resolution_clock::now();
    int res = -1;
    if (opt == 1) { // Secuencial O(n)
        cout << "[PROCESO] Iniciando busqueda secuencial..." << endl;
        for (int i = 0; i < tamanoActual; i++) if (datos[i] == val) { res = i; break; }
    }
    else { // Binaria O(log n)
        cout << "[PROCESO] Ordenando para busqueda binaria (Burbuja)..." << endl;
        burbuja(); // Requisito de ordenamiento previo [cite: 203-204, 379]
        int l = 0, r = tamanoActual - 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (datos[mid] == val) { res = mid; break; }
            if (datos[mid] < val) l = mid + 1; else r = mid - 1;
        }
    }
    auto e = high_resolution_clock::now();
    t = duration<double, milli>(e - s).count();
    return res;
}

void AnalizadorAlgoritmos::mostrarArreglo() {
    cout << "\nArreglo: ";
    for (int i = 0; i < min(tamanoActual, 10); i++) cout << datos[i] << " ";
    if (tamanoActual > 10) cout << "...";
    cout << endl;
}