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

struct pixel {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};
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
