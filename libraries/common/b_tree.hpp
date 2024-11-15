//
// Created by sergio on 12/11/24.
//

#ifndef IMTOOL_B_TREE_H
#define IMTOOL_B_TREE_H

#include <cstdint>
#include <vector>
#include <memory>
/*
class b_tree {

  private:

    struct Nodo{
        std::uint8_t r, g, b;
        // Lo de abajo es el color asignado.
        std::uint8_t r_a, g_a, b_a;
        std::vector<std::uint8_t*> direcciones_colores;
        std::vector<int> posiciones_colores;
        int cantidad_elementos = {1};
        int altura_nodo_izq = {0};
        int altura_nodo_der = {0};
        std::unique_ptr<Nodo> nodo_izq = {nullptr};
        std::unique_ptr<Nodo> nodo_der = {nullptr};
        Nodo(std::uint8_t red, std::uint8_t green, std::uint8_t  blue): r(red), g(green), b(blue) {}
    };
    // Raiz que esta vacia
    std::unique_ptr<Nodo> root = nullptr;
    std::vector<std::uint8_t>*lista_rojo, *lista_verde, *lista_azul;
    int ancho_img;
    int alto_img;
    int tamano_img;
    int counter = 0;
    std::vector<std::unique_ptr<Nodo>*> colores_eliminados;
    std::vector<std::unique_ptr<Nodo>*> colores_no_eliminados;
    // Aca se guardan tuplas de formato (posicion en colores_eliminados
    bool first_del = true;
    // Retornea 2 si el new es mayor, 1 si el new es menor y 0 si el new es igual.
    auto crear_nodo(std::uint8_t & r, std::uint8_t & g, std::uint8_t & b);
    void reequilibrar(std::vector<std::unique_ptr<Nodo>*> camino, std::vector<int> direcciones);
    void rotar_derecha(std::vector<std::unique_ptr<Nodo>*> camino,std::vector<int> direcciones, std::size_t i);
    void rotar_izquierda(std::vector<std::unique_ptr<Nodo>*> camino,std::vector<int> direcciones, std::size_t i);
    auto _in_order_del_unfreq(std::unique_ptr<Nodo>* root, int &numero_de_frecuencias, int &frecuencia_maxima) -> void;
    void _in_order_fill_unfreq(std::vector<std::unique_ptr<Nodo>*> &colores, int tamaño);




  public:
    // Esta funcion nos retorneara cuantas apariciones han habido de ese elemento en total.
    // Si es la primera vez que llega pues hay 1. La segunda: 2. Etc....
    void rellenar_datos(std::vector<uint8_t> *rojo,std::vector<uint8_t> *verde, std::vector<uint8_t> *azul, int &ancho, int &alto);
    std::uint8_t red_del, green_del, blue_del;
    auto insertar(std::uint8_t & r, std::uint8_t & g, std::uint8_t & b, int posicion_color) -> int;
    void in_order_del_unfreq(int numero_de_frecuencias, int frecuencia_maxima);
    void in_order_fill_unfreq();
    bool radar_search(std::unique_ptr<Nodo> *color, int tamaño);
    std::vector<std::vector<std::uint8_t>> lista_colores_no_eliminados();
    std::vector<std::vector<std::uint8_t>> lista_colores_eliminados();
    auto compare(std::uint8_t  r_new, std::uint8_t  g_new, std::uint8_t b_new, std::uint8_t  r_old, std::uint8_t  g_old, std::uint8_t b_old) -> int;
};
*/
#endif  // IMTOOL_B_TREE_H
