#pragma once
#ifndef SORT_H
#define SORT_H

void ordenarBurbuja(int arreglo[], int n);
void ordenarSeleccion(int arreglo[], int n);
void ordenarInsercion(int arreglo[], int n);
void quickSort(int arreglo[], int izquierda, int derecha, int& comparaciones, int& intercambios);
void mergeSort(int arreglo[], int izquierda, int derecha, int& comparaciones);

#endif
