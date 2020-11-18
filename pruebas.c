#include "abb.h"
#include "pa2mm.h"

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
    free(cosita);
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

abb_t* crear_arbol_con_claves(float* claves, int cantidad){
  abb_t* arbol = arbol_crear(comparar_cositas, destruir_cosita);
  for(int i = 0; i < cantidad; i++){
    cosita_t* cosita = crear_cosita(claves[i]);
    arbol_insertar(arbol, cosita);
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

    pa2m_afirmar(arbol_crear(NULL, NULL) == NULL, "No se puede crear crear un abb sin comparador");

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

    abb_t* arbol = arbol_crear(comparar_cositas, NULL);
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

void probar_buscar(){
    pa2m_nuevo_grupo("BUSCAR");
    cosita_t* cosita1 = crear_cosita(1);
    cosita_t* cosita2 = crear_cosita(2);
    cosita_t* cosita3 = crear_cosita(3);
    cosita_t* cosita4 = crear_cosita(4);
    cosita_t* cosita5 = crear_cosita(5);
    /*cosita_t* cosita6 = crear_cosita(6);
    cosita_t* cosita7 = crear_cosita(7);*/

    pa2m_afirmar(arbol_buscar(NULL, cosita1) == NULL, "No se encuentra un elemento en un arbol nulo");

    abb_t* arbol = arbol_crear(comparar_cositas, NULL);
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

    arbol_destruir(arbol);
    
}

void probar_recorrido_inorden(){
    pa2m_nuevo_grupo("INORDEN");
   // cosita_t* cosita1 = crear_cosita(1);
    cosita_t* cosita2 = crear_cosita(2);
    //cosita_t* cosita3 = crear_cosita(3);
   // cosita_t* cosita4 = crear_cosita(4);
    //cosita_t* cosita5 = crear_cosita(5);

    abb_t* arbol = arbol_crear(comparar_cositas, NULL);
    arbol_insertar(arbol, cosita2);

    void* cositas[1];

    size_t cantidad_llenada = arbol_recorrido_inorden(arbol, cositas, 1);
    printf("cantida llenada: %zu\n", cantidad_llenada);
    printf("clave: %f\n", ((cosita_t*)cositas[0])->clave);
    /*pa2m_afirmar(arbol_buscar(NULL, cosita1) == NULL, "No se encuentra un elemento en un arbol nulo");

    pa2m_afirmar(arbol_buscar(arbol, cosita1) == NULL, "No se encuentra un elemento en el arbol vacío");

    pa2m_afirmar(arbol_buscar(arbol, cosita2) == cosita2, "Se encuentra el único elemento");

    arbol_insertar(arbol, cosita3);
    pa2m_afirmar(arbol_buscar(arbol, cosita3) == cosita3, "Se encuentra el segundo elemento");

    arbol_insertar(arbol, cosita1);
    pa2m_afirmar(arbol_buscar(arbol, cosita1) == cosita1, "Se encuentra el tercer elemento");

    arbol_insertar(arbol, cosita4);
    pa2m_afirmar(arbol_buscar(arbol, cosita4) == cosita4, "Se encuentra el cuarto elemento");

    pa2m_afirmar(arbol_buscar(arbol, cosita5) == NULL, "No se encuentra un elemento que no se insertó");
*/
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
    probar_buscar();
    probar_recorrido_inorden();
    probar_obtener_raiz();
    pa2m_mostrar_reporte();
    return 0;
}