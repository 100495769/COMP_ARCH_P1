//
// Created by sergio on 12/11/24.
//
#include "b_tree.hpp"
#include <cstdint>
#include <vector>
#include <memory>
#include <iostream>

//IMPORTANTE, CUANTO MAS A LA IZQUIERDA MAYOR ES EL NUMERO
auto b_tree::compare(std::uint8_t  r_new, std::uint8_t  g_new, std::uint8_t  b_new, std::uint8_t  r_old, std::uint8_t  g_old, std::uint8_t  b_old) -> int{
  // Retornea 2 si el new es mayor, 1 si el old es mayor y 0 si son iguales.
  auto new_key = std::tie(b_new, g_new, r_new);
  auto old_key = std::tie(b_old, g_old, r_old);

  if (new_key > old_key) return 2;
    if (old_key > new_key) return 1;
    return 0;

}
auto b_tree::crear_nodo(std::uint8_t  & r, std::uint8_t  & g, std::uint8_t  & b){
  // Crear el nodo.
  auto mi_nodo = std::make_unique<Nodo>(r, g, b);

  //Meter los primeros colores.
  mi_nodo->direcciones_colores.push_back(&r);
  mi_nodo->direcciones_colores.push_back(&g);
  mi_nodo->direcciones_colores.push_back(&b);
  mi_nodo->altura_nodo_der = 0;
  mi_nodo->altura_nodo_der = 0;
  return mi_nodo;
}
void b_tree::rotar_derecha(std::vector<std::unique_ptr<Nodo>*> camino,std::vector<int> direcciones, std::size_t i){
  std::unique_ptr<Nodo> temp;
  std::unique_ptr<Nodo>* nueva_raiz = &((*camino[i])->nodo_izq);

  if (i!=0){
    // Cambiar al que el padre apunta por el nuevo. Root no tiene padre
    if (direcciones[i-1] == 2) {
      //Si en el padre fuimos a la izquierda cambiamos la izquierda
      temp = std::move((*camino[i-1])->nodo_izq);
      (*camino[i-1])->nodo_izq = std::move(*nueva_raiz);
    }
    else if (direcciones[i-1] == 1) {
      //Si en el padre fuimos a la derecha cambiamos la derecha.
      temp = std::move((*camino[i-1])->nodo_der);
      (*camino[i-1])->nodo_der = std::move(*nueva_raiz);
    }
  }
  if ((*nueva_raiz) != nullptr && (*nueva_raiz)->nodo_der != nullptr){
  (*camino[i])->nodo_izq = std::move((*nueva_raiz)->nodo_der);}
  // Aqui el clang-tidy da un error que no tiene ningun sentido
  // por eso lo voy a ignorar.


  // Añadimos su altura si tiene.
  if ((*camino[i])->nodo_izq != nullptr){
    (*camino[i])->altura_nodo_izq = std::max((*camino[i])->nodo_izq->altura_nodo_der, (*camino[i])->nodo_izq->altura_nodo_izq) + 1;
  }

  // Añadimos la antigua raiz a la nueva posicion y ajustamos la altura.
  (*nueva_raiz)->nodo_der = std::move(temp);
  (*nueva_raiz)->altura_nodo_der = std::max((*nueva_raiz)->nodo_der->altura_nodo_der, (*nueva_raiz)->nodo_der->altura_nodo_izq) + 1;
  if (i == 0){
    // Esto indica que hay que cambiar la raiz asique:
    root = std::move(*nueva_raiz);
  }
}
void b_tree::rotar_izquierda(std::vector<std::unique_ptr<Nodo> *> camino, std::vector<int> direcciones, std::size_t i) {
  std::unique_ptr<Nodo> temp;
  std::unique_ptr<Nodo>* nueva_raiz = &((*camino[i])->nodo_der);

  if (i!=0){
    // Cambiar al que el padre apunta por el nuevo. Root no tiene padre
    if (direcciones[i-1] == 2) {
      //Si en el padre fuimos a la izquierda cambiamos la izquierda
      temp = std::move((*camino[i-1])->nodo_izq);
      (*camino[i-1])->nodo_izq = std::move(*nueva_raiz);
    }
    else if (direcciones[i-1] == 1) {
      //Si en el padre fuimos a la derecha cambiamos la derecha.
      temp = std::move((*camino[i-1])->nodo_der);
      (*camino[i-1])->nodo_der = std::move(*nueva_raiz);
    }
  }

  if ((*nueva_raiz) != nullptr && (*nueva_raiz)->nodo_izq != nullptr){
    (*camino[i])->nodo_der = std::move((*nueva_raiz)->nodo_izq);
  }
  // Aqui el clang-tidy da un error que no tiene ningun sentido
  // por eso lo voy a ignorar.

  // Añadimos su altura si tiene.
  if ((*camino[i])->nodo_der != nullptr){
    (*camino[i])->altura_nodo_der = std::max((*camino[i])->nodo_der->altura_nodo_der, (*camino[i])->nodo_der->altura_nodo_izq) + 1;
  }

  // Añadimos la antigua raiz a la nueva posicion y ajustamos la altura.
  (*nueva_raiz)->nodo_izq = std::move(temp);
  (*nueva_raiz)->altura_nodo_izq = std::max((*nueva_raiz)->nodo_izq->altura_nodo_der, (*nueva_raiz)->nodo_izq->altura_nodo_izq) + 1;
  if (i == 0){
    // Esto indica que hay que cambiar la raiz asique:
    root = std::move(*nueva_raiz);
  }
}

void b_tree::reequilibrar(std::vector<std::unique_ptr<Nodo>*> camino, std::vector<int> direcciones){
  for (std::size_t i = camino.size(); i > 0; --i) {
    // i será mayor por 1.
    if ((*camino[i-1])->altura_nodo_izq - (*camino[i-1])->altura_nodo_der > 1){
      // Si pasa esto es que esta desbalanceado teniendo la izquierda mas valores. Toca rotar a la derecha.
      rotar_derecha(camino, direcciones, i-1);

    }
    else if((*camino[i-1])->altura_nodo_der - (*camino[i-1])->altura_nodo_izq > 1){
      // Si pasa esto es que esta desbalanceado teniendo la izquierda mas valores. Toca rotar a la izquierda.
      rotar_izquierda(camino, direcciones, i-1);
    }
  }
}

auto b_tree::insertar(std::uint8_t & r, std::uint8_t & g, std::uint8_t & b, int posicion_color) -> int{

  // Tenemos un nodo para iterar por el arbol.
  std::vector<std::unique_ptr<Nodo>*> camino;
  std::vector<int> direcciones;
  std::unique_ptr<Nodo>* nodo_it = &root;
  int comparacion = 3;

  while(comparacion != 0){
    if ((*nodo_it) == nullptr){
      // Si no hay nodo lo creamos.
      (*nodo_it) = crear_nodo(r,g, b);
      (*nodo_it)->posiciones_colores.push_back(posicion_color);
      // Añadimos el peso al nodo anterior que corresponda teniendo en cuenta que la direccion tomada está guardad en comparación.
      if (camino.size()>= 1){
          if (comparacion == 2){
            // Venimos de la derecha
            (*camino.back())->altura_nodo_der ++;
          }
          else if (comparacion == 1){
            // Venimos de la izquierda
            (*camino.back())->altura_nodo_izq ++;
        }
        reequilibrar(camino, direcciones);
      }

      return 1;
    }
    comparacion = compare(r, g, b, (*nodo_it)->r, (*nodo_it)->g, (*nodo_it)->b);
    if (comparacion == 2){
      // Movemos al nodo izquierdo
      camino.push_back(nodo_it);
      direcciones.push_back(comparacion);
      nodo_it = &(*nodo_it)->nodo_izq;
    }
    else if (comparacion == 1){
      // Movemos al nodo izquierdo
      direcciones.push_back(comparacion);
      camino.push_back(nodo_it);
      nodo_it = &(*nodo_it)->nodo_der;
    }
  }
  (*nodo_it)->direcciones_colores.push_back(&r);
  (*nodo_it)->direcciones_colores.push_back(&g);
  (*nodo_it)->direcciones_colores.push_back(&b);
  (*nodo_it)->cantidad_elementos += 1;
  (*nodo_it)->posiciones_colores.push_back(posicion_color);
  return (*nodo_it)->cantidad_elementos;
}
auto b_tree::in_order_del_unfreq(int numero_de_frecuencias, int frecuencia_maxima)->void{
  _in_order_del_unfreq(&root, numero_de_frecuencias, frecuencia_maxima);
  std::cout<<"Colores eliminados: "<<colores_eliminados.size()<<" Colores_no_eliminados: "<<colores_no_eliminados.size()<<std::endl;
}
auto b_tree::_in_order_del_unfreq(std::unique_ptr<Nodo>* root, int &numero_de_frecuencias, int &frecuencia_maxima) -> void {
    // Empty Tree
    if (*root == nullptr)
        return;

    // Recur on the left subtree
    _in_order_del_unfreq(&(*root)->nodo_izq, numero_de_frecuencias, frecuencia_maxima);

    // Visit the current node
    counter++;
    if ((*root)->cantidad_elementos <= frecuencia_maxima){
      colores_eliminados.push_back(&(*root));
      std::cout << counter<< " Color eliminado:" << static_cast<int>((*root)->b) << static_cast<int>((*root)->g)  << static_cast<int>((*root)->r) << std::endl;
    }
    else if ((*root)->cantidad_elementos == frecuencia_maxima+1 && numero_de_frecuencias != 0){
      numero_de_frecuencias--;
      colores_eliminados.push_back(&(*root));

      std::cout << counter<< " Color eliminado:" << static_cast<int>((*root)->b) << static_cast<int>((*root)->g)  << static_cast<int>((*root)->r) << std::endl;
    }
    else{
      colores_no_eliminados.push_back(&(*root));
    }

    // Recur on the right subtree
    _in_order_del_unfreq(&(*root)->nodo_der, numero_de_frecuencias, frecuencia_maxima);
}
std::vector<std::vector<std::uint8_t>> b_tree::lista_colores_no_eliminados(){
  std::vector<std::vector<std::uint8_t >> lista;
  for (auto & col: colores_no_eliminados){
    lista.push_back({(*col)->r, (*col)->g, (*col)->b});
  }
  return lista;

}
std::vector<std::vector<std::uint8_t>> b_tree::lista_colores_eliminados(){
  std::vector<std::vector<std::uint8_t >> lista;
  for (auto & col: colores_eliminados){
    lista.push_back({(*col)->r, (*col)->g, (*col)->b});
  }
  return lista;

}
auto b_tree::in_order_fill_unfreq()->void{
  std::cout<<"Colores eliminados: "<<colores_eliminados.size()<<std::endl;
  _in_order_fill_unfreq(colores_eliminados, 1);
  //Coloreamos todos los colores con el color asignado
  for (int i = 0; i<colores_eliminados.size(); i++){
    for (int g = 0; g< (*colores_eliminados[i])->cantidad_elementos; g++){
      (*lista_rojo)[(*colores_eliminados[i])->posiciones_colores[g]] = (*colores_eliminados[i])->r_a;
      (*lista_azul)[(*colores_eliminados[i])->posiciones_colores[g]] = (*colores_eliminados[i])->g_a;
      (*lista_verde)[(*colores_eliminados[i])->posiciones_colores[g]] = (*colores_eliminados[i])->b_a;
    }
  }


}
void b_tree::_in_order_fill_unfreq(std::vector<std::unique_ptr<Nodo>*> &colores, int tamaño){  // Empty Tree
  if (colores.size() == 0){
    return;
  }
  std::vector<std::unique_ptr<Nodo>*> colores_restantes;
  std::vector<std::unique_ptr<Nodo>*> colores_rellenados;
  for (int i = 0; i < colores.size(); i++) {
    bool color_found = false;
    /*
    int g = 0;
    while ((color_found == false) && (g < (*colores[i])->cantidad_elementos)){
      // Vamos a comprobar los adyacentes por cada uno hasta encontrar un color.
      int pos = (*colores[i])->posiciones_colores[g];
      //Comprobamos arriba abajo izq derecha.
      if (pos-ancho_img >= 0) {
        // Comprobamos que no se salga
        if ((*lista_rojo)[pos - ancho_img] != red_del ||
            (*lista_verde)[pos - ancho_img] != green_del ||
            (*lista_azul)[pos - ancho_img] != blue_del) {

          for (int h = 0; h < (*colores[i])->cantidad_elementos; h += 1) {
            (*lista_rojo)[(*colores[i])->posiciones_colores[h]] = (*lista_rojo)[pos - ancho_img];
            (*lista_azul)[(*colores[i])->posiciones_colores[h]] = (*lista_verde)[pos - ancho_img];
            (*lista_verde)[(*colores[i])->posiciones_colores[h]] = (*lista_azul)[pos - ancho_img];
            color_found = true;
          }
        }
      }
      if (pos+ancho_img <= tamano_img) {
        // Comprobamos que no se salga
        if ((*lista_rojo)[pos + ancho_img] != red_del ||
            (*lista_verde)[pos + ancho_img] != green_del ||
            (*lista_azul)[pos + ancho_img] != blue_del) {
          for (int h = 0; h < (*colores[i])->cantidad_elementos; h += 1) {
            (*lista_rojo)[(*colores[i])->posiciones_colores[h]] = (*lista_rojo)[pos + ancho_img];
            (*lista_azul)[(*colores[i])->posiciones_colores[h]] = (*lista_verde)[pos + ancho_img];
            (*lista_verde)[(*colores[i])->posiciones_colores[h]] = (*lista_azul)[pos + ancho_img];
            color_found = true;
          }
        }
      }
      if (pos-1 >= 0) {
        // Comprobamos que no se salga
        if ((*lista_rojo)[pos - 1] != red_del || (*lista_verde)[pos - 1] != green_del ||
            (*lista_azul)[pos - 1] != blue_del) {
          for (int h = 0; h < (*colores[i])->cantidad_elementos; h += 1) {
            (*lista_rojo)[(*colores[i])->posiciones_colores[h]]     = (*lista_rojo)[pos - 1];
            (*lista_azul)[(*colores[i])->posiciones_colores[h]] = (*lista_verde)[pos - 1];
            (*lista_verde)[(*colores[i])->posiciones_colores[h]] = (*lista_azul)[pos - 1];
            color_found = true;
          }
        }
      }
      if (pos+1 <= tamano_img) {
        //Comprobamos que no se salga
        if ((*lista_rojo)[pos + 1] != red_del ||
            (*lista_verde)[pos + 1] != green_del ||
            (*lista_azul)[pos + 1] != blue_del) {
          for (int h = 0; h < (*colores[i])->cantidad_elementos; h+=1){
            (*lista_rojo)[(*colores[i])->posiciones_colores[h]]     = (*lista_rojo)[pos + 1];
            (*lista_azul)[(*colores[i])->posiciones_colores[h]]  = (*lista_verde)[pos + 1];
            (*lista_verde)[(*colores[i])->posiciones_colores[h]]  = (*lista_azul)[pos + 1];
            color_found = true;

          }
        }
      }
    g++;
    }*/

    color_found = radar_search(colores[i], tamaño);
    tamaño += 1;
    if (color_found == false){
      // Si no se ha encontrado el color esperamos al siguiente ciclo.
      colores_restantes.push_back(colores[i]);
    }
    else{
      colores_rellenados.push_back(colores[i]);
    }
    std::cout<<"Colores restantes: "<<colores_restantes.size()<<" Colores rellenados: "<<colores_rellenados.size()<<std::endl;
  }


  std::cout<<"Vuelta hay: "<< colores_restantes.size()<<std::endl;
  _in_order_fill_unfreq(colores_restantes, tamaño);
}
bool b_tree::radar_search(std::unique_ptr<Nodo>* color, int tamaño){
  int x = tamaño / 2;
  int y = tamaño /2 + tamaño%2;
  bool found_color = false;
  //En nuestra funcion vamos a ir restando de x y sumandole a y.
  while ((x >= 0) && (found_color == false)) {
   // std::cout<<x<<"Pipo"<<y<<std::endl;
    for (int i = 0; i < (*color)->cantidad_elementos; i++) {
      for (int sign_x = -1; sign_x <= 1; sign_x += 2) {
        for (int sign_y = -1; sign_y <= 1; sign_y += 2) {
          // Antes de comprobar si ese color existe comprobamos que este la posicion en la imagen. Que sino da errores.
          if ((0 <= (*color)->posiciones_colores[i] + sign_x * x + sign_y * y * ancho_img) &&
              ((*color)->posiciones_colores[i] + sign_x * x + sign_y * y * ancho_img <= tamano_img)) {
            if ((*lista_rojo)[(*color)->posiciones_colores[i] + sign_x * x + sign_y * y * ancho_img] !=
                    red_del ||
                (*lista_verde)[(*color)->posiciones_colores[i] + sign_x * x + sign_y * y * ancho_img] !=
                    green_del ||
                (*lista_azul)[(*color)->posiciones_colores[i] + sign_x * x + sign_y * y * ancho_img] !=
                    blue_del) {
              if (found_color == false ||
                  compare((*lista_rojo)[(*color)->posiciones_colores[i] + sign_x * x + sign_y * y * ancho_img],
                          (*lista_verde)[(*color)->posiciones_colores[i] + sign_x * x + sign_y * y * ancho_img],
                          (*lista_azul)[(*color)->posiciones_colores[i] + sign_x * x + sign_y * y * ancho_img],
                          (*color)->r_a, (*color)->g_a, (*color)->b_a) == 2) {
                (*color)->r_a = (*lista_rojo)[(*color)->posiciones_colores[i] + sign_x * x + sign_y * y * ancho_img];
                (*color)->g_a = (*lista_verde)[(*color)->posiciones_colores[i] + sign_x * x + sign_y * y * ancho_img];
                (*color)->b_a = (*lista_azul)[(*color)->posiciones_colores[i] + sign_x * x + sign_y * y * ancho_img];
                found_color   = true;
              }
            }
          }
          if ((0 <= (*color)->posiciones_colores[i] + sign_y * y + sign_x * x * ancho_img) &&
              ((*color)->posiciones_colores[i] + sign_y * y + sign_x * x * ancho_img <=
               tamano_img)) {
            if ((*lista_rojo)[(*color)->posiciones_colores[i] + sign_y * y +
                              sign_x * x * ancho_img] != red_del ||
                (*lista_verde)[(*color)->posiciones_colores[i] + sign_y * y +
                               sign_x * x * ancho_img] != green_del ||
                (*lista_azul)[(*color)->posiciones_colores[i] + sign_y * y +
                              sign_x * x * ancho_img] != blue_del) {
              if (found_color == false ||
                  compare((*lista_rojo)[(*color)->posiciones_colores[i] + sign_y * y +
                                        sign_x * x * ancho_img],
                          (*lista_verde)[(*color)->posiciones_colores[i] + sign_y * y +
                                         sign_x * x * ancho_img],
                          (*lista_azul)[(*color)->posiciones_colores[i] + sign_y * y +
                                            sign_x * x * ancho_img],
                          (*color)->r_a, (*color)->g_a, (*color)->b_a) == 2) {
                (*color)->r_a = (*lista_rojo)[(*color)->posiciones_colores[i] + sign_y * y +
                                              sign_x * x * ancho_img];
                (*color)->g_a = (*lista_verde)[(*color)->posiciones_colores[i] + sign_y * y +
                                               sign_x * x * ancho_img];
                (*color)->b_a = (*lista_azul)[(*color)->posiciones_colores[i] + sign_y * y +
                                              sign_x * x * ancho_img];
                found_color   = true;
              }
            }
          }
        }
      }
    }
      x--;
      y++;
  }
  std::cout<<"ENCONTRE"<<std::endl;
  return found_color;

    // Cuando compruebo lo hago arriba, abajo, izquierda derecha.
    // Estos tienen la relacion de que abajo y derecha seran con (pos + x,  pos + y) e (pos + y, pos + x)
    // Y arriba y izquierda seran con (pos - x,  pos - y) e (pos - y, pos - x)
    // Coincidiran en la diagonal y nose si esto lo explico mejor en el pdf TODO
  }

void b_tree::rellenar_datos(std::vector<uint8_t> * rojo, std::vector<uint8_t> * verde,
                             std::vector<uint8_t> * azul, int &ancho, int &alto) {
  lista_rojo = rojo;
  lista_verde = verde;
  lista_azul = azul;
  ancho_img = ancho;
  alto_img = alto;
  tamano_img = ancho * alto;
}
