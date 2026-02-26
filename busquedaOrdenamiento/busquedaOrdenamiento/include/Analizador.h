#ifndef ANALIZADOR_H
#define ANALIZADOR_H

#include <vector>
#include <chrono>
#include <iostream>

class AnalizadorAlgoritmos {
private:
    std::vector<int> datos; ///< Uso de Heap para evitar desbordamientos[cite: 267].
    int tamanoActual;

    // Métodos internos de ordenamiento [cite: 174, 179, 184]
    void burbuja();
    void seleccion();
    void insercion();
    void quickSort(int izq, int der);
    void mergeSort(int izq, int der);
    void merge(int izq, int m, int der);

public:
    AnalizadorAlgoritmos();
    void ejecutarInterfaz(); // Toda la lógica de menús restaurada
    void generarNuevosDatos(int n, int tipoDim, int m, bool permitirRepetidos);
    double medirOrdenamiento(int opcionMetodo);
    int medirBusqueda(int opcionMetodo, int valor, double& tiempoResultado);
    void mostrarArreglo();
};

#endif