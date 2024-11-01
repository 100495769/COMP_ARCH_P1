//
// Created by sergio on 7/10/24.
//
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "imagesoa.hpp"

ImageSOA::ImageSOA() = default;

bool ImageSOA::verificar_datos() {
  // Esta función verifica que los datos de la imagen correspondan con lo que queremos.
  if (numero_magico != "P6") {
    std::cerr << "Numero magico no valido: Se espera P6 \n";
    return false;
  }
  if (alto <= 0 || ancho <= 0 || max_intensidad <= 0 || max_intensidad > 65536) {
    std::cerr << "Datos de imagen invalidos: ancho, alto o maxima intensidad no validos \n";
    return false;
  }
  return true;  // si no hay errores, devuelve true
}


void ImageSOA::cargar_imagen(std::string& path_imagen) {
  // Esta función abre la imagen desde el path dado.
  std::ifstream archivo(path_imagen, std::ios::binary);  // lo ponemos en binario para leer bytes

  if (!archivo) {   // si el archivo no existe o hay errores, se termina la ejecucion
    std::cerr << "Error al abrir la imagen: " << path_imagen << "\n";
    return;
  }

  std::string ancho_str, alto_str, max_intensidad_str;
  archivo >> numero_magico >> ancho_str >> alto_str >> max_intensidad_str ; // cogemos todos los datos ignorando saltos de pagina y espacios en blanco
  ancho = std::stoi(ancho_str);
  alto = std::stoi(alto_str);
  max_intensidad = std::stoi(max_intensidad_str);   // cambio de tipo de los valores numericos

  if (!verificar_datos()) {   // comprobamos que los datos recogidos sean validos
    std::cerr << "Los datos de la imagen no son validos \n";
    exit(1);
  }

  archivo.ignore();  // ignoramos el salto de pagina que hay despues del maximo de intensidad.

  // Leemos los bytes de los pixeles de la imagen y los metemos en su vector correspondiente
  if (max_intensidad <= 255) {  // hay 1B por cada valor de cada vector RGB
    // usamos reinterpret_cast solo para las funciones read.
    // CAMBIARLOOOO: archivo.read(reinterpret_cast<char*>)//----___----------------------------------------- !!!!
  }

}


void ImageSOA::info(){
  // Esta funcion devuelve los metadatos de la imagen perteneciente a su clase

}


void ImageSOA::maxlevel(int nueva_intensidad) {
  // Esta función escala la intensidad de la imagen a la nueva intensidad dada. Tiene en cuenta los distintos tipos de RGB

}


void ImageSOA::resize(int nuevo_ancho, int nuevo_alto) {
  // Esta función escala el tamaño de la imagen a los nuevos valores de ancho y alto dado.

}

