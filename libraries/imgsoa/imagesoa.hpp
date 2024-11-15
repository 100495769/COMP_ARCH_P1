#ifndef COMP_ARCH_LAB1_IMAGESOA_H
#define COMP_ARCH_LAB1_IMAGESOA_H
#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <cstdint>
#include <unordered_map>

struct nuevo_hash {
    size_t operator()(const std::vector<std::uint8_t> & color) const {
      // Combinación de los hashes de los tres valores
      // Una posible implementación (ajusta según tus necesidades):
      return (color[0] << 16) | (color[1] << 8) | color[2];
    }
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
  std::vector<uint8_t> red, green, blue;  // Estructura de datos en formato SOA con los pixeles de la imagen

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
  struct SurroundingColoursSOA {
      int red_low_left, green_low_left, blue_low_left;
      int red_low_right, green_low_right, blue_low_right;
      int red_high_left, green_high_left, blue_high_left;
      int red_high_right, green_high_right, blue_high_right;
  };

  struct CoordenadasSOA {
      size_t x_low;
      size_t y_low;
      size_t x_high;
      size_t y_high;
  };
  void resize(int nuevo_ancho, int nuevo_alto);
  void cutfreq(int n);
  void pixel_assessment(CoordenadasSOA& coordenadas, SurroundingColoursSOA& surrounding_colours);
  void copy_contents(std::vector<int> &nuevo_red, std::vector<int> &nuevo_green, std::vector<int> &nuevo_blue);
  std::array<float, 3> interpolation(SurroundingColoursSOA& surrounding_colours, float x_original, float y_original);

  auto compress() -> std::tuple<size_t, std::vector<std::string>>;
  /*auto tablaIndices(size_t num, std::unordered_map<std::string, std::string> coloresUnicos)
      -> std::unordered_map<std::string, std::string>;*/
  void guardar_compress(std::string const & nombre_fichero, const std::tuple<size_t, std::vector<std::string>>& elem) const ;

  // definimos getters para los atributos privados
  [[nodiscard]] auto get_numero_magico() const -> std::string { return numero_magico; }
  [[nodiscard]] auto get_ancho() const -> int { return ancho; }
  [[nodiscard]] auto get_alto() const -> int { return alto; }
  [[nodiscard]] auto get_max_intensidad() const -> int { return max_intensidad; }
  [[nodiscard]] auto get_red() const -> std::vector<uint8_t> { return red; }
  [[nodiscard]] auto get_green() const -> std::vector<uint8_t> { return green; }
  [[nodiscard]] auto get_blue() const -> std::vector<uint8_t> { return blue; }

  // Definimos los setters para los atributos privados
  void set_numero_magico(std::string const & numero_magico) { ImageSOA::numero_magico = numero_magico; }
  void set_ancho(int const & ancho) { ImageSOA::ancho = ancho; }
  void set_alto(int const & alto) { ImageSOA::alto = alto; }
  void set_max_intensidad(int const & max_intensidad) { ImageSOA::max_intensidad = max_intensidad; }
  void set_red(std::vector<uint8_t> const & red) { ImageSOA::red = red; }
  void set_green(std::vector<uint8_t> const & green) { ImageSOA::green = green; }
  void set_blue(std::vector<uint8_t> const & blue) { ImageSOA::blue = blue; }
};

#endif //COMP_ARCH_LAB1_IMAGESOA_H
