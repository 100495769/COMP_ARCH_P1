//
// Created by sergio on 14/11/24.
//
#include "calcular_prox_euclideo.hpp"
#include <iostream>

int calcular_prox_euclideo::my_pow(int a){
  return a*a;
}


void calcular_prox_euclideo::añadir_euclideos() {
  size_t tamaño_dict = restos[tamaño].size();
  int desajuste = tamaño_y;
  int resto_x = 0;
  int resto_y = 0;
  int mi_x = 0;
  int mi_y = 0;
  int resto_val = 0;
  int mi_val = 0;
  int tamaño_max = my_pow(tamaño_x) + my_pow(tamaño_y);
  for (int i = 0; i < static_cast<int>(tamaño_dict) + tamaño_y; i++){
    resto_x = restos[tamaño][static_cast<size_t>(i-tamaño_y+desajuste)][0];
    resto_y = restos[tamaño][static_cast<size_t>(i-tamaño_y+desajuste)][1];
    mi_x = tamaño_x + desajuste;
    mi_y = tamaño_y - desajuste;
    resto_val = my_pow(resto_x) + my_pow(resto_y);
    mi_val = my_pow(mi_x)+ my_pow(mi_y);
    if(resto_val > mi_val){
      if (resto_val > tamaño_max){
        restos[tamaño + 1].push_back({resto_x, resto_y});
      }
      else{
        prox_euclides.push({resto_x, resto_y});
      }
    }
    else{
      if (mi_val>tamaño_max){
        restos[tamaño + 1].push_back({mi_x, mi_y});
      }
      else{
        prox_euclides.push({mi_x, mi_y});
      }
    }
  }
}
void calcular_prox_euclideo::init(){
  prox_euclides.push({1,0});
  prox_euclides.push({1,1});
  restos[3].push_back({2,0});
  tamaño_x = 1;
  tamaño_y = 1;
}
std::vector<int> calcular_prox_euclideo::prox_euclideo() {
  if (prox_euclides.empty()){
    if (tamaño_x == tamaño_y){
      tamaño_x++;
    }
    else{
      tamaño_y++;
    }
    tamaño = tamaño_y+tamaño_x; // En verdad tamaño ++ pero asi es mas visual.
    std::cout<<"Mi tamaño: "<<tamaño<<std::endl;
    añadir_euclideos();

  }
  std::vector<int> mi_return = {0,0};
  std::cout << "Elementos dent:" <<prox_euclides.front().size()<<std::endl;
  prox_euclides.pop();
  return mi_return;
};
