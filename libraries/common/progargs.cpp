//
// Created by sergio on 7/10/24.
//
#include "progargs.hpp"
#include <iostream>

auto validate_parameters(int argc, std::vector<std::string> argumentos) -> bool {
  const std::string& operacion = argumentos[3];
  std::cout << "Argc: " << argc << "\n" << argumentos[0] << "\n" << argumentos[1] << "\n" << argumentos[2] << "\n" << argumentos[3] << "\n";

  // int nueva_intensidad, nuevo_ancho, nuevo_alto;
  //// ICIAR METE VALORES TUYOS SI LO NECESITAS !!

  // gestionamos el tipo de operacion que sea:
  if ((operacion == "info" && argc != 4) || (operacion == "maxlevel" && argc != 5) ||
      (operacion == "resize" && argc != 6)) {
    // ICIAR :  4) Elimiacion de colores menos frecuentes
    // ICIAR :  5) Comprimir
    std::cout << "Error: operacion incorrecta o numero de argumentos erroneos\n";
    return false;  // gestionamos que la operacion y el numero de argumentos son correctos
  }
  // si va bien, devolvemos true
  return true;
}
