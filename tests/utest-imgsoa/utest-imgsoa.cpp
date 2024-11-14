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

TEST(ImageSOA_TEST, cargar_imagen_no_valido_noexiste) {
  // Este test comprueba que sale un error cuando no se consigue cargar una imagen porque no existe
  ImageSOA imagen_prueba;
  std::string path_imagen_invalido = "/path/que/no_existe.ppm";
  // cogemos la salida de error que debe salir de la funcion cargar_imagen con ese path
  ::testing::internal::CaptureStderr();
  imagen_prueba.cargar_imagen(path_imagen_invalido);
  std::string salida_error = ::testing::internal::GetCapturedStderr();
  ASSERT_EQ(salida_error, "Error al abrir la imagen: /path/que/no_existe.ppm\n");
}


TEST(ImageSOA_TEST, cargar_imagen_no_valido_formato_incorrecto) {
  // Este test comprueba que sale un error cuando la imagen que se abre no tiene el formato que deberia"
  // elegiremos que para que interprete como formato invalido tenga una foto con
  // un numero magico p7 y un maximo de intensidad fuera de rango
  // vamos a comprobar que hemos salido con codigo de error exit(1)

  // vamos a crear una imagen de 2x2 con un formato no valido para este editor
  std::string path_imagen = "/tmp/imagen_prueba.ppm";
  std::vector<uint8_t> matriz_pixeles = {
    'P', '7', '\n', '2', ' ', '2', '\n', '7', '0', '0', '0','0', '\n',
    255, 0, 0,    0, 255, 0,    0, 0, 255,   255, 255, 255};
  std::ofstream archivo(path_imagen, std::ios::binary);
  archivo.write(reinterpret_cast<char*>(matriz_pixeles.data()), sizeof(matriz_pixeles));
  archivo.close();
  ImageSOA imagen_prueba;

  // capturamos el error que va a salir al llamar a cargar_imagen
  ASSERT_EXIT(imagen_prueba.cargar_imagen(path_imagen), ::testing::ExitedWithCode(1), ".*");
}


TEST(ImageSOA_TEST, guardar_imagen_valido) {
  std::string path_imagen = "/tmp/imagen_prueba_guardada.ppm";
  std::string magic_number = "P6";
  int width = 2;
  int height = 2;
  int max_intensity = 255;
  std::vector<uint8_t> rojo = {255, 0,0,255};
  std::vector<uint8_t> verde = {0, 255, 0, 255};
  std::vector<uint8_t> azul = {0, 0, 255, 255};
  ImageSOA imagen_prueba;
  imagen_prueba.set_numero_magico(magic_number);
  imagen_prueba.set_ancho(width);
  imagen_prueba.set_alto(height);
  imagen_prueba.set_max_intensidad(max_intensity);
  imagen_prueba.set_red(rojo);
  imagen_prueba.set_green(verde);
  imagen_prueba.set_blue(azul);
  imagen_prueba.guardar_imagen(path_imagen);
  // leemos esa supuesta nueva imagen y comprobamos que el fichero esta chachi piruli
  std::ifstream file(path_imagen, std::ios::binary);
  ASSERT_TRUE(file.good());
}


TEST(ImageSOA_TEST, guardar_imagen_no_valido_noexiste) {
  std::string path_imagen = "/path/que/no_mola/nada/no_existe.ppm";
  std::string magic_number = "P6";
  int width = 2;
  int height = 2;
  int max_intensity = 255;
  std::vector<uint8_t> rojo = {255, 0,0,255};
  std::vector<uint8_t> verde = {0, 255, 0, 255};
  std::vector<uint8_t> azul = {0, 0, 255, 255};
  ImageSOA imagen_prueba;
  imagen_prueba.set_numero_magico(magic_number);
  imagen_prueba.set_ancho(width);
  imagen_prueba.set_alto(height);
  imagen_prueba.set_max_intensidad(max_intensity);
  imagen_prueba.set_red(rojo);
  imagen_prueba.set_green(verde);
  imagen_prueba.set_blue(azul);
  // guardamos en el path que no existe y comprobamos que salimos en salida de error jeje
  ASSERT_EXIT(imagen_prueba.guardar_imagen(path_imagen), ::testing::ExitedWithCode(1), ".*");
}

// En el caso de maxlevel solo vamos a gestionar los casos validos, porque la unica forma de que
// sea invalido es que haya problemas en el paso de argumentos al ejecutable o a la hora de leer/guardar
// datos a los archivos: Esta gesionado en otras funciones

// creamos dos fotos prueba: Una con valor de intensidad 255, y otra con 1000, por ejemplo.
auto imagen_pequena_maxlevel() -> ImageSOA {
  ImageSOA imagen_prueba;
  std::string magic_number = "P6";
  int width = 2;
  int height = 2;
  int max_intensity = 255;
  std::vector<uint8_t> rojo = {255, 0,0,255};
  std::vector<uint8_t> verde = {0, 255, 0, 255};
  std::vector<uint8_t> azul = {0, 0, 255, 255};
  imagen_prueba.set_numero_magico(magic_number);
  imagen_prueba.set_ancho(width);
  imagen_prueba.set_alto(height);
  imagen_prueba.set_max_intensidad(max_intensity);
  imagen_prueba.set_red(rojo);
  imagen_prueba.set_green(verde);
  imagen_prueba.set_blue(azul);
  return imagen_prueba;
}

auto imagen_grande_maxlevel() -> ImageSOA {
  ImageSOA imagen_prueba;
  std::string magic_number = "P6";
  int width = 2;
  int height = 2;
  int max_intensity = 10000;
  std::vector<uint8_t> rojo = {255, 0,0,255};
  std::vector<uint8_t> verde = {0, 255, 0, 255};
  std::vector<uint8_t> azul = {0, 0, 255, 255};
  imagen_prueba.set_numero_magico(magic_number);
  imagen_prueba.set_ancho(width);
  imagen_prueba.set_alto(height);
  imagen_prueba.set_max_intensidad(max_intensity);
  imagen_prueba.set_red(rojo);
  imagen_prueba.set_green(verde);
  imagen_prueba.set_blue(azul);
  return imagen_prueba;
}

TEST(ImageSOA_TEST, maxlevel_valido_pequeño_pequeño) {
  // pasamos de 255 a 100.
  ImageSOA pequena1 = imagen_pequena_maxlevel();
  int intens= pequena1.get_max_intensidad();
  std::vector<uint8_t> expected_red;
  std::vector<uint8_t> expected_green;
  std::vector<uint8_t> expected_blue;
  std::vector<uint8_t> red = pequena1.get_red();
  std::vector<uint8_t> green = pequena1.get_green();
  std::vector<uint8_t> blue = pequena1.get_blue();

  for (size_t i=0; i < red.size(); i++) {
    int nuevo_valor_red = (red[i] * 100) / intens;
    int nuevo_valor_green = (green[i] * 100) / intens;
    int nuevo_valor_blue = (blue[i] * 100) / intens;
    expected_red[i] = static_cast<uint8_t>(nuevo_valor_red);
    expected_green[i] = static_cast<uint8_t>(nuevo_valor_green);
    expected_blue[i] = static_cast<uint8_t>(nuevo_valor_blue);
  }
  pequena1.maxlevel(100);
  // ahora hacemos lo mismo de vuelta y vamos comparando.
  int salida_intens= pequena1.get_max_intensidad();
  std::vector<uint8_t> rednew = pequena1.get_red();
  std::vector<uint8_t> greennew = pequena1.get_green();
  std::vector<uint8_t> bluenew = pequena1.get_blue();
  ASSERT_EQ(salida_intens, 100);
  for (size_t i=0; i< rednew.size(); i++) {
    // comparamos
    ASSERT_EQ(expected_red[i], rednew[i]);
    ASSERT_EQ(expected_green[i], greennew[i]);
    ASSERT_EQ(expected_blue[i], bluenew[i]);
  }
}

TEST(ImageSOA_TEST, maxlevel_valido_pequeño_grande) {
  // pasamos de 255 a 10000.
  ImageSOA pequena1 = imagen_pequena_maxlevel();
  int intens= pequena1.get_max_intensidad();
  std::vector<uint8_t> expected_red;
  std::vector<uint8_t> expected_green;
  std::vector<uint8_t> expected_blue;
  std::vector<uint8_t> red = pequena1.get_red();
  std::vector<uint8_t> green = pequena1.get_green();
  std::vector<uint8_t> blue = pequena1.get_blue();

  for (size_t i=0; i < red.size(); i++) {
    int nuevo_valor_red = (red[i] * 10000) / intens;
    int nuevo_valor_green = (green[i] * 10000) / intens;
    int nuevo_valor_blue = (blue[i] * 10000) / intens;
    expected_red[i] = static_cast<uint8_t>(nuevo_valor_red);
    expected_green[i] = static_cast<uint8_t>(nuevo_valor_green);
    expected_blue[i] = static_cast<uint8_t>(nuevo_valor_blue);
  }
  pequena1.maxlevel(10000);
  // ahora hacemos lo mismo de vuelta y vamos comparando.
  int salida_intens= pequena1.get_max_intensidad();
  std::vector<uint8_t> rednew = pequena1.get_red();
  std::vector<uint8_t> greennew = pequena1.get_green();
  std::vector<uint8_t> bluenew = pequena1.get_blue();
  ASSERT_EQ(salida_intens, 10000);
  for (size_t i=0; i< rednew.size(); i++) {
    // comparamos
    ASSERT_EQ(expected_red[i], rednew[i]);
    ASSERT_EQ(expected_green[i], greennew[i]);
    ASSERT_EQ(expected_blue[i], bluenew[i]);
  }
}

TEST(ImageSOA_TEST, maxlevel_valido_grande_pequeño) {
  // pasamos de 10000 a 255.
  ImageSOA pequena1 = imagen_grande_maxlevel();
  int intens= pequena1.get_max_intensidad();
  std::vector<uint8_t> expected_red;
  std::vector<uint8_t> expected_green;
  std::vector<uint8_t> expected_blue;
  std::vector<uint8_t> red = pequena1.get_red();
  std::vector<uint8_t> green = pequena1.get_green();
  std::vector<uint8_t> blue = pequena1.get_blue();

  for (size_t i=0; i < red.size(); i++) {
    int nuevo_valor_red = (red[i] * 255) / intens;
    int nuevo_valor_green = (green[i] * 255) / intens;
    int nuevo_valor_blue = (blue[i] * 255) / intens;
    expected_red[i] = static_cast<uint8_t>(nuevo_valor_red);
    expected_green[i] = static_cast<uint8_t>(nuevo_valor_green);
    expected_blue[i] = static_cast<uint8_t>(nuevo_valor_blue);
  }
  pequena1.maxlevel(255);
  // ahora hacemos lo mismo de vuelta y vamos comparando.
  int salida_intens= pequena1.get_max_intensidad();
  std::vector<uint8_t> rednew = pequena1.get_red();
  std::vector<uint8_t> greennew = pequena1.get_green();
  std::vector<uint8_t> bluenew = pequena1.get_blue();
  ASSERT_EQ(salida_intens, 255);
  for (size_t i=0; i< rednew.size(); i++) {
    // comparamos
    ASSERT_EQ(expected_red[i], rednew[i]);
    ASSERT_EQ(expected_green[i], greennew[i]);
    ASSERT_EQ(expected_blue[i], bluenew[i]);
  }
}

TEST(ImageSOA_TEST, maxlevel_valido_grande_grande) {
  // pasamos de 10000 a 60000.
  ImageSOA pequena1 = imagen_pequena_maxlevel();
  int intens= pequena1.get_max_intensidad();
  std::vector<uint8_t> expected_red;
  std::vector<uint8_t> expected_green;
  std::vector<uint8_t> expected_blue;
  std::vector<uint8_t> red = pequena1.get_red();
  std::vector<uint8_t> green = pequena1.get_green();
  std::vector<uint8_t> blue = pequena1.get_blue();

  for (size_t i=0; i < red.size(); i++) {
    int nuevo_valor_red = (red[i] * 60000) / intens;
    int nuevo_valor_green = (green[i] * 60000) / intens;
    int nuevo_valor_blue = (blue[i] * 60000) / intens;
    expected_red[i] = static_cast<uint8_t>(nuevo_valor_red);
    expected_green[i] = static_cast<uint8_t>(nuevo_valor_green);
    expected_blue[i] = static_cast<uint8_t>(nuevo_valor_blue);
  }
  pequena1.maxlevel(60000);
  // ahora hacemos lo mismo de vuelta y vamos comparando.
  int salida_intens= pequena1.get_max_intensidad();
  std::vector<uint8_t> rednew = pequena1.get_red();
  std::vector<uint8_t> greennew = pequena1.get_green();
  std::vector<uint8_t> bluenew = pequena1.get_blue();
  ASSERT_EQ(salida_intens, 60000);
  for (size_t i=0; i< rednew.size(); i++) {
    // comparamos
    ASSERT_EQ(expected_red[i], rednew[i]);
    ASSERT_EQ(expected_green[i], greennew[i]);
    ASSERT_EQ(expected_blue[i], bluenew[i]);
  }
}