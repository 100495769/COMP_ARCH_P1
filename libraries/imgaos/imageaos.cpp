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

// this function gets the rgb values for the 4 nearby pixels that are going to be interpolated later
void ImageAOS::pixel_assessment_aos(CoordenadasAOS& coordenadas, SurroundingColoursAOS& surrounding_colours) {
  // since we store pixels in 2D array flattened into 1D array -> accessing by y*width + x
  // y -> row number (altura), x -> column number (anchura)
  size_t ancho_casted = static_cast<size_t>(ancho);
  // bottom left pixel assessment
  surrounding_colours.low_left = vector_pixeles[coordenadas.y_low * ancho_casted + coordenadas.x_low];
  // bottom right pixel assessment
  surrounding_colours.low_right = vector_pixeles[coordenadas.y_low * ancho_casted + coordenadas.x_high];
  // top left pixel assessment
  surrounding_colours.high_left = vector_pixeles[coordenadas.y_high * ancho_casted + coordenadas.x_low];
  // top right pixel assessment
  surrounding_colours.high_right = vector_pixeles[coordenadas.y_high * ancho_casted + coordenadas.x_high];
}

void ImageAOS::copy_contents_aos(std::vector<pixel> nuevo_vector_pixeles) {

  vector_pixeles.resize(nuevo_vector_pixeles.size());
  std::copy(nuevo_vector_pixeles.begin(), nuevo_vector_pixeles.end(), vector_pixeles.begin());

  nuevo_vector_pixeles.clear();
}

std::vector<float> ImageAOS::interpolation_aos(SurroundingColoursAOS& surrounding_colours, float x_original, float y_original){

  float frac_part_x = x_original - std::floor(x_original); // calculating the fraction parts
  float frac_part_y = y_original - std::floor(y_original);
  // formula used for interpolation: C_low = (C_low_right - C_low_left)*x + C_low_left,
  // where (C_low_right - C_low_left) is the range, C_low_left - starting point and x the fraction part
  // x-axis interpolation (low row)
  float red_bottom = static_cast<float>(surrounding_colours.low_left.red) + (static_cast<float>(surrounding_colours.low_right.red - surrounding_colours.low_left.red)) * frac_part_x;
  float green_bottom = static_cast<float>(surrounding_colours.low_left.green) + (static_cast<float>(surrounding_colours.low_right.green - surrounding_colours.low_left.green)) * frac_part_x;
  float blue_bottom = static_cast<float>(surrounding_colours.low_left.blue) + (static_cast<float>(surrounding_colours.low_right.blue - surrounding_colours.low_left.blue)) * frac_part_x;

  // x-axis interpolation (high row)
  float red_top = static_cast<float>(surrounding_colours.high_left.red) + (static_cast<float>(surrounding_colours.high_right.red - surrounding_colours.high_left.red)) * frac_part_x;
  float green_top = static_cast<float>(surrounding_colours.high_left.green) + (static_cast<float>(surrounding_colours.high_right.green - surrounding_colours.high_left.green)) * frac_part_x;
  float blue_top = static_cast<float>(surrounding_colours.high_left.blue) + (static_cast<float>(surrounding_colours.high_right.blue - surrounding_colours.high_left.blue)) * frac_part_x;

  // y-axis interpolation
  float red_final = red_bottom + (red_top - red_bottom) * frac_part_y;
  float green_final = green_bottom + (green_top - green_bottom) * frac_part_y;
  float blue_final = blue_bottom + (blue_top - blue_bottom) * frac_part_y;

  std::vector<float> final_colores = {red_final, green_final, blue_final};
  return final_colores;
}

void ImageAOS::resize_aos(int nuevo_ancho, int nuevo_alto) {// Esta función escala el tamaño de la imagen a los nuevos valores de ancho y alto dado.
  // proportion in between old a new size
  float proporcion_anchura = static_cast<float>(ancho)/static_cast<float>(nuevo_ancho);
  float proporcion_altura = static_cast<float>(alto)/static_cast<float>(nuevo_alto);

  int tamano_matriz = nuevo_alto * nuevo_ancho; // changing the matrix size accordingly
  auto tamano_matriz_unsigned = static_cast<size_t>(tamano_matriz);

  std::vector<pixel> nuevo_vector_pixeles;
  nuevo_vector_pixeles.resize(tamano_matriz_unsigned);

  for (int nuevo_y = 0; nuevo_y < nuevo_alto; nuevo_y++) {  //iterating through each position in new coordinates
    for (int nuevo_x =0; nuevo_x < nuevo_ancho; nuevo_x++) {
      //taking the initial x and y values
      float x_original = static_cast<float>(nuevo_x) * proporcion_anchura;
      float y_original = static_cast<float>(nuevo_y) * proporcion_altura;
      //taking the coordinates of 4 pixeles más proximos (floor x ceiling functions)
      CoordenadasAOS coordenadas;

      coordenadas.x_low = static_cast<size_t>(std::floor(x_original));
      coordenadas.x_high = static_cast<size_t>(std::ceil(x_original));
      coordenadas.y_low = static_cast<size_t>(std::floor(y_original));
      coordenadas.y_high = static_cast<size_t>(std::ceil(y_original));

      SurroundingColoursAOS surrounding_colours;
      pixel_assessment_aos(coordenadas, surrounding_colours); // filling up the structure of colors for these surrounding pixels
      //interpolating colors by x and y axes
      std::vector<float> final_colores = interpolation_aos(surrounding_colours, x_original, y_original);
      // filling up new matrices with intepolated colors
      size_t nueva_posicion = (static_cast<size_t>(nuevo_y * nuevo_ancho + nuevo_x));

      pixel nuevo_pixel;
      nuevo_pixel.red = static_cast<int>(final_colores[0]);
      nuevo_pixel.green = static_cast<int>(final_colores[1]);
      nuevo_pixel.blue = static_cast<int>(final_colores[2]);

      nuevo_vector_pixeles[nueva_posicion] = nuevo_pixel;
    }
  }
  copy_contents_aos(nuevo_vector_pixeles);
  ancho = nuevo_ancho;
  alto = nuevo_alto;
}