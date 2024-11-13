//
// Created by sergio on 12/11/24.
//

#ifndef IMTOOL_B_TREE_H
#define IMTOOL_B_TREE_H

#include <cstdint>
#include <vector>
#include <memory>
class b_tree {

  private:
    struct Nodo{
        int r, g, b;
        std::vector<int*> direcciones_colores;
        int cantidad_elementos = {1};
        int altura_nodo_izq = {0};
        int altura_nodo_der = {0};
        std::unique_ptr<Nodo> nodo_izq = {nullptr};
        std::unique_ptr<Nodo> nodo_der = {nullptr};
        Nodo(int red, int green, int blue): r(red), g(green), b(blue) {}
    };
    // Raiz que esta vacia
    std::unique_ptr<Nodo> root = nullptr;
    int counter = 0;
    // Retornea 2 si el new es mayor, 1 si el new es menor y 0 si el new es igual.
    auto crear_nodo(int& r, int& g, int& b);
    auto compare(int r_new, int g_new, int b_new, int r_old, int g_old, int b_old) -> int;
    void reequilibrar(std::vector<std::unique_ptr<Nodo>*> camino, std::vector<int> direcciones);
    void rotar_derecha(std::vector<std::unique_ptr<Nodo>*> camino,std::vector<int> direcciones, std::size_t i);
    void rotar_izquierda(std::vector<std::unique_ptr<Nodo>*> camino,std::vector<int> direcciones, std::size_t i);
    auto _in_order_traversal(std::unique_ptr<Nodo>* root) -> void;
  public:
    // Esta funcion nos retorneara cuantas apariciones han habido de ese elemento en total.
    // Si es la primera vez que llega pues hay 1. La segunda: 2. Etc....
    auto insertar(int& r, int& g, int& b) -> int;
    void in_order_traversal();
};

#endif  // IMTOOL_B_TREE_H
