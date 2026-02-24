/**
 * @file Analizador.h
 * @brief Clase principal para la gestion y analisis de algoritmos de ordenamiento y busqueda.
 * @authors
 * - Gabriela Estefania Quintanar Yañez
 * - Fernando Navarro Rodriguez
 * - Jose Carlos Aguilar Contreras
 * - Alain Josue Reyes Dominguez
 * - Juan Antonio Prudente Roque
 * @date 2026
 */

#ifndef ANALIZADOR_H
#define ANALIZADOR_H

#include <vector>
#include <chrono>
#include <iostream>

 /**
  * @class AnalizadorAlgoritmos
  * @brief Clase encargada de encapsular la logica de generacion, ordenacion y busqueda de datos.
  * * Esta clase permite administrar un contenedor de datos dinamico, realizar pruebas de estres
  * con diferentes dimensiones y medir el impacto computacional de diversos algoritmos.
  */
class AnalizadorAlgoritmos {
private:
    std::vector<int> datos; ///< Contenedor principal para el almacenamiento de elementos.
    int tamanoActual;       ///< Representa la cantidad total de elementos generados.

public:
    /**
     * @brief Constructor de la clase. Inicializa el estado del analizador.
     */
    AnalizadorAlgoritmos();

    /**
     * @brief Genera una nueva secuencia de datos aleatorios.
     * @param n Valor base para la dimension.
     * @param tipoDim Especifica el formato (1: Lineal, 2: Cuadratico, 3: Rectangular).
     * @param m Valor adicional necesario para el calculo de dimensiones rectangulares.
     * @param permitirRepetidos Booleano que define si se admiten valores duplicados.
     */
    void generarNuevosDatos(int n, int tipoDim, int m, bool permitirRepetidos);

    /**
     * @brief Ejecuta un metodo de ordenamiento y registra su duracion.
     * @param opcionMetodo Identificador del algoritmo (1:Burbuja, 2:Seleccion, 3:Insercion, 4:QuickSort, 5:MergeSort).
     * @return Tiempo de ejecucion medido en milisegundos (ms).
     */
    double medirOrdenamiento(int opcionMetodo);

    /**
     * @brief Ejecuta un metodo de busqueda y registra su duracion.
     * @param opcionMetodo Identificador del algoritmo (1:Secuencial, 2:Binaria).
     * @param valor El elemento que se desea localizar en el arreglo.
     * @param tiempoResultado Referencia para almacenar el tiempo de respuesta obtenido.
     * @return La posicion del elemento en el arreglo, o -1 si no fue localizado.
     */
    int medirBusqueda(int opcionMetodo, int valor, double& tiempoResultado);

    /**
     * @brief Proporciona acceso al numero total de elementos gestionados actualmente.
     * @return Entero con el tamano del arreglo.
     */
    int getTamano();
};

#endif