//
// Created by sergio on 7/10/24.
//
#include "progargs.hpp"

#include <bitset>
#include <iostream>
#include <unordered_map>

auto isInt(std::string value) -> bool {
  try {
    std::stoi(value);
    return true;
  } catch (const std::exception&) {
    return false;
  }
}

auto validate_resize(int argc, std::vector<std::string> argumentos) -> bool {
  if (argc !=6) {
    std::cout << "Error: Invalid number of extra arguments for resize: " << abs(argc - 4);
    return false;
  }

  bool int_arg4 = isInt(argumentos[4]);
  bool int_arg5 = isInt(argumentos[5]);

  if ((!int_arg4) or std::stoi(argumentos[4]) <= 0){
    std::cout << "Error: Invalid resize width: " << argumentos[4];
    return false;
  }
  if ((!int_arg5) or std::stoi(argumentos[5]) <= 0){
    std::cout << "Error: Invalid resize heigh: " << argumentos[5];
    return false;
  }
  return true;
}

auto validate_parameters(int argc, std::vector<std::string> argumentos) -> bool {
  const std::string& operacion = argumentos[3];

  //std::cout << "Argc: " << argc << "\n" << argumentos[0] << "\n" << argumentos[1] << "\n" << argumentos[2] << "\n" << argumentos[3] << "\n";
  // int nueva_intensidad, nuevo_ancho, nuevo_alto;
  //// ICIAR METE VALORES TUYOS SI LO NECESITAS !!

  // gestionamos el tipo de operacion que sea:
  if ((operacion == "info" && argc != 4) || (operacion == "maxlevel" && argc != 5) ||(operacion == "compress" && argc != 4)) {
    std::cout << "Error: operacion incorrecta o numero de argumentos erroneos\n";
    return false;  // gestionamos que la operacion y el numero de argumentos son correctos
  }

  if (operacion == "resize") {
    validate_resize(argc, argumentos);
  }
  // si va bien, devolvemos true
  return true;
}

//calculo de la tabla de indices para la imagen comprimida
auto tablaIndices(size_t num, std::unordered_map<std::string, std::string> coloresUnicos) -> std::unordered_map<std::string, std::string> {
  if (num <= 255) { //dependiendo de la cantidad de colores, necesitamos 1B, 2B o 4B
    //para representar todos los indices
    for (auto const& color : coloresUnicos) {
      //int indice = color.second;
      unsigned long long indice = std::stoull(color.second);
      std::string ind = std::bitset<8>(indice).to_string();
      coloresUnicos[color.first] = ind;
    }
    return coloresUnicos;
  }
  else if (num > 255 && num < 65536) {
    //necesitamos 2B para representar todos los indices
    //return 16;
    for (auto const& color : coloresUnicos) {
      unsigned long long indice = std::stoull(color.second);
      std::string ind =(((std::bitset<16>(indice) >> 8) & std::bitset<16>(255))|
        ((std::bitset<16>(indice) << 8) & std::bitset<16>(65280))).to_string();
      coloresUnicos[color.first] = ind;
    }
    return coloresUnicos;
  }
  else if (num < 4294967296) {
    for (auto const& color : coloresUnicos) {
      unsigned long long indice= std::stoull(color.second);
      std::string ind =(((std::bitset<32>(indice)>> 24) & std::bitset<32>(255))|
        ((std::bitset<32>(indice) >> 8) & std::bitset<32>(65280))|
        ((std::bitset<32>(indice) <<8) & std::bitset<32>(16711680)) |
        ((std::bitset<32>(indice) << 24) & std::bitset<32>(4278190080))).to_string();
      coloresUnicos[color.first] = ind;
    }
    return coloresUnicos;
  }
  else {
    std::cerr << "El número de colores en la imagen es demasiado grande para ser representados en la imagen comprimida. \n";
    exit(1);  // salimos de la ejecucion si el numero de colores es demasiado grande
  }
}
