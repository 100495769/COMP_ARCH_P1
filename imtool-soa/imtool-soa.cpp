//
// Created by sergio on 7/10/24.
//
#include "libraries/imgsoa/imagesoa.hpp"

#include <iostream>
#include <string>
// std::string& imagen_path, std::string & salida_path, std::string & operacion, std::string & parametros_extra
int main(int argc, char* argv[]) {
  // cogemos los argumentos de entrada
  std::string imagen_path, salida_path, operacion;
  int nueva_intensidad, nuevo_ancho, nuevo_alto;  // ICIAR METE VALORES TUYOS!!

  ImageSOA mi_imagen;
  mi_imagen.cargar_imagen(imagen_path);   // cargamos la imagen como tal (esta funcion ya verifica los datos)

  // gestionamos el tipo de operacion que sea:
  if (operacion == "info" && argc == 3){
    mi_imagen.info();  // mostramos la información de los metadatos
  }

  else if (operacion == "maxlevel" && argc == 4) {
    nueva_intensidad = std::stoi(argv[3]);  // la posicion 4 del vector de argumentos es la nueva intensidad. lo pasamos a entero
    mi_imagen.maxlevel(nueva_intensidad);
  }

  else if (operacion == "resize") { // primero obtenemos los parametros extra
    nuevo_ancho = std::stoi(argv[4]);
    nuevo_alto = std::stoi(argv[5]);
    mi_imagen.resize(nuevo_ancho, nuevo_alto);
  }
  // ICIAR :  4) Elimiacion de colores menos frecuentes
  // ICIAR :  5) Comprimir

  else{  // Cualquier otra cosa genera un error
    std::cerr << "Error: Operacion no valida o numero de argumentos incorrecto\n";
    return -1;
  }

  mi_imagen.guardar_imagen(salida_path);  // guardamos la imagen con los cambios realizados

  return 0;

}
