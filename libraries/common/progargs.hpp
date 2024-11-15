#ifndef COMP_ARCH_LAB1_PROGARGS_HPP
#define COMP_ARCH_LAB1_PROGARGS_HPP
#include <string>
#include <unordered_map>
#include <vector>

// Aquí irían las declaraciones de funciones, clases, etc.
auto validate_parameters(int argc, std::vector<std::string> argumentos) -> bool;
auto tablaIndices(size_t num, std::unordered_map<std::string, std::string> coloresUnicos) -> std::unordered_map<std::string, std::string>;
#endif  // COMP_ARCH_LAB1_PROGARGS_HPP
