//
// Created by sergio on 7/10/24.
//
#include <cstdint>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <libraries/common/progargs.hpp>
#include <libraries/imgsoa/imagesoa.hpp>

TEST(ImageSOA_TEST, prueba) {
  EXPECT_EQ(2, 2) << "Pepito malo";
}



//TESTS PARA LA FUNCIÓN COMPRESS()----------------------------------------------------------|

//empezamos con la función tablaIndices()
TEST(ImageSOA_TEST, tablaIndices_menorRango) {
  ImageSOA mi_imagen;
  // numero de colores unicos a probar sonmenos de 255 un 1 B
  size_t coloresUnicosNum_prueba = 3;
  std::unordered_map<std::string, std::string> coloresUnicos_prueba = {{"163159148", "1"}, {"1008173", "2"}, {"2562560", "3"}};
  std::unordered_map<std::string, std::string> coloresUnicos_devueltos = tablaIndices(coloresUnicosNum_prueba, coloresUnicos_prueba);
  std::unordered_map<std::string, std::string> coloresUnicos_esperados = {{"163159148", "00000001"}, {"1008173", "00000010"}, {"2562560", "00000011"}};

  ASSERT_EQ(coloresUnicos_devueltos, coloresUnicos_esperados);
}


TEST(ImageSOA_TEST, tablaIndices_medioRango) {
  ImageSOA mi_imagen;
  // numero de colores unicos a probar son entre 255 y 65535 un 2 B (16b)
  size_t coloresUnicosNum_prueba2 = 300;
  std::unordered_map<std::string, std::string> coloresUnicos_prueba2 = {{"163159148", "4790"}, {"1008173", "2"}, {"2562560", "3"}};
  std::unordered_map<std::string, std::string> coloresUnicos_devueltos2 = tablaIndices(coloresUnicosNum_prueba2, coloresUnicos_prueba2);
  std::unordered_map<std::string, std::string> coloresUnicos_esperados2 = {{"163159148", "1011011000010010"}, {"1008173", "0000001000000000"}, {"2562560", "0000001100000000"}};
  //std::cout << "coloresUnicos_devueltos: " << coloresUnicos_devueltos2 << std::endl;
  //std::cout << "coloresUnicos_esperados: " << coloresUnicos_esperados2 << std::endl;
  ASSERT_EQ(coloresUnicos_devueltos2, coloresUnicos_esperados2);
}

TEST(ImageSOA_TEST, tablaIndices_mayorRango) {
  ImageSOA mi_imagen;
  // numero de colores unicos a probar son entre 65535 y 16777215 un 3 B (24b)
  size_t coloresUnicosNum_prueba3 = 70000;
  std::unordered_map<std::string, std::string> coloresUnicos_prueba3 = {{"163159148", "4790"}, {"1008173", "827321754"}, {"2562560", "3"}};
  std::unordered_map<std::string, std::string> coloresUnicos_devueltos3 = tablaIndices(coloresUnicosNum_prueba3, coloresUnicos_prueba3);
  std::unordered_map<std::string, std::string> coloresUnicos_esperados3 = {{"163159148", "10110110000100100000000000000000"}, {"1008173", "10011010111011010100111100110001"}, {"2562560", "00000011000000000000000000000000"}};
  ASSERT_EQ(coloresUnicos_devueltos3, coloresUnicos_esperados3);
}

TEST(ImageSOA_TEST, tablaIndices_error) {
  ImageSOA mi_imagen;
  // numero de colores unicos a probar es muy grande para ser representados en la imagen comprimida.
  size_t coloresUnicosNum_prueba4 = 1203824932334;
  std::unordered_map<std::string, std::string> coloresUnicos_prueba4 = {{"163159148", "4790"}, {"1008173", "827321754"}, {"2562560", "3"}};
  ASSERT_EXIT(tablaIndices(coloresUnicosNum_prueba4, coloresUnicos_prueba4);, ::testing::ExitedWithCode(1), ".*");

}


// Tests para la FUNCIÓN GUARDAR_COMPRESS()----------------------------------------------------------|

struct pixel {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};
TEST(ImageSOA_TEST, guardar_compress_non_valid) {
  std::string path_imagen = "/path/que/no_mola/nada/no_existe.cppm";

  ImageSOA imagen_prueba;
  std::tuple<size_t, std::vector<std::string>> elem = {23, {"00111111", "00000000", "11111100", "00000011"}};
  // guardamos en el path que no existe y comprobamos que salimos en salida de error jeje
  ASSERT_EXIT(imagen_prueba.guardar_compress(path_imagen, elem), ::testing::ExitedWithCode(1), ".*");
}

TEST(ImageSOA_TEST, guardar_compress_valid) {
  ImageSOA image_prueba;
  std::string path_imagen = "path.cppm";
  std::string magic_number = "C6";
  image_prueba.set_alto(1);
  image_prueba.set_ancho(3);
  image_prueba.set_max_intensidad(245);

  image_prueba.set_red(std::vector<uint8_t> {255, 255,40});
  image_prueba.set_green(std::vector<uint8_t> {32, 51, 192});
  image_prueba.set_blue(std::vector<uint8_t> {03, 192, 54});
  std::tuple<size_t, std::vector<std::string>> elem = {3, {"00000001", "00000010", "00000011"}};
  // guardamos en el path que no existe y comprobamos que salimos en salida de error jeje
  image_prueba.guardar_compress(path_imagen, elem);
  std::ifstream file(path_imagen);
  ASSERT_TRUE(file.good());
  file.close();
}


// Tests para la FUNCIÓN COMPRESS()----------------------------------------------------------|

TEST(ImageSOA_TEST, compress_funcionamiento_correcto_indices) {
  ImageSOA image_prueba;
  image_prueba.set_alto(1);
  image_prueba.set_ancho(3);
  image_prueba.set_max_intensidad(245);
  //auto pixeles = {{255,  32, 03}, { 40, 192, 54}};
  image_prueba.set_red(std::vector<uint8_t> {255, 255,40});
  image_prueba.set_green(std::vector<uint8_t> {32, 51, 192});
  image_prueba.set_blue(std::vector<uint8_t> {03, 192, 54});

  size_t expected_size_colors = 3;
  std::vector<std::string> pixelIndices = {"00000000", "00000001", "00000010"};
  ASSERT_EQ(image_prueba.compress(), std::make_tuple(expected_size_colors, pixelIndices));
}

TEST(ImageSOA_TEST, compress_calculo_tamaño_indice) {
    ImageSOA image_prueba;
    // Inicializar pixeles con colores específicos para pruebas
  image_prueba.set_red(std::vector<uint8_t> {255, 255,40});
  image_prueba.set_green(std::vector<uint8_t> {32, 51, 192});
  image_prueba.set_blue(std::vector<uint8_t> {03, 192, 54});
  // Ejecuta la función compress
    auto [numColoresUnicos, pixelIndices] = image_prueba.compress();

    // Comprobar el número de colores únicos
    EXPECT_EQ(numColoresUnicos, 3); // Solo 3 colores únicos

    // Verificar que se han asignado correctamente los índices binarios
    std::vector<std::string> expectedIndices = {"00000000", "00000001", "00000010"};
    EXPECT_EQ(pixelIndices, expectedIndices);
}
/*
TEST(ImageSOA_TEST, compress_calculo_correcto_muchos_pixeles) {
    ImageSOA image_prueba;

    for (int i = 0; i < 1000; ++i) {
      image_prueba.set_red(std::vector<uint8_t>((i +255) % 256 ));
      image_prueba.set_green(std::vector<uint8_t>((i +567) % 255));
      image_prueba.set_blue(std::vector<uint8_t> ((i +305) % 255));
    }

    auto [numColoresUnicos, pixelIndices] = image_prueba.compress();

    EXPECT_EQ(numColoresUnicos, 1000); // con los colores calculados esperamos que haya 1000 colores únicos
    EXPECT_EQ(pixelIndices.size(), 1000); //generamos 1000 pixeles
}*/

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
/*
TEST(ImageSOA_TEST, maxlevel_valido_pequeño_pequeño) {
  // pasamos de 255 a 100.
  ImageSOA pequena1 = crear_imagen_prueba();
  int intens= pequena1.get_max_intensidad();

  ASSERT_EQ(intens, 255); // la intensidad la coge bien

  // cogemos los vectores de los pixeles y duplicamos para luego ir cambiando el valor de uno y comparar con el otro al hacer maxlevel
  std::vector<uint8_t> red = pequena1.get_red();
  std::vector<uint8_t> green = pequena1.get_green();
  std::vector<uint8_t> blue = pequena1.get_blue();
  std::vector<uint8_t> expected_red = red;
  std::vector<uint8_t> expected_green = green;
  std::vector<uint8_t> expected_blue = blue;

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

*/
// Tests para la FUNCIÓN RESIZE()----------------------------------------------------------|

TEST(ImageSOA_TEST, interpolation_pixeles_valido) {
  // check if the interpolation algorithm works as expected
  // como salida estandar cada variable de la imagen creada.
  ImageSOA imagen_prueba = crear_imagen_prueba();

  // treating initial pixel vector wrt the coordinates the values are in it
  // check interpolation in between points (0,1) and (0,0)
  ImageSOA::CoordenadasSOA coordenadas;
  ImageSOA::SurroundingColoursSOA surrounding_colours;

  //coordenadas = {0, 0, 1, 0};
  coordenadas.x_low = 0;
  coordenadas.y_low = 0;
  coordenadas.x_high = 1;
  coordenadas.y_high = 0;
  imagen_prueba.pixel_assessment(coordenadas, surrounding_colours);
  auto rg_interpolation = imagen_prueba.interpolation(surrounding_colours, 0.5, 0);
  ASSERT_EQ(rg_interpolation[0], 127.5);
  ASSERT_EQ(rg_interpolation[1], 127.5);
  ASSERT_EQ(rg_interpolation[2], 0);

  // check interpolation in between points (0,0) and (1, 1)
  //coordenadas = {0, 0, 1, 1};
  coordenadas.x_low = 0;
  coordenadas.y_low = 0;
  coordenadas.x_high = 1;
  coordenadas.y_high = 1;
  imagen_prueba.pixel_assessment(coordenadas, surrounding_colours);
  auto rgb_interpolation = imagen_prueba.interpolation(surrounding_colours, 0.5, 0.5);
  ASSERT_EQ(rgb_interpolation[0], 127.5);
  ASSERT_EQ(rgb_interpolation[1], 127.5);
  ASSERT_EQ(rgb_interpolation[2], 127.5);

  // check interpolation in between points (0,0) and (0, 1)
  //coordenadas = {0, 0, 0, 1};
  coordenadas.x_low = 0;
  coordenadas.y_low = 0;
  coordenadas.x_high = 0;
  coordenadas.y_high = 1;
  imagen_prueba.pixel_assessment(coordenadas, surrounding_colours);
  auto gb_interpolation = imagen_prueba.interpolation(surrounding_colours, 0, 0.5);
  ASSERT_EQ(gb_interpolation[0], 127.5);
  ASSERT_EQ(gb_interpolation[1], 0);
  ASSERT_EQ(gb_interpolation[2], 127.5);
}

TEST(ImageSOA_TEST, resize_bigger_valido) {
  // check if the new size rescaled accordingly
  // como salida estandar cada variable de la imagen creada.
  ImageSOA imagen_prueba = crear_imagen_prueba();

  imagen_prueba.resize(4, 5);
  int anch = imagen_prueba.get_ancho();
  int alt = imagen_prueba.get_alto();

  ASSERT_EQ(anch, 4);
  ASSERT_EQ(alt, 5);
}

TEST(ImageSOA_TEST, resize_smaller_valido) {
  // check if the new size rescaled accordingly
  // como salida estandar cada variable de la imagen creada.
  ImageSOA imagen_prueba = crear_imagen_prueba();

  imagen_prueba.resize(1, 1);
  int anch = imagen_prueba.get_ancho();
  int alt = imagen_prueba.get_alto();

  ASSERT_EQ(anch, 1);
  ASSERT_EQ(alt, 1);
}

TEST(ImageSOA_TEST, resize_bigger_invalido_1) {
  // check if the new size rescaled accordingly
  // como salida estandar cada variable de la imagen creada.
  ImageSOA imagen_prueba = crear_imagen_prueba();

  imagen_prueba.resize(4, 5);
  int anch = imagen_prueba.get_ancho();
  int alt = imagen_prueba.get_alto();

  ASSERT_NE(anch, 5);
  ASSERT_NE(alt, 6);
}

TEST(ImageSOA_TEST, resize_bigger_invalido_2) {
  // check if the new size rescaled accordingly
  // como salida estandar cada variable de la imagen creada.
  ImageSOA imagen_prueba = crear_imagen_prueba();

  imagen_prueba.resize(4, 5);
  int anch = imagen_prueba.get_ancho();
  int alt = imagen_prueba.get_alto();

  ASSERT_NE(anch, 10);
  ASSERT_NE(alt, 11);
}

TEST(ImageSOA_TEST, resize_bigger_invalido_3) {
  // check if the new size rescaled accordingly
  // como salida estandar cada variable de la imagen creada.
  ImageSOA imagen_prueba = crear_imagen_prueba();

  imagen_prueba.resize(4, 5);
  int anch = imagen_prueba.get_ancho();
  int alt = imagen_prueba.get_alto();

  ASSERT_NE(anch, 3);
  ASSERT_NE(alt, 4);
}

TEST(ImageSOA_TEST, resize_bigger_invalido_4) {
  // check if the new size rescaled accordingly
  // como salida estandar cada variable de la imagen creada.
  ImageSOA imagen_prueba = crear_imagen_prueba();

  imagen_prueba.resize(4, 5);
  int anch = imagen_prueba.get_ancho();
  int alt = imagen_prueba.get_alto();

  ASSERT_NE(anch, 1);
  ASSERT_NE(alt, 2);
}

TEST(ImageSOA_TEST, resize_bigger_invalido_5) {
  // check if the new size rescaled accordingly
  // como salida estandar cada variable de la imagen creada.
  ImageSOA imagen_prueba = crear_imagen_prueba();

  imagen_prueba.resize(4, 5);
  int anch = imagen_prueba.get_ancho();
  int alt = imagen_prueba.get_alto();

  ASSERT_NE(anch, 2);
  ASSERT_NE(alt, 2);
}

TEST(ImageSOA_TEST, resize_smaller_invalido_1) {
  // check if the new size rescaled accordingly
  // como salida estandar cada variable de la imagen creada.
  ImageSOA imagen_prueba = crear_imagen_prueba();

  imagen_prueba.resize(1, 1);
  int anch = imagen_prueba.get_ancho();
  int alt = imagen_prueba.get_alto();

  ASSERT_NE(anch, 2);
  ASSERT_NE(alt, 2);
}

TEST(ImageSOA_TEST, resize_smaller_invalido_2) {
  // check if the new size rescaled accordingly
  // como salida estandar cada variable de la imagen creada.
  ImageSOA imagen_prueba = crear_imagen_prueba();

  imagen_prueba.resize(1, 1);
  int anch = imagen_prueba.get_ancho();
  int alt = imagen_prueba.get_alto();

  ASSERT_NE(anch, 5);
  ASSERT_NE(alt, 5);
}

TEST(ImageSOA_TEST, resize_smaller_invalido_3) {
  // check if the new size rescaled accordingly
  // como salida estandar cada variable de la imagen creada.
  ImageSOA imagen_prueba = crear_imagen_prueba();

  imagen_prueba.resize(1, 1);
  int anch = imagen_prueba.get_ancho();
  int alt = imagen_prueba.get_alto();

  ASSERT_NE(anch, 0);
  ASSERT_NE(alt, 0);
}