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
  if (nueva_raiz && (*nueva_raiz) && (*nueva_raiz)->nodo_der){
	 (*camino[i])->nodo_izq = std::move((*nueva_raiz)->nodo_der);
			  }
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

  if (nueva_raiz && (*nueva_raiz) && (*nueva_raiz)->nodo_izq){
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

auto b_tree::insertar(std::uint8_t & r, std::uint8_t & g, std::uint8_t & b, size_t posicion_color) -> std::size_t{

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
auto b_tree::in_order_del_unfreq(std::size_t numero_de_frecuencias, std::size_t frecuencia_maxima)->void{
  _in_order_del_unfreq(&root, numero_de_frecuencias, frecuencia_maxima);
  std::cout<<"Colores eliminados: "<<colores_eliminados.size()<<" Colores_no_eliminados: "<<colores_no_eliminados.size()<<std::endl;
}
auto b_tree::_in_order_del_unfreq(std::unique_ptr<Nodo>* raiz, std::size_t &numero_de_frecuencias, std::size_t &frecuencia_maxima) -> void {
    // Empty Tree
    if (*raiz == nullptr)
        return;

    // Recur on the left subtree
    _in_order_del_unfreq(&(*raiz)->nodo_izq, numero_de_frecuencias, frecuencia_maxima);

    // Visit the current node
    counter++;
    if ((*raiz)->cantidad_elementos <= frecuencia_maxima){
      colores_eliminados.push_back(&(*raiz));
      std::cout << counter<< " Color eliminado:" << static_cast<int>((*raiz)->b) << static_cast<int>((*raiz)->g)  << static_cast<int>((*raiz)->r) << std::endl;
    }
    else if ((*raiz)->cantidad_elementos == frecuencia_maxima + 1 && numero_de_frecuencias > 0){
      numero_de_frecuencias--;
      colores_eliminados.push_back(&(*raiz));

      std::cout << counter<< " Color eliminado:" << static_cast<int>((*raiz)->b) << static_cast<int>((*raiz)->g)  << static_cast<int>((*raiz)->r) << std::endl;
    }
    else{
      colores_no_eliminados.push_back(&(*raiz));
    }

    // Recur on the right subtree
    _in_order_del_unfreq(&(*raiz)->nodo_der, numero_de_frecuencias, frecuencia_maxima);
}
auto b_tree::lista_colores_no_eliminados() -> std::vector<std::vector<std::uint8_t>> {
  std::vector<std::vector<std::uint8_t >> lista;
  for (auto & col: colores_no_eliminados){
    lista.push_back({(*col)->r, (*col)->g, (*col)->b});
  }
  return lista;

}
auto b_tree::lista_colores_eliminados() -> std::vector<std::vector<std::uint8_t>>{
  std::vector<std::vector<std::uint8_t >> lista;
  for (auto & col: colores_eliminados){
    lista.push_back({(*col)->r, (*col)->g, (*col)->b});
  }
  return lista;

}

void b_tree::rellenar_datos(std::vector<uint8_t> * rojo, std::vector<uint8_t> * verde,
                             std::vector<uint8_t> * azul, size_t ancho, size_t alto) {
  lista_rojo = rojo;
  lista_verde = verde;
  lista_azul = azul;
  ancho_img = ancho;
  alto_img = alto;
  tamano_img = ancho * alto;
}
