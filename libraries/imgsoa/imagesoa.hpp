#ifndef COMP_ARCH_LAB1_IMAGESOA_H
#define COMP_ARCH_LAB1_IMAGESOA_H
#include <string>
#include <iostream>
#include <utility>
#include <vector>

struct SurroundingColours {
    int red_low_left, green_low_left, blue_low_left;
    int red_low_right, green_low_right, blue_low_right;
    int red_high_left, green_high_left, blue_high_left;
    int red_high_right, green_high_right, blue_high_right;
};

// Aquí irían las declaraciones de funciones, clases, etc.
// Definimos la clase ImageSOA con sus métodos y atributos
class ImageSOA {
  private:
  // Ponemos como atributos privados los correspondientes a los metadatos de la imagen
  std::string numero_magico; // sera la secuencia "P6"
  int ancho{}; // ancho de la imagen en decimal inicializada a 0
  int alto{}; // alto de la imagen en decimal inicializada a 0
  int max_intensidad{}; // máximo valor de intensidad inicializada a 0
  std::vector<int> red, green, blue;  // Estructura de datos en formato SOA con los pixeles de la imagen

  public:
  // constructor
  ImageSOA();
  // Definimos los métodos:
  void info();  // 1) Informacion de los metadatos
  void maxlevel(int nueva_intensidad);  // 2) Escalado de intensidad
  //void resize(int nuevo_ancho, int nuevo_alto);  // 3) Escalado de tamaño
  // ICIAR :  4) Elimiacion de colores menos frecuentes
  // ICIAR :  5) Comprimir

  // Declaramos dos metodos para cargar y guardar la imagen desde el path del archivo
  void cargar_imagen(std::string & path_imagen);
  void guardar_imagen(std::string & path_imagen);
  auto verificar_datos() -> bool;
  void resize(int nuevo_ancho, int nuevo_alto);
  void pixel_assessment(size_t x_low, size_t y_low, size_t x_high, size_t y_high, SurroundingColours& surrounding_colours);
  void copy_contents(std::vector<int> &nuevo_red, std::vector<int> &nuevo_green, std::vector<int> &nuevo_blue);
  std::vector<float> interpolation(SurroundingColours& surrounding_colours, float x_original, float y_original);
};

#endif //COMP_ARCH_LAB1_IMAGESOA_H
