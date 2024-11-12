//
// Created by sergio on 7/10/24.
//
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>
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
    int nuevo_valor_red = (red[i] * nueva_intensidad) / max_intensidad;
    int nuevo_valor_green = (green[i] * nueva_intensidad) / max_intensidad;
    int nuevo_valor_blue = (blue[i] * nueva_intensidad) / max_intensidad;
    red[i] = static_cast<uint8_t>(nuevo_valor_red);
    green[i] = static_cast<uint8_t>(nuevo_valor_green);
    blue[i] = static_cast<uint8_t>(nuevo_valor_blue);
  }
}
// this function gets the rgb values for the 4 nearby pixels that are going to be interpolated later
void ImageSOA::pixel_assessment(CoordenadasSOA& coordenadas, SurroundingColoursSOA& surrounding_colours) {

  // since we store pixels in 2D array flattened into 1D array -> accessing by y*width + x
  // y -> row number (altura), x -> column number (anchura)
  size_t ancho_casted = static_cast<size_t>(ancho);
  // bottom left pixel assessment
  surrounding_colours.red_low_left = red[coordenadas.y_low * ancho_casted + coordenadas.x_low];
  surrounding_colours.green_low_left = green[coordenadas.y_low * static_cast<size_t>(ancho_casted) + coordenadas.x_low];
  surrounding_colours.blue_low_left = blue[coordenadas.y_low * ancho_casted + coordenadas.x_low];

  // bottom right pixel assessment
  surrounding_colours.red_low_right = red[coordenadas.y_low * ancho_casted + coordenadas.x_high];
  surrounding_colours.green_low_right = green[coordenadas.y_low * ancho_casted + coordenadas.x_high];
  surrounding_colours.blue_low_right = blue[coordenadas.y_low * ancho_casted + coordenadas.x_high];

  // top left pixel assessment
  surrounding_colours.red_high_left = red[coordenadas.y_high * ancho_casted + coordenadas.x_low];
  surrounding_colours.green_high_left = green[coordenadas.y_high * ancho_casted + coordenadas.x_low];
  surrounding_colours.blue_high_left = blue[coordenadas.y_high * ancho_casted + coordenadas.x_low];

  // top right pixel assessment
  surrounding_colours.red_high_right = red[coordenadas.y_high * ancho_casted + coordenadas.x_high];
  surrounding_colours.green_high_right = green[coordenadas.y_high * ancho_casted + coordenadas.x_high];
  surrounding_colours.blue_high_right = blue[coordenadas.y_high * ancho_casted + coordenadas.x_high];
}

void ImageSOA::copy_contents(std::vector<int> &nuevo_red, std::vector<int> &nuevo_green, std::vector<int> &nuevo_blue) {
  // resize vectors to the size of new ones
  red.resize(nuevo_red.size());
  green.resize(nuevo_green.size());
  blue.resize(nuevo_blue.size());

  // copy contents from new rgb vectors into the old ones
  std::copy(nuevo_red.begin(), nuevo_red.end(), red.begin());
  std::copy(nuevo_green.begin(), nuevo_green.end(), green.begin());
  std::copy(nuevo_blue.begin(), nuevo_blue.end(), blue.begin());

  // clear up new contents
  nuevo_red.clear();
  nuevo_green.clear();
  nuevo_blue.clear();
}

std::array<float, 3> ImageSOA::interpolation(SurroundingColoursSOA& surrounding_colours, float x_original, float y_original){

  float frac_part_x = x_original - std::floor(x_original); // calculating the fraction parts
  float frac_part_y = y_original - std::floor(y_original);
  // formula used for interpolation: C_low = (C_low_right - C_low_left)*x + C_low_left,
  // where (C_low_right - C_low_left) is the range, C_low_left - starting point and x the fraction part
  // x-axis interpolation (low row)
  float red_bottom = static_cast<float>(surrounding_colours.red_low_left) + (static_cast<float>(surrounding_colours.red_low_right - surrounding_colours.red_low_left)) * frac_part_x;
  float green_bottom = static_cast<float>(surrounding_colours.green_low_left) + (static_cast<float>(surrounding_colours.green_low_right - surrounding_colours.green_low_left)) * frac_part_x;
  float blue_bottom = static_cast<float>(surrounding_colours.blue_low_left) + (static_cast<float>(surrounding_colours.blue_low_right - surrounding_colours.blue_low_left)) * frac_part_x;

  // x-axis interpolation (high row)
  float red_top = static_cast<float>(surrounding_colours.red_high_left) + (static_cast<float>(surrounding_colours.red_high_right - surrounding_colours.red_high_left)) * frac_part_x;
  float green_top = static_cast<float>(surrounding_colours.green_high_left) + (static_cast<float>(surrounding_colours.green_high_right - surrounding_colours.green_high_left)) * frac_part_x;
  float blue_top = static_cast<float>(surrounding_colours.blue_high_left) + (static_cast<float>(surrounding_colours.blue_high_right - surrounding_colours.blue_high_left)) * frac_part_x;

  // y-axis interpolation
  float red_final = red_bottom + (red_top - red_bottom) * frac_part_y;
  float green_final = green_bottom + (green_top - green_bottom) * frac_part_y;
  float blue_final = blue_bottom + (blue_top - blue_bottom) * frac_part_y;

  return {red_final, green_final, blue_final};
}

void ImageSOA::resize(int nuevo_ancho, int nuevo_alto) {// Esta función escala el tamaño de la imagen a los nuevos valores de ancho y alto dado.
  // proportion in between old a new size
  float proporcion_anchura = static_cast<float>(ancho)/static_cast<float>(nuevo_ancho);
  float proporcion_altura = static_cast<float>(alto)/static_cast<float>(nuevo_alto);

  int tamano_matriz = nuevo_alto * nuevo_ancho; // changing the matrix size accordingly
  auto tamano_matriz_unsigned = static_cast<size_t>(tamano_matriz);
  std::vector<int> nuevo_red, nuevo_green, nuevo_blue;
  nuevo_red.resize(tamano_matriz_unsigned);  //changing the new rgb matrices size
  nuevo_green.resize(tamano_matriz_unsigned);
  nuevo_blue.resize(tamano_matriz_unsigned);

  for (int nuevo_y = 0; nuevo_y < nuevo_alto; nuevo_y++) {  //iterating through each position in new coordinates
    for (int nuevo_x =0; nuevo_x < nuevo_ancho; nuevo_x++) {
      //taking the initial x and y values
      float x_original = static_cast<float>(nuevo_x) * proporcion_anchura;
      float y_original = static_cast<float>(nuevo_y) * proporcion_altura;
      //taking the coordinates of 4 pixeles más proximos (floor x ceiling functions)
      CoordenadasSOA coordenadas;
      coordenadas.x_low = static_cast<size_t>(std::floor(x_original));
      coordenadas.x_high = static_cast<size_t>(std::ceil(x_original));
      coordenadas.y_low = static_cast<size_t>(std::floor(y_original));
      coordenadas.y_high = static_cast<size_t>(std::ceil(y_original));

      SurroundingColoursSOA surrounding_colours;
      pixel_assessment(coordenadas, surrounding_colours); // filling up the structure of colors for these surrounding pixels
      //interpolating colors by x and y axes
      std::array<float, 3>  final_colores = interpolation(surrounding_colours, x_original, y_original);
      // filling up new matrices with intepolated colors
      size_t nueva_posicion = static_cast<size_t>(nuevo_y * nuevo_ancho + nuevo_x);
      nuevo_red[nueva_posicion] = static_cast<int>(final_colores[0]);
      nuevo_green[nueva_posicion] = static_cast<int>(final_colores[1]);
      nuevo_blue[nueva_posicion] = static_cast<int>(final_colores[2]);
    }
  }
  // changing values in the initial matrix to the values of a new one
  copy_contents(nuevo_red, nuevo_green, nuevo_blue);
  ancho = nuevo_ancho;
  alto = nuevo_alto;
}