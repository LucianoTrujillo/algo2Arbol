#include <string.h>
#include <stdlib.h>
#include "abb.h"
#include "pa2mm.h"

#define MAX_CLAVES 100
#define DELIM ","
#define EXITO 0
#define ERROR 1

#define PRIMER_ELEMENTO_MAYOR 1
#define IGUALES 0
#define SEGUNDO_ELEMENTO_MAYOR -1


typedef struct cosita{
    float clave;
    char contenido[10];
}cosita_t;

bool mostrar_elemento(void* elemento, void* extra){
    if(elemento)
        printf("%.2f ", ((cosita_t*)elemento)->clave);
    return false;
}

cosita_t* crear_cosita(float clave){
    cosita_t* cosita = (cosita_t*)malloc(sizeof(cosita));
    if(cosita)
        cosita->clave = clave;
    return cosita;
}

void destruir_cosita(void* cosita){
    if(cosita)
        free((cosita_t*)cosita);
}


/*
    Comparador de cositas. Recibe dos cositas y devuelve
    0 en caso de ser iguales o que al menos una no exista.
    Devuelve 1 si la clave del primer elemento es mayor y -1 si es menor.
 */
int comparar_cositas(void* elemento1, void* elemento2){
    if(!elemento1 || !elemento2)
        return IGUALES;

    if( ((cosita_t*)elemento1)->clave > ((cosita_t*)elemento2)->clave)
        return PRIMER_ELEMENTO_MAYOR;
    if( ((cosita_t*)elemento1)->clave < ((cosita_t*)elemento2)->clave)
        return SEGUNDO_ELEMENTO_MAYOR;

    return IGUALES;
}

/*
    Crea un arbol insertando nodos en el orden pasado por parametros
    Cada elemento debe estar separado por comas.
    ej: crear_arbol_con_elementos("1.0,123.45,32,12.12,2.4,53"); 
*/
abb_t* crear_arbol_con_elementos(char* claves){
    abb_t* arbol = arbol_crear(comparar_cositas, destruir_cosita);
    if(!arbol)
        return NULL;

    char* clave = NULL;
    clave = strtok(claves, DELIM);

    while(clave != NULL){
        float clave_float = (float)atof(clave);
        cosita_t* cosita = crear_cosita(clave_float);
        if(!cosita){
            arbol_destruir(arbol);
            return NULL;
        }
        arbol_insertar(arbol, cosita);
        clave = strtok(NULL, DELIM);
    }

    return arbol;
}

void probar_creacion(){
    pa2m_nuevo_grupo("CREACIÓN");

    pa2m_afirmar(arbol_crear(NULL, destruir_cosita) == NULL, "No se puede crear crear un abb sin comparador");

    abb_t* arbol = arbol_crear(comparar_cositas, NULL);
    pa2m_afirmar(arbol->comparador == comparar_cositas, "El comparador que utiliza el abb es el pasado por parametros");
    pa2m_afirmar(arbol->destructor == NULL, "Se puede crear un abb sin destructor");
    pa2m_afirmar(arbol->nodo_raiz == NULL, "El nodo raíz de un abb recién inicializado es nulo");

    arbol_destruir(arbol);
} 

void probar_insertar(){
    pa2m_nuevo_grupo("INSERTAR");
    float claves_esperadas[MAX_CLAVES] = {0,1,2,2.5,3,4,5,6,6.5,7};
    void* cositas_recorridas[MAX_CLAVES];

    cosita_t* cosita0 = crear_cosita(0);
    cosita_t* cosita1 = crear_cosita(1);
    cosita_t* cosita2 = crear_cosita(2);
    cosita_t* cosita2coma5 = crear_cosita(2.5);
    cosita_t* cosita3 = crear_cosita(3);
    cosita_t* cosita4 = crear_cosita(4);
    cosita_t* cosita5 = crear_cosita(5);
    cosita_t* cosita6 = crear_cosita(6);
    cosita_t* cosita6coma5 = crear_cosita(6.5);
    cosita_t* cosita7 = crear_cosita(7);

    pa2m_afirmar(arbol_insertar(NULL, NULL) == ERROR, "Devuelve error si no existe el arbol");

    abb_t* arbol = arbol_crear(comparar_cositas, destruir_cosita);
    pa2m_afirmar(arbol_insertar(arbol, cosita2) == EXITO, "Devuelve exito al insertar un elemento");
    pa2m_afirmar(arbol_raiz(arbol) == cosita2, "Al insertar un elemento en un árbol vacío queda como el elemento raíz");

    arbol_insertar(arbol, cosita3);
    arbol_insertar(arbol, cosita1);
    arbol_insertar(arbol, cosita7);
    arbol_insertar(arbol, cosita6);
    arbol_insertar(arbol, cosita5);
    arbol_insertar(arbol, cosita4);
    arbol_insertar(arbol, cosita0);
    arbol_insertar(arbol, cosita2coma5);
    arbol_insertar(arbol, cosita6coma5);

    arbol_recorrido_inorden(arbol, cositas_recorridas, 10);
    bool guardo_en_orden = true;
    for(int i = 0; i < 10 && guardo_en_orden; i++){
        guardo_en_orden = ((cosita_t*)cositas_recorridas[i])->clave == claves_esperadas[i];
    }
    pa2m_afirmar(guardo_en_orden, "Los elementos se insertan como corresponde");


    arbol_destruir(arbol);
    
}

void probar_borrar(){
    pa2m_nuevo_grupo("BORRAR");

    char claves[MAX_CLAVES] = "2,1,0,3,2.5,5,9,10,4,13,14,15,12,11";
    abb_t* arbol = crear_arbol_con_elementos(claves);
    if(!arbol)
        return;

    float claves_esperadas[MAX_CLAVES] = {0,2.5,3,3.5,10,11,12,13,14,15};
    void* cositas_recorridas[MAX_CLAVES];

    cosita_t* auxiliar = crear_cosita(0);

    auxiliar->clave = 7000;
    pa2m_afirmar(arbol_borrar(NULL, auxiliar) == ERROR, "No se puede borrar en un arbol nulo");
    abb_t* arbol_vacio = arbol_crear(comparar_cositas, NULL);
    pa2m_afirmar(arbol_borrar(arbol_vacio, auxiliar) == ERROR, "No se puede borrar en un arbol vacio");
    pa2m_afirmar(arbol_borrar(arbol, auxiliar) == ERROR, "No se puede borrar un elemento que no está en el arbol");

    auxiliar->clave = 4;
    pa2m_afirmar(arbol_borrar(arbol, auxiliar) == EXITO, "borrar un elemento (4) hoja del arbol");
    pa2m_afirmar(arbol_buscar(arbol, auxiliar) == NULL, "El elemento hoja ya no se encuentra en el arbol");

    auxiliar->clave = 1;
    pa2m_afirmar(arbol_borrar(arbol, auxiliar) == EXITO, "borrar elemento (1) con hijos de rama izquierda");
    pa2m_afirmar(arbol_buscar(arbol, auxiliar) == NULL, "El elemento ya no se encuentra en el arbol");

    auxiliar->clave = 9;
    pa2m_afirmar(arbol_borrar(arbol, auxiliar) == EXITO, "borrar elemento (9) con hijos de rama derecha");
    pa2m_afirmar(arbol_buscar(arbol, auxiliar) == NULL, "El elemento ya no se encuentra en el arbol");

    auxiliar->clave = 2;
    pa2m_afirmar(arbol_borrar(arbol, auxiliar) == EXITO, "borrar elemento (2) raiz");
    pa2m_afirmar(arbol_buscar(arbol, auxiliar) == NULL, "El elemento ya no se encuentra en el arbol");

     cosita_t* aux_insertar = crear_cosita(3.5);
     arbol_insertar(arbol, aux_insertar);

    cosita_t* aux_borrar = crear_cosita(5);
    pa2m_afirmar(arbol_borrar(arbol, aux_borrar) == EXITO, "borrar elemento (5) con hijos en ambas ramas");
    pa2m_afirmar(arbol_buscar(arbol, aux_borrar) == NULL, "El elemento ya no se encuentra en el arbol");


    arbol_recorrido_inorden(arbol, cositas_recorridas, 10);
    bool borro_bien_los_elementos = true;
    for(int i = 0; i < 10 && borro_bien_los_elementos; i++){
        borro_bien_los_elementos = ((cosita_t*)cositas_recorridas[i])->clave == claves_esperadas[i];
    }
    pa2m_afirmar(borro_bien_los_elementos, "El arbol quedó con la estructura correcta despues de el borrado");

    destruir_cosita(aux_borrar);
    destruir_cosita(auxiliar);
    arbol_destruir(arbol);
    arbol_destruir(arbol_vacio);
}

void probar_buscar(){
    pa2m_nuevo_grupo("BUSCAR");
    char claves[MAX_CLAVES] = "2,3,1,4";
    abb_t* arbol = crear_arbol_con_elementos(claves);
    if(!arbol)
        return;

    cosita_t* auxiliar = crear_cosita(0);

    pa2m_afirmar(arbol_buscar(NULL, auxiliar) == NULL, "No se encuentra un elemento en un arbol nulo");

    abb_t* arbol_vacio = arbol_crear(comparar_cositas, destruir_cosita);
    pa2m_afirmar(arbol_buscar(arbol_vacio, auxiliar) == NULL, "No se encuentra un elemento en el arbol vacío");

    auxiliar->clave = 2;
    pa2m_afirmar(((cosita_t*)arbol_buscar(arbol, auxiliar))->clave == auxiliar->clave, "Se encuentra el elemento raiz elemento");

     auxiliar->clave = 3;
    pa2m_afirmar(((cosita_t*)arbol_buscar(arbol, auxiliar))->clave == auxiliar->clave, "Se encuentra el segundo elemento");

     auxiliar->clave = 1;
    pa2m_afirmar(((cosita_t*)arbol_buscar(arbol, auxiliar))->clave == auxiliar->clave, "Se encuentra el tercer elemento");

     auxiliar->clave = 4;
    pa2m_afirmar(((cosita_t*)arbol_buscar(arbol, auxiliar))->clave == auxiliar->clave, "Se encuentra el cuarto elemento");

    auxiliar->clave = 5;
    pa2m_afirmar(((cosita_t*)arbol_buscar(arbol, auxiliar)) == NULL, "No se encuentra un elemento que no se insertó");

    destruir_cosita(auxiliar);
    arbol_destruir(arbol_vacio);
    arbol_destruir(arbol);
    
}

void probar_recorrido_inorden(){
    pa2m_nuevo_grupo("INORDEN");

    char claves[MAX_CLAVES] = "2,1,3,5,9,10,4,8,6,7";
    float claves_esperadas[MAX_CLAVES] = {1,2,3,4,5,6,7,8,9,10};

    abb_t* arbol = crear_arbol_con_elementos(claves);
    if(!arbol)
        return;

    void* cositas[10];

    pa2m_afirmar(arbol_recorrido_inorden(NULL, cositas, 10) == 0, "La cantidad de elementos recorridos en un arbol nulo es 0");
    pa2m_afirmar(arbol_recorrido_inorden(arbol, NULL, 10) == 0, "Dada una lista nula, no se recorre el arbol");
    pa2m_afirmar(arbol_recorrido_inorden(arbol, NULL, 0) == 0, "Si la cantidad de elementos para guardar es 0 no se recorre el arbol");


    pa2m_afirmar(arbol_recorrido_inorden(arbol, cositas, 10) == 10, "Se recorren 10 elementos");

    bool guardo_en_orden = true;
    for(int i = 0; i < 10 && guardo_en_orden; i++){
        guardo_en_orden = ((cosita_t*)cositas[i])->clave == claves_esperadas[i];
    }
    pa2m_afirmar(guardo_en_orden, "guarda en inorden los 10 elementos");

    arbol_destruir(arbol);
    
}

void probar_recorrido_preorden(){
    pa2m_nuevo_grupo("PREORDEN");

    char claves[MAX_CLAVES] = "2,1,3,5,9,10,4,8,6,7";
    float claves_esperadas[MAX_CLAVES] = {2,1,3,5,4,9,8,6,7,10};
    abb_t* arbol = crear_arbol_con_elementos(claves);
    if(!arbol)
        return;

    void* cositas[10];


    pa2m_afirmar(arbol_recorrido_preorden(NULL, cositas, 10) == 0, "La cantidad de elementos recorridos en un arbol nulo es 0");
    pa2m_afirmar(arbol_recorrido_preorden(arbol, NULL, 10) == 0, "Dada una lista nula, no se recorre el arbol");
    pa2m_afirmar(arbol_recorrido_preorden(arbol, NULL, 0) == 0, "Si la cantidad de elementos para guardar es 0 no se recorre el arbol");


    pa2m_afirmar(arbol_recorrido_preorden(arbol, cositas, 10) == 10, "Se recorren 10 elementos");

    bool guardo_en_orden = true;
    for(int i = 0; i < 10 && guardo_en_orden; i++){
        guardo_en_orden = ((cosita_t*)cositas[i])->clave == claves_esperadas[i];
    }
    pa2m_afirmar(guardo_en_orden, "guarda en inorden los 10 elementos");
    arbol_destruir(arbol);
}

void probar_recorrido_postorden(){
    pa2m_nuevo_grupo("POSTORDEN");
    
    char claves[MAX_CLAVES] = "2,1,3,5,9,10,4,8,6,7";
    float claves_esperadas[MAX_CLAVES] = {1,4,7,6,8,10,9,5,3,2};

    abb_t* arbol = crear_arbol_con_elementos(claves);
    if(!arbol)
        return;

    void* cositas[10];

    pa2m_afirmar(arbol_recorrido_postorden(NULL, cositas, 10) == 0, "La cantidad de elementos recorridos en un arbol nulo es 0");
    pa2m_afirmar(arbol_recorrido_postorden(arbol, NULL, 10) == 0, "Dada una lista nula, no se recorre el arbol");
    pa2m_afirmar(arbol_recorrido_postorden(arbol, NULL, 0) == 0, "Si la cantidad de elementos para guardar es 0 no se recorre el arbol");


    pa2m_afirmar(arbol_recorrido_postorden(arbol, cositas, 10) == 10, "Se recorren 10 elementos");

    bool guardo_en_orden = true;
    for(int i = 0; i < 10 && guardo_en_orden; i++){
        guardo_en_orden = ((cosita_t*)cositas[i])->clave == claves_esperadas[i];
    }
    pa2m_afirmar(guardo_en_orden, "guarda en postorden los 10 elementos");
    arbol_destruir(arbol);
}

void probar_obtener_raiz(){
    pa2m_nuevo_grupo("OBTENER RAIZ");

    pa2m_afirmar(arbol_raiz(NULL) == NULL, "arbol nulo no tiene raíz");

    abb_t* arbol = arbol_crear(comparar_cositas, destruir_cosita);
    pa2m_afirmar(arbol_raiz(arbol) == NULL, "arbol vacío no tiene raíz");

    cosita_t* cosita = crear_cosita(5);
    arbol_insertar(arbol, cosita);
    pa2m_afirmar(((cosita_t*)arbol_raiz(arbol)) == cosita, "arbol devuelve el elemento raíz");

    arbol_destruir(arbol);
} 

void probar_esta_vacio(){
    pa2m_nuevo_grupo("ABB ESTÁ VACÍO");

    pa2m_afirmar(arbol_vacio(NULL) == true, "arbol nulo está vacío");

    abb_t* arbol = arbol_crear(comparar_cositas, destruir_cosita);
    pa2m_afirmar(arbol_vacio(arbol) == true, "arbol sin raíz está vacío");

    cosita_t* cosita = crear_cosita(5);
    arbol_insertar(arbol, cosita);
    pa2m_afirmar(arbol_vacio(arbol) == false, "arbol con raíz no está vacío");

    arbol_destruir(arbol);
}

bool recorrer_hasta_5_imprimir_nodo(void* elemento, void* contador){
    (*(int*)contador)++;
    printf("elemento nº %i tiene clave: %f\n", *(int*)contador,  ((cosita_t*)elemento)->clave);

    if(*(int*)contador >= 5)
        return true;
    return false;
}

void probar_iterador_interno(){
    pa2m_nuevo_grupo("ITERADOR INTERNO");

    
    char claves[MAX_CLAVES] = "2,1,3,5,9,10,4,8,6,7";
    abb_t* arbol = crear_arbol_con_elementos(claves);
    if(!arbol)
        return;

    abb_t* arbol_vacio = arbol_crear(comparar_cositas, destruir_cosita);
    int contador = 0;

    pa2m_afirmar(
        abb_con_cada_elemento(NULL, ABB_RECORRER_INORDEN, recorrer_hasta_5_imprimir_nodo, (void*)&contador) == 0 &&
        contador == 0,
        "Dado un arbol nulo, se recorren 0 elementos");
    
    pa2m_afirmar(
        abb_con_cada_elemento(arbol_vacio, 999, recorrer_hasta_5_imprimir_nodo, (void*)&contador) == 0 &&
        contador == 0,
        "Dado un recorrido no válido, se recorren 0 elementos");

    pa2m_afirmar(
        abb_con_cada_elemento(arbol_vacio, ABB_RECORRER_POSTORDEN, NULL, (void*)&contador) == 0 &&
        contador == 0,
        "Dado una función nula, se recorren 0 elementos");

    
    pa2m_afirmar(
        abb_con_cada_elemento(arbol_vacio, ABB_RECORRER_PREORDEN, recorrer_hasta_5_imprimir_nodo, (void*)&contador) == 0 &&
        contador == 0,
        "Dado un arbol vacío, se recorren 0 elementos");


    pa2m_nuevo_grupo("ITERADOR INTERNO - INORDEN");
    size_t elementos_recorridos = abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, recorrer_hasta_5_imprimir_nodo, (void*)&contador);

    pa2m_afirmar(elementos_recorridos == 5, "Se recorrieron 5 elementos");

    pa2m_nuevo_grupo("ITERADOR INTERNO - PREORDEN");
    contador = 0;
    elementos_recorridos = abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, recorrer_hasta_5_imprimir_nodo, (void*)&contador);
    pa2m_afirmar(elementos_recorridos == 5, "Se recorrieron 5 elementos");

    pa2m_nuevo_grupo("ITERADOR INTERNO - POSTORDEN");
    contador = 0;
    elementos_recorridos = abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, recorrer_hasta_5_imprimir_nodo, (void*)&contador);
    pa2m_afirmar(elementos_recorridos == 5, "Se recorrieron 5 elementos");

    arbol_destruir(arbol_vacio);
    arbol_destruir(arbol);
    
}


int main() {
    probar_creacion();
    probar_insertar();
    probar_borrar();
    probar_buscar();
    probar_recorrido_inorden();
    probar_recorrido_preorden();
    probar_recorrido_postorden();
    probar_iterador_interno();
    probar_obtener_raiz();
    pa2m_mostrar_reporte();
    return 0;
}