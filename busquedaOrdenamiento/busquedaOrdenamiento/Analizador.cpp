/**
 * @file Analizador.cpp
 * @brief Implementacion de la clase con todos los algoritmos de la practica.
 */

#include "Analizador.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

AnalizadorAlgoritmos::AnalizadorAlgoritmos() : tamanoActual(0) {
    srand((unsigned)time(NULL));
}

void AnalizadorAlgoritmos::generarNuevosDatos(int n, int tipoDim, int m, bool permitirRepetidos) {
    if (tipoDim == 1) tamanoActual = n;
    else if (tipoDim == 2) tamanoActual = n * n;
    else if (tipoDim == 3) tamanoActual = n * m;

    cout << "\n[INFO] Tamano total: " << tamanoActual << " elementos." << endl;
    datos.clear();
    datos.resize(tamanoActual);

    cout << "[PROCESO] Iniciando generacion de " << tamanoActual << " elementos..." << endl;
    if (permitirRepetidos) {
        for (int i = 0; i < tamanoActual; i++) datos[i] = rand() % (tamanoActual * 2);
    }
    else {
        cout << "[PROCESO] Creando secuencia de numeros unicos..." << endl;
        for (int i = 0; i < tamanoActual; i++) datos[i] = i + 1;
        cout << "[PROCESO] Mezclando elementos (Shuffle) para eliminar orden..." << endl;
        auto rng = default_random_engine{ random_device{}() };
        shuffle(datos.begin(), datos.end(), rng);
    }
    cout << "[PROCESO] Datos generados correctamente." << endl;
}

// --- Funciones de Ordenamiento Internas ---
void bSort(vector<int>& v) {
    for (size_t i = 0; i < v.size() - 1; i++)
        for (size_t j = 0; j < v.size() - i - 1; j++)
            if (v[j] > v[j + 1]) swap(v[j], v[j + 1]);
}

void sSort(vector<int>& v) {
    for (size_t i = 0; i < v.size() - 1; i++) {
        int mIdx = i;
        for (size_t j = i + 1; j < v.size(); j++)
            if (v[j] < v[mIdx]) mIdx = j;
        swap(v[i], v[mIdx]);
    }
}

void iSort(vector<int>& v) {
    for (size_t i = 1; i < v.size(); i++) {
        int k = v[i], j = i - 1;
        while (j >= 0 && v[j] > k) { v[j + 1] = v[j]; j--; }
        v[j + 1] = k;
    }
}

void qSort(vector<int>& v, int l, int r) {
    int i = l, j = r, p = v[(l + r) / 2];
    while (i <= j) {
        while (v[i] < p) i++;
        while (v[j] > p) j--;
        if (i <= j) swap(v[i++], v[j--]);
    }
    if (l < j) qSort(v, l, j);
    if (i < r) qSort(v, i, r);
}

void mMerge(vector<int>& v, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    int* L = new int[n1], * R = new int[n2];
    for (int i = 0; i < n1; i++) L[i] = v[l + i];
    for (int j = 0; j < n2; j++) R[j] = v[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) v[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) v[k++] = L[i++];
    while (j < n2) v[k++] = R[j++];
    delete[] L; delete[] R;
}

void mSort(vector<int>& v, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mSort(v, l, m); mSort(v, m + 1, r);
        mMerge(v, l, m, r);
    }
}

double AnalizadorAlgoritmos::medirOrdenamiento(int opcionMetodo) {
    cout << "[PROCESO] Ordenando..." << endl;
    auto start = high_resolution_clock::now();

    if (opcionMetodo == 1) bSort(datos);
    else if (opcionMetodo == 2) sSort(datos);
    else if (opcionMetodo == 3) iSort(datos);
    else if (opcionMetodo == 4) qSort(datos, 0, tamanoActual - 1);
    else if (opcionMetodo == 5) mSort(datos, 0, tamanoActual - 1);

    auto end = high_resolution_clock::now();
    cout << "[PROCESO] Finalizado." << endl;
    return duration<double, milli>(end - start).count();
}

int AnalizadorAlgoritmos::medirBusqueda(int metodo, int valor, double& tiempoRes) {
    auto start = high_resolution_clock::now();
    int p = -1;
    if (metodo == 1) { // Secuencial
        for (int i = 0; i < tamanoActual; i++) if (datos[i] == valor) { p = i; break; }
    }
    else { // Binaria
        bSort(datos);
        int l = 0, r = tamanoActual - 1;
        while (l <= r) {
            int m = l + (r - l) / 2;
            if (datos[m] == valor) { p = m; break; }
            if (datos[m] < valor) l = m + 1; else r = m - 1;
        }
    }
    auto end = high_resolution_clock::now();
    tiempoRes = duration<double, milli>(end - start).count();
    return p;
}

int AnalizadorAlgoritmos::getTamano() { return tamanoActual; }