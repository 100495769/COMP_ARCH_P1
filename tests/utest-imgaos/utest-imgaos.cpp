//
// Created by sergio on 7/10/24.
//
//#include <cmake-build-debug/_deps/googletest-src/googlemock/include/gmock/gmock-actions.h>
//#include <cmake-build-debug/_deps/googletest-src/googlemock/include/gmock/gmock-function-mocker.h>
//#include <cmake-build-debug/_deps/googletest-src/googlemock/include/gmock/gmock-spec-builders.h>
#include <cstdint>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <libraries/common/progargs.hpp>
#include <libraries/imgaos/imageaos.hpp>
#include <string>
#include <unordered_map>
#include <vector>



//TESTS PARA LA FUNCIÓN COMPRESS()----------------------------------------------------------|

//empezamos con la función tablaIndices()
TEST(ImageAOS_TEST, tablaIndices_menorRango) {
  ImageAOS mi_imagen;
  // numero de colores unicos a probar sonmenos de 255 un 1 B
  size_t coloresUnicosNum_prueba = 3;
  std::unordered_map<std::string, std::string> coloresUnicos_prueba = {{"163159148", "1"}, {"1008173", "2"}, {"2562560", "3"}};
  std::unordered_map<std::string, std::string> coloresUnicos_devueltos = tablaIndices(coloresUnicosNum_prueba, coloresUnicos_prueba);
  std::unordered_map<std::string, std::string> coloresUnicos_esperados = {{"163159148", "00000001"}, {"1008173", "00000010"}, {"2562560", "00000011"}};
  ASSERT_EQ(coloresUnicos_devueltos, coloresUnicos_esperados);
}


TEST(ImageAOS_TEST, tablaIndices_medioRango) {
  ImageAOS mi_imagen;
  // numero de colores unicos a probar son entre 255 y 65535 un 2 B (16b)
  size_t coloresUnicosNum_prueba2 = 300;
  std::unordered_map<std::string, std::string> coloresUnicos_prueba2 = {{"163159148", "4790"}, {"1008173", "2"}, {"2562560", "3"}};
  std::unordered_map<std::string, std::string> coloresUnicos_devueltos2 = tablaIndices(coloresUnicosNum_prueba2, coloresUnicos_prueba2);
  std::unordered_map<std::string, std::string> coloresUnicos_esperados2 = {{"163159148", "1011011000010010"}, {"1008173", "0000001000000000"}, {"2562560", "0000001100000000"}};
  ASSERT_EQ(coloresUnicos_devueltos2, coloresUnicos_esperados2);
}

TEST(ImageAOS_TEST, tablaIndices_mayorRango) {
  ImageAOS mi_imagen;
  // numero de colores unicos a probar son entre 65535 y 16777215 un 3 B (24b)
  size_t coloresUnicosNum_prueba3 = 70000;
  std::unordered_map<std::string, std::string> coloresUnicos_prueba3 = {{"163159148", "4790"}, {"1008173", "827321754"}, {"2562560", "3"}};
  std::unordered_map<std::string, std::string> coloresUnicos_devueltos3 = tablaIndices(coloresUnicosNum_prueba3, coloresUnicos_prueba3);
  std::unordered_map<std::string, std::string> coloresUnicos_esperados3 = {{"163159148", "10110110000100100000000000000000"}, {"1008173", "10011010111011010100111100110001"}, {"2562560", "00000011000000000000000000000000"}};
  ASSERT_EQ(coloresUnicos_devueltos3, coloresUnicos_esperados3);
}

TEST(ImageAOS_TEST, tablaIndices_error) {
  ImageAOS mi_imagen;
  // numero de colores unicos a probar es muy grande para ser representados en la imagen comprimida.
  size_t coloresUnicosNum_prueba4 = 1203824932334;
  std::unordered_map<std::string, std::string> coloresUnicos_prueba4 = {{"163159148", "4790"}, {"1008173", "827321754"}, {"2562560", "3"}};
  ASSERT_EXIT(tablaIndices(coloresUnicosNum_prueba4, coloresUnicos_prueba4);, ::testing::ExitedWithCode(1), ".*");

}


// Tests para la FUNCIÓN GUARDAR_COMPRESS()----------------------------------------------------------|
/*
struct pixel {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};*/

TEST(ImageAOS_TEST, guardar_compress_non_valid) {
  std::string path_imagen = "/path/que/no_mola/nada/no_existe.cppm";

  ImageAOS imagen_prueba;
  std::tuple<size_t, std::vector<std::string>> elem = {23, {"00111111", "00000000", "11111100", "00000011"}};
  // guardamos en el path que no existe y comprobamos que salimos en salida de error jeje
  ASSERT_EXIT(imagen_prueba.guardar_compress(path_imagen, elem), ::testing::ExitedWithCode(1), ".*");
}

TEST(ImageAOS_TEST, guardar_compress_valid) {
  ImageAOS image_prueba;
  std::string path_imagen = "path.cppm";
  std::string magic_number = "C6";
  image_prueba.set_alto(1);
  image_prueba.set_ancho(3);
  image_prueba.set_max_intensidad(245);
  //auto pixeles = {{255,  32, 03}, { 40, 192, 54}};
  image_prueba.add_pixel({.red =255, .green = 32,.blue = 03});
  image_prueba.add_pixel({.red =255, .green = 51,.blue = 98});
  image_prueba.add_pixel({.red = 40, .green = 192,.blue = 54});

  std::tuple<size_t, std::vector<std::string>> elem = {3, {"00000001", "00000010", "00000011"}};
  // guardamos en el path que no existe y comprobamos que salimos en salida de error jeje
  image_prueba.guardar_compress(path_imagen, elem);
  std::ifstream file(path_imagen);
  ASSERT_TRUE(file.good());
  file.close();
}


// Tests para la FUNCIÓN COMPRESS()----------------------------------------------------------|

TEST(ImageAOS_TEST, compress_funcionamiento_correcto_indices) {
  ImageAOS image_prueba;
  image_prueba.set_alto(1);
  image_prueba.set_ancho(3);
  image_prueba.set_max_intensidad(245);
  //auto pixeles = {{255,  32, 03}, { 40, 192, 54}};
  image_prueba.add_pixel({.red =255, .green = 32,.blue = 03});
  image_prueba.add_pixel({.red =255, .green = 51,.blue = 98});
  image_prueba.add_pixel({.red = 40, .green = 192,.blue = 54});

  size_t expected_size_colors = 3;
  std::vector<std::string> pixelIndices = {"00000000", "00000001", "00000010"};
  ASSERT_EQ(image_prueba.compress(), std::make_tuple(expected_size_colors, pixelIndices));
}

TEST(ImageAOS_TEST, compress_calculo_tamaño_indice) {
    ImageAOS image_prueba;
    // Inicializar pixeles con colores específicos para pruebas
    image_prueba.add_pixel({.red =255, .green = 32,.blue = 03});
    image_prueba.add_pixel({.red =255, .green = 51,.blue = 98});
    image_prueba.add_pixel({.red = 40, .green = 192,.blue = 54});
  // Ejecuta la función compress
    auto [numColoresUnicos, pixelIndices] = image_prueba.compress();

    // Comprobar el número de colores únicos
    EXPECT_EQ(numColoresUnicos, 3); // Solo 3 colores únicos

    // Verificar que se han asignado correctamente los índices binarios
    std::vector<std::string> expectedIndices = {"00000000", "00000001", "00000010"};
    EXPECT_EQ(pixelIndices, expectedIndices);
}

TEST(ImageAOS_TEST, compress_calculo_correcto_muchos_pixeles) {
    ImageAOS image_prueba;

    for (int i = 0; i < 1000; ++i) {
        image_prueba.add_pixel({.red = static_cast<uint8_t>(i +255 % 256 ), .green = static_cast<uint8_t>((i +255) % 255),.blue = static_cast<uint8_t>((i +255) % 255)});
    }

    auto [numColoresUnicos, pixelIndices] = image_prueba.compress();

    EXPECT_EQ(numColoresUnicos, 1000); // con los colores calculados esperamos que haya 1000 colores únicos
    EXPECT_EQ(pixelIndices.size(), 1000); //generamos 1000 pixeles
}

// CREAR IMAGEN AOS -------------------------------------------------------------------------------
// esta funcion creara una imagen de prueba en el archivo indicado
auto crear_imagen_prueba() -> ImageAOS {
  // vamos a crear una imagen de 2x2 de prueba para generar los tests
  std::string path_imagen = "/tmp/imagen_prueba_aos.ppm";

  // metemos la los metadatos y la matriz de pixeles
  std::vector<uint8_t> matriz_pixeles = {
    'P', '6', '\n', '2', ' ', '2', '\n', '2', '5', '5', '\n',
    255, 0, 0,    0, 255, 0,    0, 0, 255,   255, 255, 255};

  std::ofstream archivo(path_imagen, std::ios::binary);
  archivo.write(reinterpret_cast<char*>(matriz_pixeles.data()), sizeof(matriz_pixeles));
  archivo.close();

  ImageAOS imagen_prueba;
  imagen_prueba.cargar_imagen_aos(path_imagen);

  return imagen_prueba;
}

auto crear_imagen_prueba_grande() -> ImageAOS {
  // vamos a crear una imagen de 2x2 de prueba con una intensidad de 65535
  std::string path_imagen = "/tmp/imagen_prueba_grande.ppm";
  // metemos la los metadatos y la matriz de pixeles
  std::vector<uint16_t> matriz_pixeles = {
    'P', '6', '\n', '2', ' ', '2', '\n', '6','5','5','3','5', '\n',
    65535, 0, 0,    0, 65535, 0,    0, 0, 65535,   65535, 65535, 65535};
  std::ofstream archivo(path_imagen, std::ios::binary);
  archivo.write(reinterpret_cast<char*>(matriz_pixeles.data()), sizeof(matriz_pixeles));
  archivo.close();
  ImageAOS imagen_prueba;
  imagen_prueba.cargar_imagen_aos(path_imagen);
  return imagen_prueba;
}

TEST(ImageAOS_TEST, cargar_imagen_valido_255) {
  // si cargar_imagen funciona correctamente, guarda correctamente los metadatos en variables.
  // Eso significa que la funcion info tambien funciona correctamente porque esta solo saca
  // como salida estandar cada variable de la imagen creada.
  ImageAOS imagen_prueba = crear_imagen_prueba();
  // obtenemos los parametros de la imagen
  std::string num_mag = imagen_prueba.get_numero_magico();
  int anch = imagen_prueba.get_ancho();
  int alt = imagen_prueba.get_alto();
  int max_int = imagen_prueba.get_max_intensidad();

  // definimos vectores que van a ser con los que comprobemos cada posicion del vector de structs
  std::vector<uint8_t> red_comprobar = {255, 0,0,255};
  std::vector<uint8_t> verde_comprobar = {0, 255, 0, 255};
  std::vector<uint8_t> azul_comprobar = {0, 0, 255, 255};

  // comprobamos que los parametros sean los que queremos
  ASSERT_EQ("P6", num_mag);
  ASSERT_EQ(2, anch);
  ASSERT_EQ(2, alt);
  ASSERT_EQ(255, max_int);

  // recorremos el vector
  int tamano_matriz = alt * anch;
  auto tamano_matriz_unsigned = static_cast<size_t>(tamano_matriz);
  for (size_t i = 0; i < tamano_matriz_unsigned; i++) {
    uint8_t rojo = imagen_prueba.get_red(i);
    uint8_t verde = imagen_prueba.get_green(i);
    uint8_t azul = imagen_prueba.get_blue(i);
    ASSERT_EQ(red_comprobar[i], rojo);
    ASSERT_EQ(verde_comprobar[i],verde);
    ASSERT_EQ(azul_comprobar[i], azul);
  }
}

TEST(ImageAOS_TEST, cargar_imagen_no_valido_noexiste) {
  // Este test comprueba que sale un error cuando no se consigue cargar una imagen porque no existe
  ImageAOS imagen_prueba;
  std::string path_imagen_invalido = "/path/que/no_existe.ppm";
  // cogemos la salida de error que debe salir de la funcion cargar_imagen con ese path
  ::testing::internal::CaptureStderr();
  imagen_prueba.cargar_imagen_aos(path_imagen_invalido);
  std::string salida_error = ::testing::internal::GetCapturedStderr();
  ASSERT_EQ(salida_error, "Error al abrir la imagen: /path/que/no_existe.ppm\n");
}


TEST(ImageAOS_TEST, cargar_imagen_no_valido_formato_incorrecto) {
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
  ImageAOS imagen_prueba;

  // capturamos el error que va a salir al llamar a cargar_imagen
  ASSERT_EXIT(imagen_prueba.cargar_imagen_aos(path_imagen), ::testing::ExitedWithCode(1), ".*");
}


TEST(ImageAOS_TEST, guardar_imagen_valido) {
  std::string path_imagen = "/tmp/imagen_prueba_guardada.ppm";
  std::string magic_number = "P6";
  int width = 2;
  int height = 2;
  int max_intensity = 255;
  ImageAOS imagen_prueba;
  imagen_prueba.set_numero_magico(magic_number);
  imagen_prueba.set_ancho(width);
  imagen_prueba.set_alto(height);
  imagen_prueba.set_max_intensidad(max_intensity);

  imagen_prueba.add_pixel({.red =255, .green = 0,.blue = 0});
  imagen_prueba.add_pixel({.red =0, .green = 255,.blue = 0});
  imagen_prueba.add_pixel({.red =0, .green = 0,.blue = 255});
  imagen_prueba.add_pixel({.red =255, .green = 255,.blue = 255});

  imagen_prueba.guardar_imagen_aos(path_imagen);
  // leemos esa supuesta nueva imagen y comprobamos que el fichero esta chachi piruli
  std::ifstream file(path_imagen, std::ios::binary);
  ASSERT_TRUE(file.good());
}


TEST(ImageAOS_TEST, guardar_imagen_no_valido_noexiste) {
  std::string path_imagen = "/path/que/no_mola/nada/no_existe.ppm";
  std::string magic_number = "P6";
  int width = 2;
  int height = 2;
  int max_intensity = 255;
  ImageAOS imagen_prueba;
  imagen_prueba.set_numero_magico(magic_number);
  imagen_prueba.set_ancho(width);
  imagen_prueba.set_alto(height);
  imagen_prueba.set_max_intensidad(max_intensity);
  imagen_prueba.add_pixel({.red =255, .green = 0,.blue = 0});
  imagen_prueba.add_pixel({.red =0, .green = 255,.blue = 0});
  imagen_prueba.add_pixel({.red =0, .green = 0,.blue = 255});
  imagen_prueba.add_pixel({.red =255, .green = 255,.blue = 255});

  // guardamos en el path que no existe y comprobamos que salimos en salida de error jeje
  ASSERT_EXIT(imagen_prueba.guardar_imagen_aos(path_imagen), ::testing::ExitedWithCode(1), ".*");
}

// En el caso de maxlevel solo vamos a gestionar los casos validos, porque la unica forma de que
// sea invalido es que haya problemas en el paso de argumentos al ejecutable o a la hora de leer/guardar
// datos a los archivos: Esta gesionado en otras funciones

TEST(ImageAOS_TEST, maxlevel_valido_pequeño_pequeño) {
  // pasamos de 255 a 100.
  ImageAOS pequena1 = crear_imagen_prueba();
  int intens= pequena1.get_max_intensidad();
  int alt = pequena1.get_alto();
  int anch = pequena1.get_ancho();

  // definimos vectores que van a ser con los que comprobemos cada posicion del vector de structs
  std::vector<uint8_t> red_comprobar = {255, 0,0,255};
  std::vector<uint8_t> verde_comprobar = {0, 255, 0, 255};
  std::vector<uint8_t> azul_comprobar = {0, 0, 255, 255};
  std::vector<uint8_t> expected_red = red_comprobar;
  std::vector<uint8_t> expected_green = verde_comprobar;
  std::vector<uint8_t> expected_blue = azul_comprobar;

  int tamano_matriz = alt * anch;
  auto tamano_matriz_unsigned = static_cast<size_t>(tamano_matriz);

  for (size_t i=0; i < tamano_matriz_unsigned; i++) {
    int nuevo_valor_red = (red_comprobar[i] * 100) / intens;
    int nuevo_valor_green = (verde_comprobar[i] * 100) / intens;
    int nuevo_valor_blue = (azul_comprobar[i] * 100) / intens;
    expected_red[i] = static_cast<uint8_t>(nuevo_valor_red);
    expected_green[i] = static_cast<uint8_t>(nuevo_valor_green);
    expected_blue[i] = static_cast<uint8_t>(nuevo_valor_blue);
  }
  // hacemos el maxlevel con 100 de intensidad
  pequena1.maxlevel(100);
  // recorremos el vector de nuevo y esta vez hacemos comprobaciones con los valores que deberian tener
  int salida_intens= pequena1.get_max_intensidad();
  ASSERT_EQ(salida_intens, 100);
  for (size_t i = 0; i < tamano_matriz_unsigned; i++) {
    uint8_t rojo = pequena1.get_red(i);
    uint8_t verde = pequena1.get_green(i);
    uint8_t azul = pequena1.get_blue(i);
    ASSERT_EQ(expected_red[i], rojo);
    ASSERT_EQ(expected_green[i],verde);
    ASSERT_EQ(expected_blue[i], azul);
  }
}

TEST(ImageAOS_TEST, maxlevel_valido_pequeño_grande) {
  // pasamos de 255 a 10000.
  ImageAOS pequena1 = crear_imagen_prueba();
  int intens= pequena1.get_max_intensidad();
  int alt = pequena1.get_alto();
  int anch = pequena1.get_ancho();

  // definimos vectores que van a ser con los que comprobemos cada posicion del vector de structs
  std::vector<uint8_t> red_comprobar = {255, 0,0,255};
  std::vector<uint8_t> verde_comprobar = {0, 255, 0, 255};
  std::vector<uint8_t> azul_comprobar = {0, 0, 255, 255};
  std::vector<uint8_t> expected_red = red_comprobar;
  std::vector<uint8_t> expected_green = verde_comprobar;
  std::vector<uint8_t> expected_blue = azul_comprobar;

  int tamano_matriz = alt * anch;
  auto tamano_matriz_unsigned = static_cast<size_t>(tamano_matriz);

  for (size_t i=0; i < tamano_matriz_unsigned; i++) {
    int nuevo_valor_red = (red_comprobar[i] * 10000) / intens;
    int nuevo_valor_green = (verde_comprobar[i] * 10000) / intens;
    int nuevo_valor_blue = (azul_comprobar[i] * 10000) / intens;
    expected_red[i] = static_cast<uint8_t>(nuevo_valor_red);
    expected_green[i] = static_cast<uint8_t>(nuevo_valor_green);
    expected_blue[i] = static_cast<uint8_t>(nuevo_valor_blue);
  }
  // hacemos el maxlevel con 100 de intensidad
  pequena1.maxlevel(10000);
  // recorremos el vector de nuevo y esta vez hacemos comprobaciones con los valores que deberian tener
  int salida_intens= pequena1.get_max_intensidad();
  ASSERT_EQ(salida_intens, 10000);
  for (size_t i = 0; i < tamano_matriz_unsigned; i++) {
    uint8_t rojo = pequena1.get_red(i);
    uint8_t verde = pequena1.get_green(i);
    uint8_t azul = pequena1.get_blue(i);
    ASSERT_EQ(expected_red[i], rojo);
    ASSERT_EQ(expected_green[i],verde);
    ASSERT_EQ(expected_blue[i], azul);
  }
}


