#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "hashmap.h"
#define BARRA "-------------------------------------------------------"
  
typedef struct Tarea Tarea;

struct Tarea{
  char nombre[31];
  int prioridad;
  List *precedencia; // duda esta lista podra apuntar a otras tareas? o solo su copia de datos?
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

//||
void estPreced(HashMap *map, char preced[], char nombre[]){
  // aca solo quiero agregar un precedente en una lista dentro de el struct tarea. la idea que esta lista apunte a otras tareas como precedentes y valide luego de si la tarea siga teniendo precedentes para el monticulo de la siguiente funcion.
  Pair *casilla=searchMap(map,nombre);
  if(casilla==NULL){
    printf("no se encontro dato tarea");
    return;
  }
  ((Tarea *)casilla->value)->precedencia=createList();
  List *listPrec=((Tarea *)casilla->value)->precedencia;

  //busco la tarea prec
  Pair *casillaPrec=searchMap(map,preced);
  if(casillaPrec==NULL){
    printf("no se encontro dato precedente");
    return;
  }
  //inserto esa otra tarea en la lista precedencia para que la tarea apunte a esa otra tarea
  pushBack(listPrec,casillaPrec);
}

//||
void mostrarTareas(HashMap * map){
  for(Pair *i=firstMap(map);i!=NULL;i=nextMap(map)){
    printf("%s,%d\n",i->key,((Tarea *)i->value)->prioridad);
    //recorro lista de precedencia casilla
    //si es null se inserta de los primeros en el heap
  }
}

int main()
{
  HashMap *map = createMap(10002); 
  int comand, prioridad;
  char nombre[31], tarea[31], preced[31];
  
  while(true){ 
    mostrarMenu();
    scanf("%d",&comand);

    while (comand >  4 || comand < 0) {
      printf("Ingrese un número válido \n");
      scanf("%d", &comand);
    }
    
    if(comand==1){
      printf("ingrese el nombre de tarea\n");
      scanf("%s",nombre);

      printf("ingrese la prioridad de su tarea\n");
      scanf("%d",&prioridad);
      agregarTarea(map,nombre,prioridad);
      
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

plan mostrar tareas
        estructuras necesitadas
        -lista de realizados
        -mapa auxiliar que almacenara las tareas hechas 
        -usar monticulo minimo para reorganizar las tareas
-while se tienen que acabar los datos del mapa
-for se recorre todo el mapa
-en el caso de que no tenga precedente se insertara en el mapa auxiliar y tambien se insertara en el monticulo minimo
-en cada casilla tendra una lista que se comprobara si un dato dentro de esa lista estara en el mapa auxiliar, en el caso de que este este precedente se eliminara de la lista
-al final de el for se hara lo siguiente
-popheap eliminara la raiz para ponerlo en la lista de realizados
-se agregaran los datos en la lista de tareas realizadas.
-de la lista se pone al mapa auxiliar

recordatorios: 

-para recorrer listas:
for (Tarea *a = firstList(tareas); a != NULL; a = nextList(tareas)){
    printf("%s %d\n", a->nombre,a->prioridad);
  }

*/


