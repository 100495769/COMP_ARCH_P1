//
// Created by sergio on 7/10/24.
//
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include "imagesoa.hpp"

ImageSOA::ImageSOA() = default;

auto ImageSOA::verificar_datos() -> bool {
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


void ImageSOA::cargar_imagen(std::string& path_imagen) {    // Esta función abre la imagen desde el path dado.
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
  // ahora gestionamos la matriz de pixeles: ponemos cada array al tamaño que corresponda
  // usaremos static_casts para que no haya problemas en los cambios de int a size_t
  int tamano_matriz = alto * ancho;
  auto tamano_matriz_unsigned = static_cast<size_t>(tamano_matriz);
  red.resize(tamano_matriz_unsigned);
  //green.resize(static_cast<std::vector<int>::size_type>(tamano_matriz));
  green.resize(tamano_matriz_unsigned);
  blue.resize(tamano_matriz_unsigned);

  // Leemos los bytes de los pixeles de la imagen y los metemos en su vector correspondiente
  for (size_t i = 0; i < tamano_matriz_unsigned; i++) {// usamos reinterpret_cast solo para las funciones read: de esta forma meteremos 8bits u 16 bits dependiendo de la imagen
    if (max_intensidad <= 255) {  // hay 1B (usamos el tipo uint8_t) por cada valor de cada vector RGB
      archivo.read(reinterpret_cast<char*>(&red[i]), sizeof(uint8_t));
      archivo.read(reinterpret_cast<char*>(&green[i]), sizeof(uint8_t));
      archivo.read(reinterpret_cast<char*>(&blue[i]), sizeof(uint8_t));
    }
    else {  // hay 2B (usamos el tipo uint16_t) por cada valor de cada vector RGB
      archivo.read(reinterpret_cast<char*>(&red[i]), sizeof(uint16_t));
      archivo.read(reinterpret_cast<char*>(&green[i]), sizeof(uint16_t));
      archivo.read(reinterpret_cast<char*>(&blue[i]), sizeof(uint16_t));
    }
  }
}

void ImageSOA::guardar_imagen(std::string& path_salida) {    // Esta función guarda la imagen en el path dado.
  std::ofstream archivo(path_salida, std::ios::binary);  // lo ponemos en binario para escribir bytes

  if (!archivo.is_open()) {   // salimos de la ejecucion si hay errores
    std::cerr <<"Error: No se puede abrir el archivo dado en el path: " << path_salida << "\n";
    exit(1);
  }
  // escribimos los metadatos separados por espacios (salto de linea para el numero magico y la intensidad)
  archivo << numero_magico << "\n" << ancho << " " << alto  << " " << max_intensidad << "\n";

  // ahora gestionamos la escritura de la matriz de pixeles dependiendo de su intensidad maxima
  for (size_t i = 0; i < red.size(); i++) {
    if(max_intensidad <= 255) {  // escribimos 1B (usamos el tipo uint8_t) para cada valor de cada vector
      archivo.write(reinterpret_cast<char*>(&red[i]), sizeof(uint8_t));
      archivo.write(reinterpret_cast<char*>(&green[i]), sizeof(uint8_t));
      archivo.write(reinterpret_cast<char*>(&blue[i]), sizeof(uint8_t));
    }
    else {   // escribimos 2B (usamos el tipo uint16_t) para cada valor de cada vector
      archivo.write(reinterpret_cast<char*>(&red[i]), sizeof(uint16_t));
      archivo.write(reinterpret_cast<char*>(&green[i]), sizeof(uint16_t));
      archivo.write(reinterpret_cast<char*>(&blue[i]), sizeof(uint16_t));
    }
  }
  archivo.close();
}

void ImageSOA::info(){    // Esta funcion devuelve los metadatos de la imagen perteneciente a su clase
  std::cout << "Magic number: " << numero_magico << "\n" << "Ancho: " << ancho << "\n" << "Alto: " << alto << "\n" << "Maximo intensidad: " << max_intensidad << "\n";
}

void ImageSOA::maxlevel(int nueva_intensidad) {     // Esta función escala la intensidad de la imagen a la nueva intensidad dada
  // realmente en estas funciones no vamos a gestionar si la intensidad es mayor o menor que 255. Eso lo gestionamos al guardarlo
  for (size_t i = 0; i < red.size(); i++) {   // recorremos los vectores de pixeles (todos tienen el mismo tamaño por lo que hago solo un bucle)
    int nuevo_valor_red = red[i] * (nueva_intensidad / max_intensidad);
    int nuevo_valor_green = green[i] * (nueva_intensidad / max_intensidad);
    int nuevo_valor_blue = blue[i] * (nueva_intensidad / max_intensidad);
    red[i] = nuevo_valor_red;
    green[i] = nuevo_valor_green;
    blue[i] = nuevo_valor_blue;
  }
}

/*
void ImageSOA::resize(int nuevo_ancho, int nuevo_alto) {      // Esta función escala el tamaño de la imagen a los nuevos valores de ancho y alto dado.

}
*/