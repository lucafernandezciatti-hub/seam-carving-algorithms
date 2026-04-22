#include "FuerzaBruta.h"

// Función que calcula la energía de un seam.
double energia_calc (const std::vector<std::vector<double>>& energia, const std::vector<int>& V) {
    double energia_seam = 0;
    for (int i=0; i<V.size(); i++) {                     // Por cada fila representada como un indice de V.
        energia_seam = energia_seam + energia[i][V[i]];// Se suman las energías de la fila i, columna V[i].
    }
    return energia_seam;
}

// Función auxiliar recursiva para fuerza bruta. El parametro k indica la fila por recorrer.
std::vector<int> encontrarSeamFuerzaBruta_aux(const std::vector<std::vector<double>>& energia, int k, std::vector<int> V) { 
    if (k == energia.size()) {       // Ya no quedan más filas por recorrer.
        return V;
    } 
    else {
        // Me encuentro en la primera fila de la matriz de energias.
        if (k==0) {
            std::vector<int> mejor; // Esta variable guarda el mejor camino encontrado.
            for (int i=0; i<energia[0].size();i++) { // Por cada columna.
                std::vector<int> V_nuevo = V;
                V_nuevo.push_back(i);
                std::vector<int> S = encontrarSeamFuerzaBruta_aux(energia,k+1,V_nuevo); // Ejecuto la recursión para la siguiente fila.
                if (mejor.size() == 0 || energia_calc(energia,S) < energia_calc(energia,mejor)) { // Si todavía no tengo un mejor o el seam hallado tiene menor energía, cambio mejor.
                    mejor = S;
                }
            }
            return mejor;
        // No estoy en la primer fila de pixeles.
        } else {
            int ultimo_agregado = V[V.size() - 1];
            std::vector<int> mejor; // Veo cual es el mejor camino a partir del último pixel agregado a V.
            for (int j = ultimo_agregado - 1; j <= ultimo_agregado + 1; j++) { // Elegimos inteligentemente solo recorrer las columnas adyacentes.
                if (j >= 0 && j < energia[0].size()) { // Chequeamos no pasarnos por izquierda o derecha de los índices válidos de columna.
                    std::vector<int> V_nuevo = V;
                    V_nuevo.push_back(j); 
                    std::vector<int> S = encontrarSeamFuerzaBruta_aux(energia,k+1,V_nuevo); // Mismo procedimiento que para la primera fila.
                    if (mejor.empty() || energia_calc(energia,S) < energia_calc(energia,mejor)) {  
                        mejor = S;
                    }
                }
            }
            return mejor;
        }
    }
}

std::vector<int> encontrarSeamFuerzaBruta(const std::vector<std::vector<double>>& energia) {
    std::vector<int> V = {};
    return encontrarSeamFuerzaBruta_aux(energia, 0, V); // Llamamos a la recursión desde la primera fila (k=0), con V (vector para guardar el seam parcial) vacío.
}