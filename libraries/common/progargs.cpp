//
// Created by sergio on 7/10/24.
//

#include "progargs.hpp"
#include <iostream>

bool validate_parameters(int argc, char * argv[]) {
  std::string operacion = argv[2];
  //int nueva_intensidad, nuevo_ancho, nuevo_alto;
  //// ICIAR METE VALORES TUYOS SI LO NECESITAS !!

  // gestionamos el tipo de operacion que sea:
  if ((operacion == "info" && argc != 3) ||
      (operacion == "maxlevel" && argc != 4) ||
      (operacion == "resize" && argc != 5) ||
      (operacion != "info" || "maxlevel" || "resize" || "cutfreq" || "compress")
      ){
      // ICIAR :  4) Elimiacion de colores menos frecuentes
      // ICIAR :  5) Comprimir
    return false;   // gestionamos que la operacion y el numero de argumentos son correctos
  }
  // si va bien, devolvemos true
  return true;
}
