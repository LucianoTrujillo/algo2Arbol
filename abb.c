#include <stdlib.h>
#include <stdio.h>
#include "abb.h"

#define EXITO 0
#define ERROR 1

#define PRIMER_ELEMENTO_MAYOR 1
#define IGUALES 0
#define SEGUNDO_ELEMENTO_MAYOR -1

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
        if(!nodo->izquierda)
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
    
    if(arbol->comparador(nodo_padre->elemento, elemento) == IGUALES){
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


void* arbol_raiz(abb_t* arbol){
    if(!arbol || !arbol->nodo_raiz)
        return NULL;
    
    return arbol->nodo_raiz->elemento;
}

bool arbol_vacio(abb_t* arbol){
    return !arbol || !arbol->nodo_raiz;
}

/*
 * Destruye el arbol liberando la memoria reservada por el mismo.
 * Adicionalmente invoca el destructor con cada elemento presente en
 * el arbol.
 */
void arbol_destruir(abb_t* arbol){
    free(arbol);
}
