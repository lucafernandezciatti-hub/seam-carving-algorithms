#include "FuerzaBruta.h"
#include <limits>

// Función auxiliar de recursión para Backtracking. El parametro k indica la fila por recorrer.
void encontrarSeamBacktracking_aux(const std::vector<std::vector<double>>& energia, int k, std::vector<int> V, double energia_actual, double& mejor_energia, std::vector<int>& mejor_solucion) {
    // mejor_solucion es un parametro por el cual registramos la mejor_solucion encontrada hasta el momento para poder podar por optimalidad.
    if (energia_actual >= mejor_energia) { // Si la energía acumulada hasta ahora ya es mayor que la solución encontradaa hasta el momento, hay que podar por optimalidad.
        return;
    }

    if (k == energia.size()) { // Si se llega a una solución completa y es mejor que la obtenida hasta ahora, se actualiza la mejor solución y la mejor energía.
        if (energia_actual < mejor_energia) { 
            mejor_energia = energia_actual;
            mejor_solucion = V;
        }
        return;
    }

    if (k == 0) { // Misma división en casos que en FB, cuando estamos en la primera fila empezamos por todas las columnas.
        for (int i = 0; i < (int)energia[0].size(); i++) { // Mismo ciclo que en FB pero ahora en cada iteración se va actualizando la energía actual.
            std::vector<int> V_nuevo = V;
            V_nuevo.push_back(i);
            double nueva_energia = energia_actual + energia[k][i]; // Se añade a la energía actual la energía del último píxel añadido al vector del seam.
            encontrarSeamBacktracking_aux(energia, k + 1, V_nuevo, nueva_energia, mejor_energia, mejor_solucion);
        }

    } else {
        // Mismo procedimiento que para la primera fila.
        int ultimo = V[V.size() - 1];
        for (int j = ultimo - 1; j <= ultimo + 1; j++) { // Elegimos inteligentemente solo recorrer las columnas adyacentes. Debido a esto no es hecesaria una poda por factibilidad.
            if (j >= 0 && j < (int)energia[0].size()) {
                std::vector<int> V_nuevo = V;
                V_nuevo.push_back(j);
                double nueva_energia = energia_actual + energia[k][j];
                encontrarSeamBacktracking_aux(energia, k + 1, V_nuevo, nueva_energia, mejor_energia, mejor_solucion);
            }
        }
    }
}

std::vector<int> encontrarSeamBacktracking(const std::vector<std::vector<double>>& energia) {
    std::vector<int> V = {};
    double mejor_energia = std::numeric_limits<double>::infinity(); // La mejor energia es al comienzo infinito porque no se conoce la energía total de la costura de minima energia.
    std::vector<int> mejor_solucion;
    encontrarSeamBacktracking_aux(energia, 0, V, 0, mejor_energia, mejor_solucion); // Pasamos como parametro la mejor energía y la energia actual del seam.
    return mejor_solucion;
}