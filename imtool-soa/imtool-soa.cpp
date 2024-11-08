//
// Created by sergio on 7/10/24.
//
#include "libraries/imgsoa/imagesoa.hpp"
#include "libraries/common/progargs.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {    // cogemos primero los argumentos de entrada y los validamos con el validador de progargs
  if (!validate_parameters(argc, argv)) {
    return 1; // si los parametros no son validos, terminamos
  }

  std::string imagen_path = argv[0];
  std::string salida_path = argv[1];
  std::string operacion = argv[2];

  ImageSOA mi_imagen;
  mi_imagen.cargar_imagen(imagen_path);   // cargamos la imagen como tal (esta funcion ya verifica los datos)

  // gestionamos el tipo de operacion que sea:
  if (operacion == "info"){   // mostramos la información de los metadatos
    mi_imagen.info(); }

  else if (operacion == "maxlevel") {  // la posicion 4 del vector de argumentos es la nueva intensidad. lo pasamos a entero
    mi_imagen.maxlevel(std::stoi(argv[3])); }

  else if (operacion == "resize") {   // primero obtenemos los parametros extra: [4] es ancho, [5] es alto
    mi_imagen.resize(std::stoi(argv[4]), std::stoi(argv[5])); }

  // ICIAR :  4) Elimiacion de colores menos frecuentes
  // ICIAR :  5) Comprimir

  else{  // Cualquier otra cosa genera un error
    std::cerr << "Error: Operacion no valida\n";
    return 1; }

  mi_imagen.guardar_imagen(salida_path);  // guardamos la imagen con los cambios realizados
  return 0;
}
