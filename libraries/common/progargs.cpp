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

void validate_resize(int argc, std::vector<std::string> argumentos) {
  // validate resize function inputs
  if (argc !=6) {
    std::cout << "Error: Invalid number of extra arguments for resize: " << abs(argc - 4) << "\n";
    exit(-1);
  }

  bool int_arg4 = isInt(argumentos[4]);
  bool int_arg5 = isInt(argumentos[5]);

  if ((!int_arg4) or std::stoi(argumentos[4]) <= 0){
    std::cout << "Error: Invalid resize width: " << argumentos[4] << "\n";
    exit(-1);
  }
  if ((!int_arg5) or std::stoi(argumentos[5]) <= 0){
    std::cout << "Error: Invalid resize heigh: " << argumentos[5] << "\n";
    exit(-1);
  }
}

void validate_maxlevel(int argc, std::vector<std::string> argumentos) {
  // validate maxlevel function inputs
  if (argc !=5) {
    std::cout << "Error: Invalid number of extra arguments for maxlevel: " << abs(argc - 4) << "\n";
    exit(-1);
  }

  bool int_arg4 = isInt(argumentos[4]);

  if ((!int_arg4) or std::stoi(argumentos[4]) <= 0){
    std::cout << "Error: Invalid maxlevel: " << argumentos[4] << "\n";
    exit(-1);
  }

  if (std::stoi(argumentos[4]) < 0 or std::stoi(argumentos[4]) > 65535){
    std::cout << "Error: Invalid maxlevel: " << argumentos[4] << "\n";
    exit(-1);
  }
}

void validate_info(int argc, std::vector<std::string> argumentos) {
  // validate info function inputs
  if (argc !=4) {
    std::cout << "Error: Invalid extra arguments for info: " << argumentos[4] << "\n";
    exit(-1);
  }
}

void validate_cutfreq(int argc, std::vector<std::string> argumentos) {
  // validate cutfreq function inputs
  if (argc != 5) {
    std::cout << "Error: Invalid number of extra arguments for cutfreq: " << abs(argc - 4) << "\n";
    exit(-1);
  }

  bool int_arg4 = isInt(argumentos[4]);

  if ((!int_arg4) or std::stoi(argumentos[4]) <= 0){
    std::cout << "Error: Invalid cutfreq:: " << argumentos[4] << "\n";
    exit(-1);
  }
}

void validate_compress(int argc, std::vector<std::string> argumentos) {
  // validate compress function inputs
  if (argc !=4) {
    std::cout << "Invalid extra arguments for compress: " << argumentos[4] << "\n";
    exit(-1);
  }
}

auto validate_parameters(int argc, std::vector<std::string> argumentos) -> bool {
  // validate all input parameters
  const std::string& operacion = argumentos[3];
  if (argc <= 3) {
    std::cout << "Error: Invalid number of arguments: " << argc-1 << "\n";
    exit(-1);
  } else if (operacion == "maxlevel") {
    validate_maxlevel(argc, argumentos);
  } else if (operacion == "resize") {
    validate_resize(argc, argumentos);
  } else if (operacion == "info") {
    validate_info(argc, argumentos);
  } else if (operacion == "cutfreq") {
    validate_cutfreq(argc, argumentos);
  } else if (operacion == "compress") {
    validate_compress(argc, argumentos);
  } else {
    std::cout << "Error: Invalid option: " << argumentos[3] << "\n";
    exit(-1);
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
