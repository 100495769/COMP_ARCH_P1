#ifndef COMP_ARCH_LAB1_IMAGEAOS_H
#define COMP_ARCH_LAB1_IMAGEAOS_H
#include <string>
#include <iostream>
#include <utility>
#include <vector>

// Aquí irían las declaraciones de funciones, clases, etc.
// Definimos la clase ImageSOA con sus métodos y atributos
class ImageAOS {
  private:
  // Ponemos como atributos privados los correspondientes a los metadatos de la imagen
  std::string numero_magico; // sera la secuencia "P6"
  int ancho{}; // ancho de la imagen en decimal inicializada a 0
  int alto{}; // alto de la imagen en decimal inicializada a 0
  int max_intensidad{}; // máximo valor de intensidad inicializada a 0

  struct pixel{ // cada pixel tiene tres valores
    int red, green, blue;
  };

  struct SurroundingColoursAOS {
      pixel low_left;
      pixel low_right;
      pixel high_left;
      pixel high_right;
  };

  struct CoordenadasAOS {
      size_t x_low;
      size_t y_low;
      size_t x_high;
      size_t y_high;
  };

  // Estructura de datos en formato AOS con los pixeles de la imagen
  std::vector<pixel> vector_pixeles;

  public:
  // constructor
  ImageAOS();
  // Definimos los métodos:
  void info();  // 1) Informacion de los metadatos
  void maxlevel(int nueva_intensidad);  // 2) Escalado de intensidad
  //void resize(int nuevo_ancho, int nuevo_alto);  // 3) Escalado de tamaño
  // ICIAR :  4) Elimiacion de colores menos frecuentes
  // ICIAR :  5) Comprimir

  // Declaramos dos metodos para cargar y guardar la imagen desde el path del archivo
  void cargar_imagen_aos(std::string & path_imagen);
  void guardar_imagen_aos(std::string & path_imagen);
  auto verificar_datos_aos() -> bool;
  void resize_aos(int nuevo_ancho, int nuevo_alto);
  void pixel_assessment_aos(CoordenadasAOS& coordenadas, SurroundingColoursAOS& surrounding_colours);
  void copy_contents_aos(std::vector<pixel> nuevo_vector_pixeles);
  std::array<float, 3> interpolation_aos(SurroundingColoursAOS& surrounding_colours, float x_original, float y_original);
  std::vector<float> interpolation_aos(SurroundingColoursAOS& surrounding_colours, float x_original, float y_original);
  void cutfreq(int n_elimaciones);
};

#endif //COMP_ARCH_LAB1_IMAGEAOS_H
