#ifndef COMP_ARCH_LAB1_IMAGEAOS_H
#define COMP_ARCH_LAB1_IMAGEAOS_H
#include <iostream>
#include <string>
#include <bitset>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <iostream>
#include <utility>
#include <vector>
#include <array>

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
    uint8_t red, green, blue;
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
  void info();
  void maxlevel(int nueva_intensidad);
  // Declaramos dos metodos para cargar y guardar la imagen desde el path del archivo
  void cargar_imagen_aos(std::string & path_imagen);
  void guardar_imagen_aos(std::string & path_imagen);
  auto verificar_datos_aos() -> bool;

  void resize_aos(int nuevo_ancho, int nuevo_alto);
  void pixel_assessment_aos(CoordenadasAOS& coordenadas, SurroundingColoursAOS& surrounding_colours);
  void copy_contents_aos(std::vector<pixel> nuevo_vector_pixeles);
  auto interpolation_aos(SurroundingColoursAOS& surrounding_colours, float x_original, float y_original) -> std::array<float, 3> ;

  auto compress() -> std::tuple<size_t, std::vector<std::string>>;
  /*auto tablaIndices(size_t num, std::unordered_map<std::string, std::string> coloresUnicos)
      -> std::unordered_map<std::string, std::string>;*/
  void guardar_compress(std::string const & nombre_fichero,
                        std::tuple<size_t, std::vector<std::string>> const & elem) const;

    // definimos getters para los atributos privados
    [[nodiscard]] auto get_numero_magico() const -> std::string { return numero_magico; }
    [[nodiscard]] auto get_ancho() const -> int { return ancho; }
    [[nodiscard]] auto get_alto() const -> int { return alto; }
    [[nodiscard]] auto get_max_intensidad() const -> int { return max_intensidad; }
    [[nodiscard]] auto get_pixeles() const -> std::vector<pixel> { return vector_pixeles; }
    [[nodiscard]] auto get_red(size_t index) const -> uint8_t { return vector_pixeles[index].red; }
    [[nodiscard]] auto get_green(size_t index) const -> uint8_t { return vector_pixeles[index].green; }
    [[nodiscard]] auto get_blue(size_t index) const -> uint8_t { return vector_pixeles[index].blue; }

    // Definimos los setters para los atributos privados
    void set_numero_magico(std::string const & numero_magico) { ImageAOS::numero_magico = numero_magico; }
    void set_ancho(int const & ancho) { ImageAOS::ancho = ancho; }
    void set_alto(int const & alto) { ImageAOS::alto = alto; }
    void set_max_intensidad(int const & max_intensidad) { ImageAOS::max_intensidad = max_intensidad; }
    void set_pixeles(std::vector<pixel> const & vector_pixel) { ImageAOS::vector_pixeles = vector_pixel; }
    void add_pixel(pixel const & pixel) { ImageAOS::vector_pixeles.push_back(pixel); }
};
#endif //COMP_ARCH_LAB1_IMAGEAOS_H
