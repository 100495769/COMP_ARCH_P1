//
// Created by sergio on 14/11/24.
//

#ifndef IMTOOL_CALCULAR_PROX_EUCLIDEO_HPP
#define IMTOOL_CALCULAR_PROX_EUCLIDEO_HPP
#include <cstdint>
#include <vector>
#include <queue>
#include <memory>
#include <unordered_map>
class calcular_prox_euclideo {
  private:
    // Tamaños en uso
    int tamaño = 0;
    int x_anterior = 0;
    int x_actual = 0;
    // Diccionario: key = tamaño; lista con cosas ya calculadas que son mayores que el anterior tamaño comprobar en este
    std::vector<std::vector<std::pair<int, int>>> restos;
    // Lista con los valores de euclides que van a ir saliendo;
    std::queue<std::vector<int>> prox_euclides;
    void añadir_euclideos();
    int my_pow(int a);
  public:
    std::vector<int>prox_euclideo();
};

#endif  // IMTOOL_CALCULAR_PROX_EUCLIDEO_HPP
