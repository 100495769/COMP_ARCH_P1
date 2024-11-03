//
// Created by sergio on 7/10/24.
//
#include "libraries/imgsoa/imagesoa.hpp"

#include <iostream>
#include <string>

int main(std::string& imagen_path, std::string & salida_path, std::string & operacion, std::string & parametros_extra) {
  ImageSOA mi_imagen;
  mi_imagen.cargar_imagen(imagen_path);   // cargamos la imagen como tal (esta funcion ya verifica los datos)

  // gestionamos el tipo de operacion que toque:
  if (operacion == "info") {

  }
  else if (operacion == "maxlevel") {

  }
  else if (operacion == "resize") {

  }
  // ICIAR :  4) Elimiacion de colores menos frecuentes
  // ICIAR :  5) Comprimir

  mi_imagen.guardar_imagen(salida_path);  // guardamos la imagen con los cambios realizados

  return 0;

}
