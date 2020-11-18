#include <stdlib.h>
#include <stdio.h>
#include "abb.h"

#define EXITO 0
#define ERROR 1

#define PRIMER_ELEMENTO_MAYOR 1
#define IGUALES 0
#define SEGUNDO_ELEMENTO_MAYOR -1

#define IZQ -1
#define DER 1

typedef struct cosita{
    float clave;
    char contenido[10];
}cosita_t;
/*
    Crea un nodo cuyo elemento es el pasado por parámetros.
    En caso de no poder crearlo devuelve NULL.
*/
nodo_abb_t* crear_nodo(void* elemento){
    nodo_abb_t* nodo = malloc(sizeof(nodo_abb_t));

    if(!nodo)
        return NULL;

    nodo->derecha = NULL;
    nodo->izquierda = NULL;
    nodo->elemento = elemento;
    return nodo;
}

/*
    Busca el nodo que contenga el elemento, empezando a buscar desde el nodo pasado por parámetros.
    Devuelve el nodo encontrado.
    Devuelve el elemento anterior antes de llegar a NULL en caso de no encontrarlo.
*/
nodo_abb_t* buscar_nodo(nodo_abb_t* nodo, abb_comparador comparador, void* elemento){
    if(!nodo)
        return NULL;
    
    if(comparador(nodo->elemento, elemento) == IGUALES)
        return nodo;
    
    if(comparador(nodo->elemento, elemento) == PRIMER_ELEMENTO_MAYOR){
        if(!nodo->izquierda)
            return nodo;

        return buscar_nodo(nodo->izquierda, comparador, elemento);
    }
    
    if(comparador(nodo->elemento, elemento) == SEGUNDO_ELEMENTO_MAYOR){
        if(!nodo->derecha)
            return nodo;
            
        return buscar_nodo(nodo->derecha, comparador, elemento);
    }

    return NULL;
    
}

/*
 * Crea el arbol y reserva la memoria necesaria de la estructura.
 * Comparador se utiliza para comparar dos elementos.
 * Destructor es invocado sobre cada elemento que sale del arbol,
 * puede ser NULL indicando que no se debe utilizar un destructor.
 *
 * Devuelve un puntero al arbol creado o NULL en caso de error.
 */
abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){
    if(!comparador)
        return NULL;

    abb_t* arbol = malloc(sizeof(abb_t));

    if(!arbol)
        return NULL;

    arbol->comparador = comparador;
    arbol->destructor = destructor;
    arbol->nodo_raiz = NULL;

    return arbol;
}
/*
 * Inserta un elemento en el arbol.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 * El arbol admite elementos con valores repetidos.
 */
int arbol_insertar(abb_t* arbol, void* elemento){
    if(!arbol)
        return ERROR;
    
    nodo_abb_t* nodo = crear_nodo(elemento);

    if(!nodo)
        return ERROR;

    if(!arbol->nodo_raiz){
        arbol->nodo_raiz = nodo;
        return EXITO;
    }
    
    nodo_abb_t* nodo_padre = buscar_nodo(arbol->nodo_raiz, arbol->comparador, elemento);
    
    if(!nodo_padre)
        return ERROR;
    
    //printf("nodo que quiere insertar: %f - nodo padre: %f", ((cosita_t*)elemento)->clave, ((cosita_t*)(nodo_padre->elemento))->clave);
    if(arbol->comparador(nodo_padre->elemento, elemento) == IGUALES ){
        nodo_padre->izquierda = nodo;
    }
    
    if(arbol->comparador(nodo_padre->elemento, elemento) == PRIMER_ELEMENTO_MAYOR){
        nodo_padre->izquierda = nodo;
    }
    
    if(arbol->comparador(nodo_padre->elemento, elemento) == SEGUNDO_ELEMENTO_MAYOR){
        nodo_padre->derecha = nodo;
    }
    return EXITO;
}

/*
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación).
 *
 * Devuelve el elemento encontrado o NULL si no lo encuentra.
 */
void* arbol_buscar(abb_t* arbol, void* elemento){
  if(!arbol)
    return NULL;

  nodo_abb_t* nodo_padre = buscar_nodo(arbol->nodo_raiz, arbol->comparador, elemento);

  if(!nodo_padre)
    return NULL;

  if(arbol->comparador(nodo_padre->elemento, elemento) != IGUALES )
    return NULL;

  return nodo_padre->elemento;
}

void borrar(nodo_abb_t* nodo_padre, nodo_abb_t* nodo_a_borrar, int lado, void (*destructor)(void *)){
  if(!nodo_a_borrar->izquierda && !nodo_a_borrar->derecha){
    if(lado == IZQ)
      nodo_padre->izquierda = NULL;
    
    if(lado == DER)
      nodo_padre->derecha = NULL;
  }

  /*if(lado == IZQ){
    if(nodo_a_borrar->izquierda && !nodo_a_borrar->derecha){
      nodo_padre->izquierda = nodo_a_borrar->izquierda;
    } else if(nodo_a_borrar->derecha && !nodo_a_borrar->izquierda){
      nodo_padre->izquierda = nodo_a_borrar->derecha;
    } else if(nodo_a_borrar->derecha && nodo_a_borrar->izquierda){
      if(nodo)
    }
  }

   if(lado == DER){
    if(nodo_a_borrar->izquierda && !nodo_a_borrar->derecha){
      nodo_padre->derecha = nodo_a_borrar->izquierda;
    }

    if(nodo_a_borrar->derecha && !nodo_a_borrar->izquierda){
      nodo_padre->derecha = nodo_a_borrar->derecha;
    }
  }*/

    if(destructor)
      destructor(nodo_a_borrar->elemento);
    free(nodo_a_borrar);
  }

/*
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación) y si lo encuentra lo quita del arbol.
 * Adicionalmente, si encuentra el elemento, invoca el destructor con
 * dicho elemento.
 * Devuelve 0 si pudo eliminar el elemento o -1 en caso contrario.
 */
int arbol_borrar(abb_t* arbol, void* elemento){
  if(!arbol)
    return ERROR;

  nodo_abb_t* nodo_padre = buscar_nodo(arbol->nodo_raiz, arbol->comparador, elemento);

  if(!nodo_padre)
    return ERROR;

  /*if(arbol->comparador(nodo_padre->elemento, elemento) == IGUALES){
    borrar(nodo_padre, 1, arbol->destructor);
    return ERROR;
  }*/

  return EXITO;
}



void* arbol_raiz(abb_t* arbol){
    if(!arbol || !arbol->nodo_raiz)
        return NULL;
    
    return arbol->nodo_raiz->elemento;
}

bool arbol_vacio(abb_t* arbol){
    return !arbol || !arbol->nodo_raiz;
}


void inorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* cantidad_actual){
  if(!nodo || *cantidad_actual == tamanio_array)
    return;
  
  if(nodo->izquierda)
    inorden(nodo->izquierda, array, tamanio_array, cantidad_actual);

  array[(*cantidad_actual)++] = nodo->elemento;

  if(nodo->derecha)
    inorden(nodo->derecha, array, tamanio_array, cantidad_actual);
}

void preorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* cantidad_actual){
  if(!nodo || *cantidad_actual == tamanio_array)
    return;
  
  array[(*cantidad_actual)++] = nodo->elemento;

  if(nodo->izquierda)
    preorden(nodo->izquierda, array, tamanio_array, cantidad_actual);


  if(nodo->derecha)
    preorden(nodo->derecha, array, tamanio_array, cantidad_actual);
}

void postorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* cantidad_actual){
  if(!nodo || *cantidad_actual == tamanio_array)
    return;

  if(nodo->izquierda)
    postorden(nodo->izquierda, array, tamanio_array, cantidad_actual);

  if(nodo->derecha)
    postorden(nodo->derecha, array, tamanio_array, cantidad_actual);

  array[(*cantidad_actual)++] = nodo->elemento;
}

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia inorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array){
  if(!arbol || !arbol->nodo_raiz || !array)
    return 0;
  
  size_t cantidad_actual = 0;
  inorden(arbol->nodo_raiz, array, tamanio_array, &cantidad_actual);
  return cantidad_actual;
}

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia preorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array){
  if(!arbol || !arbol->nodo_raiz || !array)
    return 0;
  
  size_t cantidad_actual = 0;
  preorden(arbol->nodo_raiz, array, tamanio_array, &cantidad_actual);
  return cantidad_actual;
}


/*
 * Destruye el arbol liberando la memoria reservada por el mismo.
 * Adicionalmente invoca el destructor con cada elemento presente en
 * el arbol.
 */
void arbol_destruir(abb_t* arbol){
    free(arbol);
}
