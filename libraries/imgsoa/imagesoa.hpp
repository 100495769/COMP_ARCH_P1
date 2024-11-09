#ifndef COMP_ARCH_LAB1_IMAGESOA_H
#define COMP_ARCH_LAB1_IMAGESOA_H
#include <string>
#include <iostream>
#include <utility>
#include <vector>

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
  void guardar_imagen(std::string & path_imagen);  // ponemos el cost para que no se modifique nigun miembro de la clase
  auto verificar_datos() -> bool;
};


#endif //COMP_ARCH_LAB1_IMAGESOA_H
