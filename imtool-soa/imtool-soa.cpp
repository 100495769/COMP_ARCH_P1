//
// Created by sergio on 7/10/24.
//
#include "../libraries/imgsoa/imagesoa.hpp"
#include "../libraries/common/progargs.hpp"
#include <iostream>
#include <string>

auto main(int argc, char* argv[]) -> int {
  // cogemos primero los argumentos de entrada y los validamos con el validador de progargs
  std::vector<std::string> argumentos(argv, argv + argc);

  if (!validate_parameters(argc, argumentos)) {
    return 1; // si los parametros no son validos, terminamos
  }
  std::string imagen_path = argv[1];
  std::string salida_path = argv[2];
  std::string operacion = argv[3];

  ImageSOA mi_imagen;
  mi_imagen.cargar_imagen(imagen_path);   // cargamos la imagen como tal (esta funcion ya verifica los datos)
  // gestionamos el tipo de operacion que sea:
  if (operacion == "info"){   // mostramos la información de los metadatos
    mi_imagen.info();
  }

  else if (operacion == "maxlevel") {  // la posicion 4 del vector de argumentos es la nueva intensidad. lo pasamos a entero
    mi_imagen.maxlevel(std::stoi(argv[4]));
  }
  else if (operacion == "resize") {   // primero obtenemos los parametros extra: [4] es ancho, [5] es alto
    mi_imagen.resize(std::stoi(argv[4]), std::stoi(argv[5]));
  }
  else if (operacion == "cutfreq"){
    mi_imagen.cutfreq(std::stoi(argv [4]));
  }
  else if (operacion == "compress") {
    std::tuple<size_t, std::vector<std::string>> compresion = mi_imagen.compress();
    mi_imagen.guardar_compress(salida_path, compresion);
  }
  else{  // Cualquier otra cosa genera un error
    std::cerr << "Error: Operacion no valida\n";
    return 1;
  }
  if (operacion != "info" && operacion != "compress") {    // guardamos la imagen con los cambios realizados
    mi_imagen.guardar_imagen(salida_path);
  }
  return 0;
}

