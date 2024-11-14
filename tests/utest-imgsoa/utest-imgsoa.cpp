//
// Created by sergio on 7/10/24.
//
#include <libraries/imgsoa/imagesoa.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <cstdint>

TEST(ImageSOA_TEST, prueba){
  EXPECT_EQ(2, 2) << "Pepito malo";
}

// esta funcion creara una imagen de prueba en el archivo indicado
auto crear_imagen_prueba() -> ImageSOA {
  // vamos a crear una imagen de 2x2 de prueba para generar los tests
  std::string path_imagen = "/tmp/imagen_prueba.ppm";

  // metemos la los metadatos y la matriz de pixeles
  std::vector<uint8_t> matriz_pixeles = {
    'P', '6', '\n', '2', ' ', '2', '\n', '2', '5', '5', '\n',
    255, 0, 0,    0, 255, 0,    0, 0, 255,   255, 255, 255};

  std::ofstream archivo(path_imagen, std::ios::binary);
  archivo.write(reinterpret_cast<char*>(matriz_pixeles.data()), sizeof(matriz_pixeles));
  archivo.close();

  ImageSOA imagen_prueba;
  imagen_prueba.cargar_imagen(path_imagen);

  return imagen_prueba;
}


TEST(ImageSOA_TEST, cargar_imagen_valido_255) {
  // si guardar_imagen funciona correctamente, guarda correctamente los metadatos.
  // Eso significa que la funcion info tambien funciona correctamente porque esta solo saca
  // como salida estandar cada variable de la imagen creada.
  ImageSOA imagen_prueba = crear_imagen_prueba();
  // obtenemos los parametros de la imagen
  std::string num_mag = imagen_prueba.get_numero_magico();
  int anch = imagen_prueba.get_ancho();
  int alt = imagen_prueba.get_alto();
  int max_int = imagen_prueba.get_max_intensidad();
  std::vector<uint8_t> rojo = imagen_prueba.get_red();
  std::vector<uint8_t> verde = imagen_prueba.get_green();
  std::vector<uint8_t> azul = imagen_prueba.get_blue();
  std::vector<uint8_t> red_comprobar = {255, 0,0,255};
  std::vector<uint8_t> verde_comprobar = {0, 255, 0, 255};
  std::vector<uint8_t> azul_comprobar = {0, 0, 255, 255};
  // comprobamos que los parametros sean los que queremos
  ASSERT_EQ("P6", num_mag);
  ASSERT_EQ(2, anch);
  ASSERT_EQ(2, alt);
  ASSERT_EQ(255, max_int);
  ASSERT_EQ(rojo, red_comprobar);
  ASSERT_EQ(verde, verde_comprobar);
  ASSERT_EQ(azul, azul_comprobar);
}