//
// Created by sergio on 7/10/24.
//
#include "imagesoa.hpp"

#include <algorithm>
#include <bitset>
//#include <cmake-build-debug/_deps/googletest-src/googletest/src/gtest-internal-inl.h>
#include "b_tree.hpp"

#include <chrono>
#include <cmath>
#include "b_tree.hpp"
#include "calcular_prox_euclideo.hpp"
#include "imagesoa.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <complex>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <progargs.hpp>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include "imagesoa.hpp"
#include "b_tree.hpp"
#include <chrono>

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
  max_intensidad = nueva_intensidad;
}
// this function gets the rgb values for the 4 nearby pixels that are going to be interpolated later
void ImageSOA::pixel_assessment(CoordenadasSOA& coordenadas, SurroundingColoursSOA& surrounding_colours) {

  // since we store pixels in 2D array flattened into 1D array -> accessing by y*width + x
  // y -> row number (altura), x -> column number (anchura)
  auto ancho_casted = static_cast<size_t>(ancho);
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
  std::ranges::copy(nuevo_red.begin(), nuevo_red.end(), red.begin());
  std::ranges::copy(nuevo_green.begin(), nuevo_green.end(), green.begin());
  std::ranges::copy(nuevo_blue.begin(), nuevo_blue.end(), blue.begin());

  // clear up new contents
  nuevo_red.clear();
  nuevo_green.clear();
  nuevo_blue.clear();
}

auto ImageSOA::interpolation(SurroundingColoursSOA& surrounding_colours, float x_original, float y_original) -> std::array<float, 3>{

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
  if (nuevo_ancho <=0 or nuevo_alto <=0) {
    std::cerr << "Nueva altura y anchura no puede ser menor o igual a cero!\n";
  }
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
      auto nueva_posicion = static_cast<size_t>(nuevo_y * nuevo_ancho + nuevo_x);
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

auto ImageSOA::compress() -> std::tuple<size_t, std::vector<std::string>> {
  // Función que creará el contenido comprimido de la imagen.
  // comprimida
  std::unordered_map<std::string, std::string> coloresUnicos{};  // conjunto de colores únicos que hemos visto en la imagen

  int nowIndice = 0;

  // en este bucle se genera el texto con los valores de los pixeles en la imagen comprimida
  for (size_t i = 0; i < red.size(); i++) {
    // en colores guardamos los tres colores
    std::string color = std::to_string(red[i]) + std::to_string(green[i]) +
                        std::to_string(blue[i]);
    if (coloresUnicos.find(color) == coloresUnicos.end()) {
      // no se ha visto este color antes, lo añadimos al texto y al mapa
      coloresUnicos[color] = std::to_string(nowIndice);
      nowIndice++;
    }
  }

  size_t numColoresUnicos = coloresUnicos.size();

  std::unordered_map<std::string, std::string> bin = tablaIndices(numColoresUnicos, coloresUnicos);

  std::vector<std::string> pixelIndices;
  // guardamos secuencia de indices de los colores en el texto
  pixelIndices.resize(red.size());
  // mínimo vamos a necesitar 1B parnuma representar todos los indices
  for (size_t i = 0; i < red.size(); i++) {
    std::string color = std::to_string(red[i]) + std::to_string(green[i]) +
                       std::to_string(blue[i]);
    pixelIndices[i] = bin[color];
  }
  return {numColoresUnicos, pixelIndices};
}


void ImageSOA::guardar_compress(const std::string& nombre_fichero, const std::tuple<size_t, std::vector<std::string>>& elem) const {
  std::ofstream archivo(nombre_fichero, std::ios::binary);
  if (!archivo.is_open()) {   // salimos de la ejecucion si hay errores
    std::cerr <<"Error: No se puede abrir el archivo dado en el path: " << nombre_fichero << "\n";
    exit(1);
  }
  else if (!nombre_fichero.ends_with(".cppm")) {
    std::cerr <<"Error: La extensión del fichero de salida no es la deseada";
    exit(1);
  }
  auto [size, pixelIndices] = elem;
  // escribimos los metadatos separados por espacios (salto de linea para el numero magico y la intensidad)
  archivo << "C6"<< " " << ancho << " " << alto  << " " << max_intensidad << " " << std::to_string(size) << "\n";
  //archivo << colores << "\n";
  for (size_t i = 0; i < red.size(); i++) {
    if(max_intensidad <= 255) {  // escribimos 1B (usamos el tipo uint8_t) para cada valor de cada vector
      archivo.write(reinterpret_cast<const char*>(&red[i]), sizeof(uint8_t));
      archivo.write(reinterpret_cast<const char*>(&green[i]), sizeof(uint8_t));
      archivo.write(reinterpret_cast<const char*>(&blue[i]), sizeof(uint8_t));
    }
    else {   // escribimos 2B (usamos el tipo uint16_t) para cada valor de cada vector
      archivo.write(reinterpret_cast<const char *>(&red[i]), sizeof(uint16_t));
      archivo.write(reinterpret_cast<const char *>(&green[i]), sizeof(uint16_t));
      archivo.write(reinterpret_cast<const char *>(&blue[i]), sizeof(uint16_t));
    }
  }
  if (size <= 255) {
    for (size_t i = 0; i < red.size(); i++) {
      archivo.write(reinterpret_cast<char*>(&pixelIndices[i]), sizeof(uint8_t));
    }
  }
  else if (size > 255 && size < 65536) {
    for (size_t i = 0; i < red.size(); i++) {
      archivo.write(reinterpret_cast<char*>(&pixelIndices[i]), sizeof(uint16_t));
    }
  }
  else {
    for (size_t i = 0; i < red.size(); i++) {
      archivo.write(reinterpret_cast<char*>(&pixelIndices[i]), sizeof(uint32_t));
    }
  }
  archivo.close();
}
/*auto ImageSOA::compare(std::uint8_t  r_new, std::uint8_t  g_new, std::uint8_t  b_new, std::uint8_t  r_old, std::uint8_t  g_old, std::uint8_t  b_old) -> int{
  // Retornea 2 si el new es mayor, 1 si el old es mayor y 0 si son iguales.
  auto new_key = std::tie(b_new, g_new, r_new);
  auto old_key = std::tie(b_old, g_old, r_old);

  if (new_key > old_key) return 2;
    if (old_key > new_key) return 1;
    return 0;

}

bool ImageSOA::radar_search(int & pos, int & x, int & y ,std::unordered_map<std::vector<std::uint8_t>,std::vector<std::uint8_t>, nuevo_hash> &parejas_colores, std::unordered_map<std::vector<std::uint8_t>, int, nuevo_hash> & colores ){
  // Cuando btee compare si da 2 el de la izq> si da 1 el de la der y si da 0 son iguales
  bool found_color = false;
  int pos_check = 0;
  //En nuestra funcion vamos a ir restando de x y sumandole a y.
      for (int sign_x = -1; sign_x <= 1; sign_x += 2) {
        for (int sign_y = -1; sign_y <= 1; sign_y += 2) {
          // Antes de comprobar si ese color existe comprobamos que este la posicion en la imagen. Que sino da errores.
          pos_check = pos + sign_x * x + sign_y * y * ancho;
          if ((0 <= pos_check) &&
              (pos_check <= alto*ancho)) {
            if (colores[{red[pos_check], green[pos_check], blue[pos_check]}] == 1){
              parejas_colores[{red[pos], green[pos], blue[pos]}] = {red[pos_check], green[pos_check], blue[pos_check]};
              found_color = true;
            }
          }

          pos_check = pos+y*sign_y+x*sign_x*ancho;
          if ((0 <= pos_check) &&
              (pos_check <= alto*ancho)) {
            if (colores[{red[pos_check], green[pos_check], blue[pos_check]}] == 1){
              if (found_color == true) {
                std::uint8_t old_red = parejas_colores[{red[pos], green[pos], blue[pos]}][0];
                std::uint8_t old_green = parejas_colores[{red[pos], green[pos], blue[pos]}][1];
                std::uint8_t old_blue = parejas_colores[{red[pos], green[pos], blue[pos]}][2];
                if(compare(red[pos_check], green[pos_check], blue[pos_check], old_red, old_green, old_blue) == 2){
                  parejas_colores[{red[pos], green[pos], blue[pos]}] = {
                    red[pos_check], green[pos_check], blue[pos_check]};
                }
              }
              else {
                parejas_colores[{red[pos], green[pos], blue[pos]}] = {
                  red[pos_check], green[pos_check], blue[pos_check]};
                found_color = true;
              }
            }
          }
      }
  }
  return found_color;
}

void ImageSOA::cutfreq(int n) {
  std::unordered_map<std::vector<std::uint8_t>, int, nuevo_hash> mis_colores;
  b_tree arbol_de_apariciones;
  arbol_de_apariciones.rellenar_datos(&red, &blue, &green, ancho, alto);
  std::vector<int> cantidad_de_repeticiones; // En el 0 los que haya solo una vez. En el 1 los que haya dos veces, etc.
  std::size_t pixeles_en_imagen = static_cast<size_t>(ImageSOA::ancho*ImageSOA::alto);
  std::size_t tamaño_repeticiones = 0;

  for(size_t i = 0; i < pixeles_en_imagen; i++){
    int repeticion = arbol_de_apariciones.insertar(red[i], green[i], blue[i], static_cast<int>(i));
    //std::cout<<repeticion<<std::endl;

    if (repeticion > cantidad_de_repeticiones.size()){
      if (tamaño_repeticiones != 0){
        cantidad_de_repeticiones.back() --;
      }
      tamaño_repeticiones++;
      cantidad_de_repeticiones.push_back(1);
    }
    else {
      if (repeticion != 1) {
        cantidad_de_repeticiones[repeticion - 1]++;
        cantidad_de_repeticiones[repeticion - 2]--;
      } else {
        cantidad_de_repeticiones[0]++;
      }
    }
    //std::cout<<cantidad_de_repeticiones.size()<<"  "<<cantidad_de_repeticiones[0]<<std::endl;
  }
  //Voy a calcular hasta que repeticion me quito.
  std::size_t i = 0;
  bool cabe_mas = true;
  int total_repeticiones = 0;
  while (i < tamaño_repeticiones){
    total_repeticiones += cantidad_de_repeticiones[i];
    i++;
  }
  i = 0;
   std::cout<<"Colores: "<<total_repeticiones<<std::endl;

  while (i < cantidad_de_repeticiones.size() && cabe_mas){
    std::cout<<"Cantidad de repeticiones para: "<<i<<" "<<cantidad_de_repeticiones[i];
    if (n > cantidad_de_repeticiones[i]){
      n -= cantidad_de_repeticiones[i];
      i++;

    }
    else {
      cabe_mas = false;
    }
  }
  std::cout << i<< "  " << cantidad_de_repeticiones.size()<<"acatoy"<<std::endl;
  if (i >= cantidad_de_repeticiones.size()){
    std::cerr<<"Cutfreq: hay mas valores a eliminar que colores."<<std::endl;
    return;
  }
  arbol_de_apariciones.in_order_del_unfreq(n, i);
  std::vector<std::vector<std::uint8_t>> colores_eliminados;
  std::vector<std::vector<std::uint8_t>> colores_no_eliminados;
  colores_eliminados = arbol_de_apariciones.lista_colores_eliminados();
  colores_no_eliminados = arbol_de_apariciones.lista_colores_no_eliminados();
  std::cout << "Tamaño colores_eliminados: "<< colores_eliminados.size() << " Tamaño colores_no_eliminados: "<<colores_no_eliminados.size()<<std::endl;

  for (auto col: colores_eliminados){
    mis_colores[col] = 0;
    //std::cout<<col[0]<<col[1]<<col[2]<<std::endl;
  }
  for (auto col : colores_no_eliminados){
    mis_colores[col] = 1;
  }
 std::cout<<mis_colores.size()<<std::endl;

 std::unordered_map<std::vector<std::uint8_t>, std::vector<std::uint8_t>, nuevo_hash> parejas_colores;
 int colores_restantes = colores_eliminados.size();
 calcular_prox_euclideo euclideo;

 while (colores_restantes > 0) {
   std::vector<int> distancias = euclideo.prox_euclideo();
   for (int pos = 0; pos < ancho * alto; pos++) {
     int found_color = false;
     if (mis_colores[{red[pos], green[pos], blue[pos]}] == 0) {
       found_color = radar_search(pos, distancias[0], distancias[1], parejas_colores, mis_colores);
       if (found_color == true){
         colores_restantes --;
         mis_colores[{red[pos], green[pos], blue[pos]}] = 2;
       }
     }
   }
 }
 for (int pos = 0; pos < ancho*alto; pos++){
   if (mis_colores[{red[pos], green[pos], blue[pos]}] == 2){
     std::vector<std::uint8_t > pareja = parejas_colores[{red[pos], green[pos], blue[pos]}];
     red[pos] = pareja[0];
     green[pos] = pareja[1];
     blue[pos] = pareja[2];
   }
 }




  auto start = std::chrono::high_resolution_clock::now();

  // En este vector en cada indice cuantos elementos hay repetidos indice veces.
  // Muy útil luego para sacar cosas ya lo comentare bien tod
  b_tree arbol_de_apariciones;
  arbol_de_apariciones.rellenar_datos(&red, &blue, &green, ancho, alto);
  std::vector<int> cantidad_de_repeticiones; // En el 0 los que haya solo una vez. En el 1 los que haya dos veces, etc.
  std::size_t pixeles_en_imagen = static_cast<size_t>(ImageSOA::ancho*ImageSOA::alto);
  std::size_t tamaño_repeticiones = 0;

  for(size_t i = 0; i < pixeles_en_imagen; i++){
    //int repeticion = arbol_de_apariciones.insertar(red[i], green[i], blue[i]);

    if (repeticion != 1){
      cantidad_de_repeticiones[repeticion-1]++;
      cantidad_de_repeticiones[repeticion-2]--;
    }
    else{
      cantidad_de_repeticiones[0];
    int repeticion = arbol_de_apariciones.insertar(red[i], green[i], blue[i], static_cast<int>(i));
    //std::cout<<repeticion<<std::endl;

    if (repeticion > cantidad_de_repeticiones.size()){
      if (tamaño_repeticiones != 0){
        cantidad_de_repeticiones.back() --;
      }
      tamaño_repeticiones++;
      cantidad_de_repeticiones.push_back(1);
    }
    else {
      if (repeticion != 1) {
        cantidad_de_repeticiones[repeticion - 1]++;
        cantidad_de_repeticiones[repeticion - 2]--;
      } else {
        cantidad_de_repeticiones[0]++;
      }
    }

  }
    //std::cout<<cantidad_de_repeticiones.size()<<"  "<<cantidad_de_repeticiones[0]<<std::endl;
  }


  //Voy a calcular hasta que repeticion me quito.
  std::size_t i = 0;
  bool cabe_mas = true;
  //start of the block of comments
  int total_repeticiones = 0;
  while (i < tamaño_repeticiones){
    total_repeticiones += cantidad_de_repeticiones[i];
    i++;
  }
  i = 0;
   std::cout<<"Colores: "<<total_repeticiones<<std::endl;
   //start of the block of comments

  while (i < cantidad_de_repeticiones.size() && cabe_mas){
    std::cout<<"Cantidad de repeticiones para: "<<i<<" "<<cantidad_de_repeticiones[i];
    if (n > cantidad_de_repeticiones[i]){
      n -= cantidad_de_repeticiones[i];
      i++;

    }
    else {
      cabe_mas = false;
    }
  }

  }
  auto end = std::chrono::high_resolution_clock::now();

  // Calcula la duración
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "Tiempo transcurrido: " << duration.count() << " microsegundos" << std::endl;
  std::cout << i<< "  " << cantidad_de_repeticiones.size()<<"acatoy"<<std::endl;
  if (i >= cantidad_de_repeticiones.size()){
    std::cerr<<"Cutfreq: hay mas valores a eliminar que colores."<<std::endl;
    return;
  }
  else{
    std::cout<<"si se puede"<<std::endl;
  }

  arbol_de_apariciones.in_order_del_unfreq(n, i);
  std::uint8_t red_del = arbol_de_apariciones.red_del;
  std::uint8_t blue_del = arbol_de_apariciones.blue_del;
  std::uint8_t green_del = arbol_de_apariciones.green_del;
  int tamano_imagen = ancho * alto;
  for (int i = 0; i < tamano_imagen; i++){
    if (i+1 < tamano_imagen){
      //Comprobamos el de la derecha.

    }
  }

  arbol_de_apariciones.in_order_fill_unfreq();*/
