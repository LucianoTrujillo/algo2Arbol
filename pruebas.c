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


/*
 * Destructor de elementos. Cada vez que un elemento deja el arbol
 * (arbol_borrar o arbol_destruir) se invoca al destructor pasandole
 * el elemento.
 */
void abb_liberar_numero(void* elemento){

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

    pa2m_afirmar(arbol_insertar(NULL, NULL) == ERROR, "No se puede insertar en un arbol nulo");

    abb_t* arbol = arbol_crear(comparar_cositas, destruir_cosita);
    pa2m_afirmar(arbol_insertar(arbol, cosita2) == EXITO, "Primer elemento insertado devuelve EXITO");
    pa2m_afirmar(arbol->nodo_raiz->elemento == cosita2, "Al insertar un elemento en un árbol vacío queda como el elemento raíz");

    pa2m_afirmar(arbol_insertar(arbol, cosita3) == EXITO, "Segundo elemento insertado devuelve EXITO");
    pa2m_afirmar(arbol->nodo_raiz->derecha->elemento == cosita3, "Segundo elemento (de mayor clave) se inserta a la derecha");

    pa2m_afirmar(arbol_insertar(arbol, cosita1) == EXITO, "Tercer elemento insertado devuelve EXITO");
    pa2m_afirmar(arbol->nodo_raiz->izquierda->elemento == cosita1, "Tercer elemento (de menor clave) se inserta a la izquierda");

    pa2m_afirmar(arbol_insertar(arbol, cosita7) == EXITO, "Cuarto elemento insertado devuelve EXITO");
    pa2m_afirmar(arbol->nodo_raiz->derecha->derecha->elemento == cosita7, "Cuarto elemento se inserta donde corresponde");

    pa2m_afirmar(arbol_insertar(arbol, cosita6) == EXITO, "Quinto elemento insertado devuelve EXITO");
    pa2m_afirmar(arbol->nodo_raiz->derecha->derecha->izquierda->elemento == cosita6, "Quinto elemento se inserta donde corresponde");

    pa2m_afirmar(arbol_insertar(arbol, cosita5) == EXITO, "Sexto elemento insertado devuelve EXITO");
    pa2m_afirmar(arbol->nodo_raiz->derecha->derecha->izquierda->izquierda->elemento == cosita5, "Sexto elemento se inserta donde corresponde");

    pa2m_afirmar(arbol_insertar(arbol, cosita4) == EXITO, "Septimo elemento insertado devuelve EXITO");
    pa2m_afirmar(arbol->nodo_raiz->derecha->derecha->izquierda->izquierda->izquierda->elemento == cosita4, "Septimo elemento se inserta donde corresponde");

    pa2m_afirmar(arbol_insertar(arbol, cosita0) == EXITO, "Octavo elemento insertado devuelve EXITO");
    pa2m_afirmar(arbol->nodo_raiz->izquierda->izquierda->elemento == cosita0, "Octavo elemento se inserta dodne corresponde");

      pa2m_afirmar(arbol_insertar(arbol, cosita2coma5) == EXITO, "Noveno elemento insertado devuelve EXITO");
    pa2m_afirmar(arbol->nodo_raiz->derecha->izquierda->elemento == cosita2coma5, "Noveno elemento se inserta dodne corresponde");

    pa2m_afirmar(arbol_insertar(arbol, cosita6coma5) == EXITO, "Decimo elemento insertado devuelve EXITO");
    pa2m_afirmar(arbol->nodo_raiz->derecha->derecha->izquierda->derecha->elemento == cosita6coma5, "Decimo elemento se inserta dodne corresponde");

    arbol_destruir(arbol);
    
}

void probar_borrar(){
    pa2m_nuevo_grupo("BORRAR");
    cosita_t* cosita1 = crear_cosita(1);
    cosita_t* cosita2 = crear_cosita(2);
    cosita_t* cosita3 = crear_cosita(3);
    cosita_t* cosita4 = crear_cosita(4);
    cosita_t* cosita5 = crear_cosita(5);
    cosita_t* cosita6 = crear_cosita(6);
    cosita_t* cosita7 = crear_cosita(7);
    cosita_t* cosita8 = crear_cosita(8);
    cosita_t* cosita9 = crear_cosita(9);
    cosita_t* cosita10 = crear_cosita(10);
    cosita_t* cosita11 = crear_cosita(11);
    cosita_t* cosita12 = crear_cosita(12);
    cosita_t* cosita13 = crear_cosita(13);
    cosita_t* cosita14 = crear_cosita(14);
    cosita_t* cosita15 = crear_cosita(15);

    abb_t* arbol = arbol_crear(comparar_cositas, destruir_cosita);
    arbol_insertar(arbol, cosita2);
    arbol_insertar(arbol, cosita1);
    arbol_insertar(arbol, cosita3);
    arbol_insertar(arbol, cosita5);
    arbol_insertar(arbol, cosita9);
    arbol_insertar(arbol, cosita10);
    arbol_insertar(arbol, cosita4);
    arbol_insertar(arbol, cosita8);
    arbol_insertar(arbol, cosita6);
    arbol_insertar(arbol, cosita7);
    arbol_insertar(arbol, cosita13);
    arbol_insertar(arbol, cosita14);
    arbol_insertar(arbol, cosita15);
    arbol_insertar(arbol, cosita12);
    arbol_insertar(arbol, cosita11);

    cosita_t* auxiliar = crear_cosita(0);

    auxiliar->clave = 1;
    pa2m_afirmar(arbol_borrar(arbol, cosita1) == EXITO, "borrar elemento hoja devuelve EXITO");
    pa2m_afirmar(arbol_buscar(arbol, auxiliar) == NULL, "El elemento ya no se encuentra en el arbol");


    auxiliar->clave = 4;
    pa2m_afirmar(arbol_borrar(arbol, cosita4) == EXITO, "borrar otro elemento hoja devuelve EXITO");
    pa2m_afirmar(arbol_buscar(arbol, auxiliar) == NULL, "El elemento ya no se encuentra en el arbol");

    auxiliar->clave = 14;
    pa2m_afirmar(arbol_borrar(arbol, cosita14) == EXITO, "borrar elemento con hijos de rama derecha devuelve EXITO");
    pa2m_afirmar(arbol_buscar(arbol, auxiliar) == NULL, "El elemento ya no se encuentra en el arbol");

     auxiliar->clave = 12;
    pa2m_afirmar(arbol_borrar(arbol, cosita12) == EXITO, "borrar elemento con hijos de rama izquierda devuelve EXITO");
    pa2m_afirmar(arbol_buscar(arbol, auxiliar) == NULL, "El elemento ya no se encuentra en el arbol");


    destruir_cosita(auxiliar);
    arbol_destruir(arbol);
}

void probar_buscar(){
    pa2m_nuevo_grupo("BUSCAR");
    cosita_t* cosita1 = crear_cosita(1);
    cosita_t* cosita2 = crear_cosita(2);
    cosita_t* cosita3 = crear_cosita(3);
    cosita_t* cosita4 = crear_cosita(4);
    cosita_t* cosita5 = crear_cosita(5);

    pa2m_afirmar(arbol_buscar(NULL, cosita1) == NULL, "No se encuentra un elemento en un arbol nulo");

    abb_t* arbol = arbol_crear(comparar_cositas, destruir_cosita);
    pa2m_afirmar(arbol_buscar(arbol, cosita1) == NULL, "No se encuentra un elemento en el arbol vacío");

    arbol_insertar(arbol, cosita2);
    pa2m_afirmar(arbol_buscar(arbol, cosita2) == cosita2, "Se encuentra el único elemento");

    arbol_insertar(arbol, cosita3);
    pa2m_afirmar(arbol_buscar(arbol, cosita3) == cosita3, "Se encuentra el segundo elemento");

    arbol_insertar(arbol, cosita1);
    pa2m_afirmar(arbol_buscar(arbol, cosita1) == cosita1, "Se encuentra el tercer elemento");

    arbol_insertar(arbol, cosita4);
    pa2m_afirmar(arbol_buscar(arbol, cosita4) == cosita4, "Se encuentra el cuarto elemento");

    pa2m_afirmar(arbol_buscar(arbol, cosita5) == NULL, "No se encuentra un elemento que no se insertó");

    destruir_cosita(cosita5);
    arbol_destruir(arbol);
    
}

void probar_recorrido_inorden(){
    pa2m_nuevo_grupo("INORDEN");

    char claves[MAX_CLAVES] = "2,1,3,5,9,10,4,8,6,7";
    abb_t* arbol = crear_arbol_con_elementos(claves);
    if(!arbol)
        return;

    void* cositas[10];

    pa2m_afirmar(arbol_recorrido_inorden(NULL, cositas, 10) == 0, "La cantidad de elementos recorriods en un arbol nulo es 0");
    pa2m_afirmar(arbol_recorrido_inorden(arbol, NULL, 10) == 0, "Dada una lista nula, no se recorre el arbol");
    pa2m_afirmar(arbol_recorrido_inorden(arbol, NULL, 0) == 0, "Si la cantidad de elementos para guardar es 0 no se recorre el arbol");


    pa2m_afirmar(arbol_recorrido_inorden(arbol, cositas, 10) == 10, "Se recorren 10 elementos");

    bool guardo_en_orden = true;
    for(int i = 0; i < 10 && guardo_en_orden; i++){
        guardo_en_orden = ((cosita_t*)cositas[i])->clave == i + 1;
    }
    pa2m_afirmar(guardo_en_orden, "guarda en inorden los 10 elementos");

    arbol_destruir(arbol);
    
}

void probar_recorrido_preorden(){
    pa2m_nuevo_grupo("PREORDEN");

    char claves[MAX_CLAVES] = "2,1,3,5,9,10,4,8,6,7";
    abb_t* arbol = crear_arbol_con_elementos(claves);
    if(!arbol)
        return;

    void* cositas[10];


    pa2m_afirmar(arbol_recorrido_preorden(NULL, cositas, 10) == 0, "La cantidad de elementos recorriods en un arbol nulo es 0");
    pa2m_afirmar(arbol_recorrido_preorden(arbol, NULL, 10) == 0, "Dada una lista nula, no se recorre el arbol");
    pa2m_afirmar(arbol_recorrido_preorden(arbol, NULL, 0) == 0, "Si la cantidad de elementos para guardar es 0 no se recorre el arbol");


    pa2m_afirmar(arbol_recorrido_preorden(arbol, cositas, 10) == 10, "Se recorren 10 elementos");

    pa2m_afirmar(((cosita_t*)cositas[0])->clave ==  2, "guarda en preorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[1])->clave ==  1, "guarda en preorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[2])->clave ==  3, "guarda en preorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[3])->clave ==  5, "guarda en preorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[4])->clave ==  4, "guarda en preorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[5])->clave ==  9, "guarda en preorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[6])->clave ==  8, "guarda en preorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[7])->clave ==  6, "guarda en preorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[8])->clave ==  7, "guarda en preorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[9])->clave ==  10, "guarda en preorden los 10 elementos");

    arbol_destruir(arbol);
}

void probar_recorrido_postorden(){
    pa2m_nuevo_grupo("POSTORDEN");
    
    char claves[MAX_CLAVES] = "2,1,3,5,9,10,4,8,6,7";
    abb_t* arbol = crear_arbol_con_elementos(claves);
    if(!arbol)
        return;

    void* cositas[10];

    pa2m_afirmar(arbol_recorrido_postorden(NULL, cositas, 10) == 0, "La cantidad de elementos recorriods en un arbol nulo es 0");
    pa2m_afirmar(arbol_recorrido_postorden(arbol, NULL, 10) == 0, "Dada una lista nula, no se recorre el arbol");
    pa2m_afirmar(arbol_recorrido_postorden(arbol, NULL, 0) == 0, "Si la cantidad de elementos para guardar es 0 no se recorre el arbol");


    pa2m_afirmar(arbol_recorrido_postorden(arbol, cositas, 10) == 10, "Se recorren 10 elementos");

    pa2m_afirmar(((cosita_t*)cositas[0])->clave ==  1, "guarda en postorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[1])->clave ==  4, "guarda en postorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[2])->clave ==  7, "guarda en postorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[3])->clave ==  6, "guarda en postorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[4])->clave ==  8, "guarda en postorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[5])->clave ==  10, "guarda en postorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[6])->clave ==  9, "guarda en postorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[7])->clave ==  5, "guarda en postorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[8])->clave ==  3, "guarda en postorden los 10 elementos");
    pa2m_afirmar(((cosita_t*)cositas[9])->clave ==  2, "guarda en postorden los 10 elementos");

    arbol_destruir(arbol);
}

void probar_obtener_raiz(){
    pa2m_nuevo_grupo("OBTENER RAIZ");

    pa2m_afirmar(arbol_raiz(NULL) == NULL, "arbol nulo no tiene raíz");

    abb_t* arbol = arbol_crear(comparar_cositas, destruir_cosita);
    pa2m_afirmar(arbol_raiz(arbol) == NULL, "arbol vacío no tiene raíz");

    /*Falta probar con un arbol completo*/

    arbol_destruir(arbol);
} 

void probar_esta_vacio(){
    pa2m_nuevo_grupo("ABB ESTÁ VACÍO");

    pa2m_afirmar(arbol_vacio(NULL) == true, "arbol nulo está vacío");

    abb_t* arbol = arbol_crear(comparar_cositas, destruir_cosita);
    pa2m_afirmar(arbol_vacio(arbol) == true, "arbol sin raíz está vacío");

    /*Falta probar con un nodo completo*/

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

    cosita_t* cosita1 = crear_cosita(1);
    cosita_t* cosita2 = crear_cosita(2);
    cosita_t* cosita3 = crear_cosita(3);
    cosita_t* cosita4 = crear_cosita(4);
    cosita_t* cosita5 = crear_cosita(5);
    cosita_t* cosita6 = crear_cosita(6);
    cosita_t* cosita7 = crear_cosita(7);
    cosita_t* cosita8 = crear_cosita(8);
    cosita_t* cosita9 = crear_cosita(9);
    cosita_t* cosita10 = crear_cosita(10);

    abb_t* arbol = arbol_crear(comparar_cositas, destruir_cosita);
    int contador = 0;

    pa2m_afirmar(
        abb_con_cada_elemento(NULL, ABB_RECORRER_INORDEN, recorrer_hasta_5_imprimir_nodo, (void*)&contador) == 0 &&
        contador == 0,
        "Dado un arbol nulo, se recorren 0 elementos");
    
    pa2m_afirmar(
        abb_con_cada_elemento(arbol, 999, recorrer_hasta_5_imprimir_nodo, (void*)&contador) == 0 &&
        contador == 0,
        "Dado un recorrido no válido, se recorren 0 elementos");

    pa2m_afirmar(
        abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, NULL, (void*)&contador) == 0 &&
        contador == 0,
        "Dado una función nula, se recorren 0 elementos");

    
    pa2m_afirmar(
        abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, recorrer_hasta_5_imprimir_nodo, (void*)&contador) == 0 &&
        contador == 0,
        "Dado un arbol vacío, se recorren 0 elementos");


    arbol_insertar(arbol, cosita2);
    arbol_insertar(arbol, cosita1);
    arbol_insertar(arbol, cosita3);
    arbol_insertar(arbol, cosita5);
    arbol_insertar(arbol, cosita9);
    arbol_insertar(arbol, cosita10);
    arbol_insertar(arbol, cosita4);
    arbol_insertar(arbol, cosita8);
    arbol_insertar(arbol, cosita6);
    arbol_insertar(arbol, cosita7);

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