//
// Created by sergio on 7/10/24.
//
#include "libraries/imgsoa/imagesoa.hpp"

#include <iostream>
#include <string>

int main(std::string& imagen_path, std::string & salida_path, std::string & operacion, std::string & parametros_extra) {
  ImageSOA mi_imagen;
  mi_imagen.cargar_imagen(imagen_path);   // cargamos la imagen como tal
  mi_imagen.verificar_datos();    // verificamos que los datos esten bien


}
