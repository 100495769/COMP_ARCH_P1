//
// Created by sergio on 7/10/24.
//
//#include <cmake-build-debug/_deps/googletest-src/googlemock/include/gmock/gmock-actions.h>
//#include <cmake-build-debug/_deps/googletest-src/googlemock/include/gmock/gmock-function-mocker.h>
//#include <cmake-build-debug/_deps/googletest-src/googlemock/include/gmock/gmock-spec-builders.h>
#include <gtest/gtest.h>
#include <libraries/imgaos/imageaos.hpp>
#include <string>
#include <unordered_map>
#include <vector>
/*
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
// TEST PARA LA FUNCION compress() de la estructura ImageAOS
class MockImageAOS : public ImageAOS {
  MOCK_METHOD(std::unordered_map<std::string, std::string>, tablaIndices, (size_t num, const std::unordered_map<std::string, std::string>& coloresUnicos), ());

};

TEST(ImageAOS_TEST, compress){
  // TODO: Implementar test para la función compress()
  MockImageAOS mock;
  std::unordered_map<std::string, std::string> coloresUnicos = {{"rojo", "ff0000"}, {"azul", "0000ff"}, {"amarillo", "ffff00"}};
  std::vector<std::string> pixelIndices = {"111111", "000000", "111111"};
  EXPECT_CALL(mock, tablaIndices(3, coloresUnicos)).WillOnce(::testing::Return(coloresUnicos));
  EXPECT_CALL(mock, guardar_compress(testing::_, std::make_tuple(3, pixelIndices))).WillOnce(::testing::Return());

  mock.compress();
  EXPECT_CALL(mock, subfuncionA(5)).WillOnce(::testing::Return(15));

  ASSERT_EQ(2,2) << "moneda moneda billete";

}*/


TEST(ImageAOS_TEST, tablaIndices) {
  ImageAOS mi_imagen;
  // numero de colores unicos a probar sonmenos de 255 un 1 B
  size_t coloresUnicosNum_prueba = 3;
  std::unordered_map<std::string, std::string> coloresUnicos_prueba = {{"163159148", "1"}, {"1008173", "2"}, {"2562560", "3"}};
  std::unordered_map<std::string, std::string> coloresUnicos_devueltos = mi_imagen.tablaIndices(coloresUnicosNum_prueba, coloresUnicos_prueba);
  std::unordered_map<std::string, std::string> coloresUnicos_esperados = {{"163159148", "00000001"}, {"1008173", "00000010"}, {"2562560", "00000011"}};
  ASSERT_EQ(coloresUnicos_devueltos, coloresUnicos_esperados);

  size_t coloresUnicosNum_prueba2 = 300;
  std::unordered_map<std::string, std::string> coloresUnicos_prueba2 = {{"163159148", "4790"}, {"1008173", "2"}, {"2562560", "3"}};
  std::unordered_map<std::string, std::string> coloresUnicos_devueltos2 = mi_imagen.tablaIndices(coloresUnicosNum_prueba2, coloresUnicos_prueba2);
  std::unordered_map<std::string, std::string> coloresUnicos_esperados2 = {{"163159148", "1011011000010010"}, {"1008173", "0000001000000000"}, {"2562560", "0000001100000000"}};
  ASSERT_EQ(coloresUnicos_devueltos2, coloresUnicos_esperados2);
  // Aunque no tengamos el número de co

  size_t coloresUnicosNum_prueba3 = 70000;
  std::unordered_map<std::string, std::string> coloresUnicos_prueba3 = {{"163159148", "4790"}, {"1008173", "827321754"}, {"2562560", "3"}};
  std::unordered_map<std::string, std::string> coloresUnicos_devueltos3 = mi_imagen.tablaIndices(coloresUnicosNum_prueba3, coloresUnicos_prueba3);
  std::unordered_map<std::string, std::string> coloresUnicos_esperados3 = {{"163159148", "10110110000100100000000000000000"}, {"1008173", "10011010111011010100111100110001"}, {"2562560", "00000011000000000000000000000000"}};
  ASSERT_EQ(coloresUnicos_devueltos3, coloresUnicos_esperados3);

  size_t coloresUnicosNum_prueba4 = 1203824932334;
  std::unordered_map<std::string, std::string> coloresUnicos_prueba4 = {{"163159148", "4790"}, {"1008173", "827321754"}, {"2562560", "3"}};
  std::unordered_map<std::string, std::string> coloresUnicos_devueltos4 = mi_imagen.tablaIndices(coloresUnicosNum_prueba3, coloresUnicos_prueba3);
  std::unordered_map<std::string, std::string> coloresUnicos_esperados4 = {{"163159148", "10110110000100100000000000000000"}, {"1008173", "10011010111011010100111100110001"}, {"2562560", "00000011000000000000000000000000"}};
  ASSERT_EQ(coloresUnicos_devueltos3, coloresUnicos_esperados3);
}