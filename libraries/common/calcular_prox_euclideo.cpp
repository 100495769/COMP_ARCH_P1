//
// Created by sergio on 14/11/24.
//
#include "calcular_prox_euclideo.hpp"
#include <iostream>

/*int calcular_prox_euclideo::my_pow(int a){
  return a*a;
}

void calcular_prox_euclideo::añadir_euclideos() {
  int desajuste = 0;
  int max_tamaño_actual = 2 * my_pow(tamaño);
  int mi_tamaño = 0;
  int mi_tamaño_dic = 99999;
  std::cout<<"Restos: "<<restos[tamaño].size()<<std::endl;
  for (int i = 0; i< restos[tamaño].size() + tamaño; i++){
      mi_tamaño = (my_pow(tamaño + desajuste) + my_pow(tamaño-desajuste));
      if (restos[tamaño].size() > 0) {
        mi_tamaño_dic =
            my_pow(restos[tamaño-1][i - desajuste][0]) * my_pow(restos[tamaño-1][i - desajuste][1]);
      }
     if ( mi_tamaño <= mi_tamaño_dic){
       desajuste++;
       if (mi_tamaño <= max_tamaño_actual){
         std::cout <<"Mi tamaño euclides:"<< tamaño+desajuste<<" "<<tamaño-desajuste<<std::endl;
         prox_euclides.push({tamaño+desajuste, tamaño-desajuste});
       }
       else {
         // Lo metemos al diccionario para el siguiente.
         restos[tamaño].push_back({my_pow(tamaño + desajuste), my_pow(tamaño - desajuste)});
       }
     }
     else{
       if (mi_tamaño_dic <= max_tamaño_actual){
         prox_euclides.push({restos[tamaño][i-desajuste][0], restos[tamaño][i-desajuste][1]});
         std::cout << "Mi tamaño euclides:"<< restos[tamaño][i-desajuste][0]<<" "<<restos[tamaño][i-desajuste][1]<<std::endl;
       }
       else {
         restos[tamaño].push_back({restos[tamaño][i-desajuste][0], restos[tamaño][i-desajuste][1]});
       }
     }
  }
}

std::vector<int> calcular_prox_euclideo::prox_euclideo() {
  if (prox_euclides.size() < 1){
    tamaño += 1;
    std::cout<<"Mi tamaño: "<<tamaño<<std::endl;
    añadir_euclideos();

  }
  std::vector<int> mi_return = {0,0};
  std::cout << "Elementos dent:" <<prox_euclides.front().size()<<std::endl;
  prox_euclides.pop();
  return mi_return;
};
*/