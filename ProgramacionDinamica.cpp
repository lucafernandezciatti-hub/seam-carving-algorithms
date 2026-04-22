#include "ProgramacionDinamica.h"
#include <limits>
#include <vector>
#include <iostream>
#include <chrono>

// Función auxiliar que calcula el minimo entre tres doubles.
double min(double a, double b, double c) {
    if (a > b) {
        if (b > c) {
            return c;
        }
        return b;
    }
    if (a > c) {
        return c;
    }
    return a;
}

// Función auxiliar que crea un memo de filas x cols con valores -1.0 en cada celda.
std::vector<std::vector<double>> crearMEMO(int filas, int cols) {
    return std::vector<std::vector<double>>(filas, std::vector<double>(cols, -1.0));
}

// Función auxiliar Bottom-Up que realiza Tableau-Filling para encontrar la energía de la costura de mínima energía y luego reconstruye la solución.
std::vector<int> encontrarSeamPD_aux(const std::vector<std::vector<double>>& energia) {
    int nrow = energia.size();
    int ncol = energia[0].size();

    std::vector<std::vector<double>> MEMO = crearMEMO(nrow, ncol); // Creamos el MEMO
    // MEMO guarda en cada par de indices i,j la energia de la costura de mínima energía desde el pixel i,j de
    // la matriz de energías, hasta la última fila, i=nrow-1.

    for (int j = 0; j < ncol; j++) { // Completamos la última fila de MEMO porque los valores de sus indices coinciden con los de la matriz de energía.
        MEMO[MEMO.size() - 1][j] = energia[nrow - 1][j];
    }

    for (int i = nrow - 2; i >= 0; i--) { // Por cada fila desde la anteúltima hasta la primera y 
        for (int j = 0; j < ncol; j++) {  // por cada columna, completamos los valores de MEMO[i][j].
            double izq = std::numeric_limits<double>::infinity();
            double der = std::numeric_limits<double>::infinity();

            if (j + 1 < ncol) { // Si j+1 no se pasa de índice por derecha nos fijamos el valor de MEMO en esa columna de la fila abajo.
                der = MEMO[i + 1][j + 1];
            }
            if (j - 1 >= 0) { // Si j-1 no se pasa de índice por derecha nos fijamos el valor de MEMO en esa columna de la fila abajo.
                izq = MEMO[i + 1][j - 1];
            }
            // No es posible pasarse de índice moviendose sobre la misma columna.
            double mid = MEMO[i + 1][j];
            MEMO[i][j] = energia[i][j] + min(izq, mid, der); // Asignamos a MEMO[i][j] el valor de su mínima costura, que es
                                                             // el valor de su celda en la matriz de energías sumado al valor de la mínima costura desde sus pixeles inferiores adyacentes válidos.
        }
    }
    // Asignamos un mínimo valor provisorio y un índice para este mínimo.
    // Los valores totales de energía de las mínimas costuras alcanzables para cada píxel de la primera fila
    // se encuentran en MEMO[0]
    double min = MEMO[0][0];
    int indexmin = 0;

    for (int j = 1; j < ncol; j++) { // Buscamos el verdadero mínimo valor de energía para una costura completa.
        if (MEMO[0][j] < min) {
            min = MEMO[0][j];
            indexmin = j;
        }
    }

    std::vector<int> vr = {}; // Inicializamos el vector de respuesta push_backeando el primer índice de columna
    vr.push_back(indexmin);   // encontrado para la primera fila.

    // RECONSTRUCCIÓN DE LA SOLUCIÓN:
    // Por cada fila de la matriz de energías guardo el minimo valor de los MEMOs de las columnas adyacentes en la fila inferior
    // y lo inserto en el vector de respuesta guardando en next_j su índice de columna. En la siguiente fila, next_j es la columna
    // a partir de la cual tengo que volver a repetir el proceso.
    for (int i = 0; i < nrow - 1; i++) {
        int next_j = -1;
        double min_val_below = std::numeric_limits<double>::infinity();

        for (int j = indexmin - 1; j <= indexmin + 1; j++) {
            if ((j >= 0 && j < ncol) && MEMO[i + 1][j] < min_val_below) { // Si el índice j es válido para columnas y el valor de MEMO en la siguiente fila columna j es mejor que el que ya tengo lo guardo.
                min_val_below = MEMO[i + 1][j];
                next_j = j;
            }
        }

        indexmin = next_j;
        vr.push_back(indexmin); 
    }

    return vr;
}

std::vector<int> encontrarSeamPD(const std::vector<std::vector<double>>& energia) {
    std::vector<int> res = encontrarSeamPD_aux(energia);
    return res;
}