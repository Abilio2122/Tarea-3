#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "hashmap.h"
#include "heap.h"
#define BARRA "-------------------------------------------------------"
  
typedef struct Tarea Tarea;

struct Tarea{
  char nombre[31];
  int prioridad;
  List *precedencia; //esta lista guardara las claves de otras tareas
} ;

void mostrarMenu() {
  puts(BARRA);
  printf("                   Organizador de Tareas\n");
  puts(BARRA);
  printf("\nSeleccione una opción:\n\n1. Agregar tarea\n2. Establecer precedencia entre tareas\n3. Mostrar tareas por hacer\n4. Marcar tarea como completada\n0. Salir\n\n");
  puts(BARRA);
}

void agregarTarea(HashMap * map, char nombre[], int prior){
  Tarea *nodo=(Tarea *)malloc(sizeof(Tarea));
  strcpy(nodo->nombre,nombre);
  
  nodo->prioridad=prior;
  insertMap(map, nodo->nombre, nodo);
}

void estPreced(HashMap *map, char preced[], char nombre[]){
  // aca solo quiero agregar la clave de un precedente en una lista dentro de el struct tarea.
  Pair *casilla=searchMap(map,nombre);
  if(casilla==NULL){
    printf("no se encontro dato tarea");
    return;
  }
  
  if(((Tarea *)casilla->value)->precedencia==NULL)
    ((Tarea *)casilla->value)->precedencia=createList();

  //guardo la lista precedente en una variable por orden
  List *listPrec=((Tarea *)casilla->value)->precedencia;

  //busco la tarea prec
  Pair *casillaPrec=searchMap(map,preced);
  if(casillaPrec==NULL){
    printf("no se encontro dato precedente");
    return;
  }
  //inserto esa otra tarea en la lista precedencia para que la tarea guarde la clave de esa tarea
  pushBack(listPrec,casillaPrec->key);
}


void mostrarTareas(HashMap * map){
  List *listRealizados=createList();
  HashMap *mapAux=createMap(10002);
  Heap *monMin=createHeap();
  List *listPrec;// puede dar error
  Pair *casillaAux;
  Tarea *tareaLista;

  while(size(map)!=0 || sizeH(monMin)!=0){
    //recorremos el mapa
    for(Pair *i=firstMap(map);i!=NULL;i=nextMap(map)){
      listPrec=((Tarea *)i->value)->precedencia;
      //en el caso de que la lista precedencia de una casilla tenga datos aka el dato tiene precedentes se verificara toda su lista para ver si algun precedente ya se hizo
      /////////////////////////////////////////////////////
      if(firstList(listPrec)!=NULL){
        for (char *a = firstList(listPrec); a != NULL; a = nextList(listPrec)){
          //compruebo en el caso de que el mapAux tenga datos
          if(size(mapAux)){
            casillaAux=searchMap(mapAux,a);
            //en el caso de que la lista de precedentes se encuentre en el mapAux este se eliminará
            if(casillaAux!=NULL){ 
              popCurrent(listPrec);
            }
          }
        }
      }
      ///////////////////////////////////////////////////////////
      //en el otro caso inserto el dato directamente en el monticulo minimo
      if(firstList(listPrec)==NULL){ 
        heap_push(monMin,i->value,((Tarea *)i->value)->prioridad);
        eraseMap(map,i->key);
      }
    }
    // Una vez terminado el recorrido del arreglo map, se realizarán las siguientes tareas:
        // 1. Se almacenará la raíz de monMin en listRealizados
        // 2. Se guardará también en mapAux
        // 3. Por último, se eliminará la raíz del monMin
    
    tareaLista=heap_top(monMin);
    pushBack(listRealizados,tareaLista);
    insertMap(mapAux,tareaLista->nombre,tareaLista);
    heap_pop(monMin);
  }
  //a continuacion se mostraran los datos precedentes
  for (Tarea *a = firstList(listRealizados); a != NULL; a = nextList(listRealizados)){
    printf("%s %d\n", a->nombre,a->prioridad);
  }
}

int main()
{
  HashMap *map = createMap(10002); 
  int comand, prioridad;
  char nombre[31], tarea[31], preced[31];
  // para crear un numero aleatorio
  srand(time(NULL));  // Inicializar la semilla de generación de números aleatorios
  int min = 1;
  int max = 10;
  int randomNum;
  //////////////////////////////////////////////////////////////////////////////
  while(true){ 
    randomNum = min + rand() % (max - min + 1); // genera randoms
    
    mostrarMenu();
    scanf("%d",&comand);

    while (comand >  4 || comand < 0) {
      printf("Ingrese un número válido \n");
      scanf("%d", &comand);
    }
    
    if(comand==1){
      printf("ingrese el nombre de tarea\n");
      scanf("%s",nombre);
      
      
      /*printf("ingrese la prioridad de su tarea\n");
      scanf("%d",&prioridad);
      */
      
      agregarTarea(map,nombre,randomNum);
      printf("%d",randomNum);
    }

    if(comand==2){
      printf("ingrese el nombre de la tarea");
      scanf("%s",nombre);
      strcpy(tarea,nombre);

      printf("ingrese el precedente de la tarea");
      scanf("%s",nombre);
      strcpy(preced,nombre);

      estPreced(map,preced,tarea);
      
    }
    
    if(comand==3){
      mostrarTareas(map);
    }

    if(comand==0){
      printf("Fin del programa");
      break;
    }
  }
  
  return 0;
}

/*
plan para la funcion procedencia:
-hacer un hashmap para buscar los nodos de la Tarea por clave
-empieza por recibir un nodo de tarea 
-asignarle a ese nodo dentro de su struct la parte de el puntero de procedencia la direccion de el nodo al que le debe la procedencia.

-----------------------------------------------------------------------

plan mostrar tareas
        estructuras necesitadas
        -lista de realizados
        -mapa auxiliar que almacenara las tareas hechas 
        -usar monticulo minimo para reorganizar las tareas
-while se tienen que acabar los datos del mapa
-for se recorre todo el mapa
-en el caso de que no tenga precedente se insertara en el mapa auxiliar y tambien se insertara en el monticulo minimo
-en cada casilla tendra una lista que se comprobara si un dato dentro de esa lista estara en el mapa auxiliar, en el caso de que esté este precedente se eliminara de la lista
-al final de el for se hara lo siguiente
-popheap eliminara la raiz para ponerlo en la lista de realizados
-se agregaran los datos en la lista de tareas realizadas.
-de la lista se pone al mapa auxiliar

---------------------------------------------------------------------

recordatorios: 

-para recorrer listas:

for (Tarea *a = firstList(tareas); a != NULL; a = nextList(tareas)){
    printf("%s %d\n", a->nombre,a->prioridad);
  }
  
  
-para comprobar si la funcion 1 y 2 funcionan correctamente

void mostrarTareas(HashMap * map){
  for(Pair *i=firstMap(map);i!=NULL;i=nextMap(map)){
    printf("%s,%d\n",i->key,((Tarea *)i->value)->prioridad);
    //recorro lista de precedencia casilla
    for (char *a = firstList(((Tarea *)i->value)->precedencia); a != NULL; a = nextList(((Tarea *)i->value)->precedencia)){
      printf("precedentes:");
      printf("%s, ",a);
    }
    printf("\n");
  }
  printf("\n");
}


detalles que ver:
si hay 2 claves con el mismo nombre da error
*/


