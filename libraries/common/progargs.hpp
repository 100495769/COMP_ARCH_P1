#ifndef COMP_ARCH_LAB1_PROGARGS_HPP
#define COMP_ARCH_LAB1_PROGARGS_HPP
#include <string>
#include <unordered_map>
#include <vector>

// Aquí irían las declaraciones de funciones, clases, etc.
auto isInt(std::string value) -> bool;
void validate_resize(int argc, std::vector<std::string> argumentos);
void validate_maxlevel(int argc, std::vector<std::string> argumentos);
void validate_info(int argc, std::vector<std::string> argumentos);
void validate_cutfreq(int argc, std::vector<std::string> argumentos);
void validate_compress(int argc, std::vector<std::string> argumentos);
auto validate_parameters(int argc, std::vector<std::string> argumentos) -> bool;
auto tablaIndices(size_t num, std::unordered_map<std::string, std::string> coloresUnicos) -> std::unordered_map<std::string, std::string>;
#endif  // COMP_ARCH_LAB1_PROGARGS_HPP
