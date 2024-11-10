//
// Created by sergio on 7/10/24.
//
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include "imageaos.hpp"

ImageAOS::ImageAOS() = default;

auto ImageAOS::verificar_datos_aos() -> bool {
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


void ImageAOS::cargar_imagen_aos(std::string& path_imagen) {    // Esta función abre la imagen desde el path dado.
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

  if (!verificar_datos_aos()) {   // comprobamos que los datos recogidos sean validos
    std::cerr << "Los datos de la imagen no son validos \n";
    exit(1);
  }
  archivo.ignore();  // ignoramos el salto de pagina que hay despues del maximo de intensidad.
  // ahora gestionamos los pixeles: tenemos que recorrer cada iteracion y recoger el valor de cada pixel
  int tamano_matriz = alto * ancho;
  auto tamano_matriz_unsigned = static_cast<size_t>(tamano_matriz);
  vector_pixeles.resize(tamano_matriz_unsigned);

  // Leemos los bytes de los pixeles de la imagen y los metemos en su struct correspondiente
  for (size_t i = 0; i < tamano_matriz_unsigned; i++) {// usamos reinterpret_cast solo para las funciones read: de esta forma meteremos 8bits u 16 bits dependiendo de la imagen
    if (max_intensidad <= 255) {  // hay 1B (usamos el tipo uint8_t) por cada valor de cada vector RGB
      archivo.read(reinterpret_cast<char*>(&vector_pixeles[i].red), sizeof(uint8_t));
      archivo.read(reinterpret_cast<char*>(&vector_pixeles[i].green), sizeof(uint8_t));
      archivo.read(reinterpret_cast<char*>(&vector_pixeles[i].blue), sizeof(uint8_t));
    }
    else {  // hay 2B (usamos el tipo uint16_t) por cada valor de cada vector RGB
      archivo.read(reinterpret_cast<char*>(&vector_pixeles[i].red), sizeof(uint16_t));
      archivo.read(reinterpret_cast<char*>(&vector_pixeles[i].green), sizeof(uint16_t));
      archivo.read(reinterpret_cast<char*>(&vector_pixeles[i].blue), sizeof(uint16_t));
    }
  }
}

void ImageAOS::guardar_imagen_aos(std::string& path_salida) {    // Esta función guarda la imagen en el path dado.
  std::ofstream archivo(path_salida, std::ios::binary);  // lo ponemos en binario para escribir bytes

  if (!archivo.is_open()) {   // salimos de la ejecucion si hay errores
    std::cerr <<"Error: No se puede abrir el archivo dado en el path: " << path_salida << "\n";
    exit(1);
  }
  // escribimos los metadatos separados por espacios (salto de linea para el numero magico y la intensidad)
  archivo << numero_magico << "\n" << ancho << " " << alto  << " " << max_intensidad << "\n";

  // ahora gestionamos la escritura de la matriz de pixeles dependiendo de su intensidad maxima
  for (auto & vector_pixele : vector_pixeles) {
    if(max_intensidad <= 255) {  // escribimos 1B (usamos el tipo uint8_t) para cada valor de cada vector
      archivo.write(reinterpret_cast<char*>(&vector_pixele.red), sizeof(uint8_t));
      archivo.write(reinterpret_cast<char*>(&vector_pixele.green), sizeof(uint8_t));
      archivo.write(reinterpret_cast<char*>(&vector_pixele.blue), sizeof(uint8_t));
    }
    else {   // escribimos 2B (usamos el tipo uint16_t) para cada valor de cada vector
      archivo.write(reinterpret_cast<char*>(&vector_pixele.red), sizeof(uint16_t));
      archivo.write(reinterpret_cast<char*>(&vector_pixele.green), sizeof(uint16_t));
      archivo.write(reinterpret_cast<char*>(&vector_pixele.blue), sizeof(uint16_t));
    }
  }
  archivo.close();
}

void ImageAOS::info(){    // Esta funcion devuelve los metadatos de la imagen perteneciente a su clase
  std::cout << "Magic number: " << numero_magico << "\n" << "Ancho: " << ancho << "\n" << "Alto: " << alto << "\n" << "Maximo intensidad: " << max_intensidad << "\n";
}

void ImageAOS::maxlevel(int nueva_intensidad) {     // Esta función escala la intensidad de la imagen a la nueva intensidad dada
  // realmente en estas funciones no vamos a gestionar si la intensidad es mayor o menor que 255. Eso lo gestionamos al guardarlo
  for (auto & vector_pixele : vector_pixeles) {
    int nuevo_valor_red = (vector_pixele.red * nueva_intensidad) / max_intensidad;
    int nuevo_valor_green = (vector_pixele.green * nueva_intensidad) / max_intensidad;
    int nuevo_valor_blue = (vector_pixele.blue * nueva_intensidad) / max_intensidad;
    vector_pixele.red = nuevo_valor_red;
    vector_pixele.green = nuevo_valor_green;
    vector_pixele.blue = nuevo_valor_blue;
  }
}

/*
void ImageAOS::resize(int nuevo_ancho, int nuevo_alto) {      // Esta función escala el tamaño de la imagen a los nuevos valores de ancho y alto dado.

}
*/