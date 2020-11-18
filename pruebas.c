#include "abb.h"
#include "pa2mm.h"

#define EXITO 0
#define ERROR 1

#define PRIMER_ELEMENTO_MAYOR 1
#define IGUALES 0
#define SEGUNDO_ELEMENTO_MAYOR -1


typedef struct cosita{
    int clave;
    char contenido[10];
}cosita_t;

cosita_t* crear_cosita(int clave){
    cosita_t* cosita = (cosita_t*)malloc(sizeof(cosita));
    if(cosita)
        cosita->clave = clave;
    return cosita;
}

void destruir_cosita(cosita_t* c){
    free(c);
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
 * Destructor de elementos. Cada vez que un elemento deja el arbol
 * (arbol_borrar o arbol_destruir) se invoca al destructor pasandole
 * el elemento.
 */
void abb_liberar_numero(void* elemento){

}

void probar_creacion(){
    pa2m_nuevo_grupo("CREACIÓN");

    pa2m_afirmar(arbol_crear(NULL, NULL) == NULL, "No se puede crear crear un abb sin comparador");

    abb_t* arbol = arbol_crear(comparar_cositas, NULL);
    pa2m_afirmar(arbol->comparador == comparar_cositas, "El comparador que utiliza el abb es el pasado por parametros");
    pa2m_afirmar(arbol->destructor == NULL, "Se puede crear un abb sin destructor");
    pa2m_afirmar(arbol->nodo_raiz == NULL, "El nodo raíz de un abb recién inicializado es nulo");

    arbol_destruir(arbol);
} 

void probar_insertar(){
    pa2m_nuevo_grupo("INSERTAR");
    cosita_t* cosita1 = crear_cosita(1);
    cosita_t* cosita2 = crear_cosita(20);
    cosita_t* cosita3 = crear_cosita(-5);
    cosita_t* cosita4 = crear_cosita(3);

    pa2m_afirmar(arbol_insertar(NULL, NULL) == ERROR, "No se puede insertar en un arbol nulo");

    abb_t* arbol = arbol_crear(comparar_cositas, NULL);
    pa2m_afirmar(arbol_insertar(arbol, cosita1) == EXITO, "Primer elemento insertado devuelve EXITO");
    pa2m_afirmar(arbol->nodo_raiz->elemento == cosita1, "Al insertar un elemento en un árbol vacío queda como el elemento raíz");

    pa2m_afirmar(arbol_insertar(arbol, cosita2) == EXITO, "Segundo elemento insertado devuelve EXITO");
    pa2m_afirmar(arbol->nodo_raiz->derecha->elemento == cosita2, "Segundo elemento (de mayor clave) se inserta a la derecha");

    pa2m_afirmar(arbol_insertar(arbol, cosita3) == EXITO, "Tercer elemento insertado devuelve EXITO");
    pa2m_afirmar(arbol->nodo_raiz->izquierda->elemento == cosita3, "Tercer elemento (de menor clave) se inserta a la izquierda");

    pa2m_afirmar(arbol_insertar(arbol, cosita4) == EXITO, "Cuarto elemento insertado devuelve EXITO");
    pa2m_afirmar(arbol->nodo_raiz->derecha->izquierda->elemento == cosita4, "Cuarto elemento se inserta donde corresponde");

    arbol_destruir(arbol);
    
}

void probar_obtener_raiz(){
    pa2m_nuevo_grupo("OBTENER RAIZ");

    pa2m_afirmar(arbol_raiz(NULL) == NULL, "arbol nulo no tiene raíz");

    abb_t* arbol = arbol_crear(comparar_cositas, NULL);
    pa2m_afirmar(arbol_raiz(arbol) == NULL, "arbol vacío no tiene raíz");

    /*Falta probar con un arbol completo*/

    arbol_destruir(arbol);
} 

void probar_esta_vacio(){
    pa2m_nuevo_grupo("ABB ESTÁ VACÍO");

    pa2m_afirmar(arbol_vacio(NULL) == true, "arbol nulo está vacío");

    abb_t* arbol = arbol_crear(comparar_cositas, NULL);
    pa2m_afirmar(arbol_vacio(arbol) == true, "arbol sin raíz está vacío");

    /*Falta probar con un nodo completo*/

    arbol_destruir(arbol);
} 


int main() {
    probar_creacion();
    probar_insertar();
    probar_obtener_raiz();
    pa2m_mostrar_reporte();
    return 0;
}