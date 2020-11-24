# TDA Árbol

[![N|Solid](https://yourbasic.org/algorithms/tree.png)](https://nodesource.com/products/nsolid)

# El programa

### ¿Qúe es?

Es una implementación del tipo de dato abstracto "Arbol binario de búsqueda", con diferentes primitivas que permiten utilizarlo con cualquier tipo de dato que sea comparable. La lista está implementada con nodos en memoria dinámica.

### Compilación

Para compilarlo se puede hacer uso del siguiente comando dentro de la carpeta del proyecto.
```
$ make abb
```

### Ejecución

La ejecución es muy sencilla, solo basta con el siguiente comando para correr todas las pruebas unitarias del TDA.
```
$ ./abb
```

### Tests de memoria

El test de memoria con Valgrind se corre de la siguiente manera.
```
$ make mem_test
```

Para mas comandos visitar el Makefile.

### Funcionamiento general de la implementación

El TDA viene con las operaciones básicos que se esperan de un árbol, las cuales se mencionan más abajo. Al utiliza nodos enlazados, maneja de manera eficiente la cantidad de memoria que ocupa en el Heap, ya que solo usa lo justo y necesario. Para hacer uso del árbol, basta con crearlo, utilizarlo y destruirlo.

Cada operación del TDA se encuentra testeada con pruebas unitarias exhaustivas que se realizan al ejecutar el programa.
Las operaciones permitidas se encuentran especificadas en el archivo abb.h.

# Conceptos teóricos

## Árbol Binario

En la informática, árbol binario es una estructura de datos en la cual cada nodo puede tener un hijo izquierdo y un hijo derecho, y cuenta con ciertas operaciones básicas para insertar, remover, buscar datos. Para cada nodo, hay un único camino el cual lleva hacia él. Tiene la gran desventaja de no tener ordenados a sus elementos ya que no cuenta con un método de comparación al insertarlos o buscarlos, con lo cual al buscar un elemento del árbol, solo se puede recorrerlo enteramente hasta encontrarlo. Esto reduce la eficiencia del algoritmo de búsqueda. 

## Árbol Binario de Búsqueda

A diferencia de los arboles binarios desordenados(o simplemente arboles binarios),  el arbol binario de busqueda agregan dos condicion importantes: 
   - Se necesita de una condición de comparación que dado dos elementos, pueda decidir cual es mayor, menor o igual. 
   - Todo elemento de la rama izquierda de cualquier elemento será menor a todos los elementos de la rama derecha.

Esto permite mantener al arbol ordenado siempre, lo cual genera en la mayoría de los casos, una mejora en la búsqueda de elementos. De todas maneras, dado que no está balanceado, puede llegar a pasar que el peso en una rama sea mucho mayor a la otra, con lo cual los tiempos de búsqueda no se reducirían en gran cantidad.

Las **operaciones** mas comunes que se hacen sobre una lista son:

   - Crear.
   - Destruir.
   - Insertar.
   - Buscar.
   - Eliminar.
   - Recorrer.
   - ¿Está vacío?

Un arbol podría tener diferentes implementaciones tales como recursivas o con vectores estáticos, pero suele ser mas utilizada la recursiva por un asunto de facilidad y conceptual. En el caso de la recursiva se puede usar una estructura arbol y que cada nodo tenga sub-árboles o un árbol que contenga nodos y esos nodos contengas hijos nodos.

Un **nodo** es tipo de dato que dependende del tipo de árbol varía, pero contiene un dato y referencias a almenos dos de sus hijos.
El hecho de que la búsqueda sea mas óptima que el arbol binario es importante, dado que para la mayoría de las operaciones se suele buscar un elemento. Sin embargo, no se puede asegurar que la búsqueda sea O(logn) dado a que depende de como se inserten los elementos.

Esta implementación cuenta con tres tipos de recorridos: Inorden, Preorden, Postorden. Si no son interrumpidos, logran recorrer todo el árbol visitando cada nodo una sola vez. 


### Función Destrucción

El objetivo de tener una función destrucción es permitirle al usuario ejecutar alguna acción sobre los elementos del árbol antes de eliminarlo. Ya sea liberar la memoria de los mismos a alguna otra cosa. El árbol se encargará de recorrer todos los elementos del árbol y aplicar el destructor sobre cada uno. Si el usuario no pudiera enviarle esta función al árbol, no sería capaz de liberar la memoria de los elementos. En caso de que los elementos estén en el stack, no hay problema y podría mandarse un puntero NULL.

### Complejidad de las Operaciones


| Operación | Complejidad |
| ------ | ------ |
| Crear | *O(1)* | 
| Destruir | *O(n)* |
| Insertar en lugar *n* | *O(n)* | 
| Eliminar en lugar *n* | *O(n)* |
| Está vacía | *O(1)* |
| Ver Elemento en lugar *n* |

Licencia
----

Derechos reservados © Luchito

