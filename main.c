#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "hashmap.h"
#include "heap.h"
#include <time.h>
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

void agregarTarea(HashMap * map, char nombre[],int prior){
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

  //busco la tarea precedente
  Pair *casillaPrec=searchMap(map,preced);
  if(casillaPrec==NULL){
    printf("no se encontro dato precedente");
    return;
  }
  //inserto esa otra tarea en la lista precedencia para que la tarea guarde la clave de esa tarea
  pushBack(listPrec,casillaPrec->key);
}


List * Ordenar(HashMap * mapOriginal,HashMap * map){
  List *listRealizados=createList();
  HashMap *mapAux=createMap(10002);
  Heap *monMin=createHeap();
  List *listPrec; // una variable para ordenar mejor
  List *listPrecOriginal; // para asignarles una copia de precedentes originales y asignarles al final a las tareas listas para que muestre su lista de precedentes al final
  Pair *casillaAux;   
  Tarea *tareaLista;
  //copia lista de precedencia aqui con un for
  while(size(map)!=0 || sizeH(monMin)!=0){
    //recorremos el mapa
    for(Pair *i=firstMap(map);i!=NULL;i=nextMap(map)){
      listPrec=((Tarea *)i->value)->precedencia;
      //en el caso de que la lista precedencia de una casilla tenga datos aka el dato tiene precedentes se verificara toda su lista para ver si algun precedente ya se hizo

      if(firstList(listPrec)!=NULL){
        for (char *a = firstList(listPrec); a != NULL; ){
          //compruebo en el caso de que el mapAux tenga datos
            casillaAux=searchMap(mapAux,a);
            //en el caso de que la lista de precedentes se encuentre en el mapAux este se eliminará
            if(casillaAux!=NULL){ 
              popCurrent(listPrec); //cuidado
              a = nextList(listPrec);
              continue;
            }
            a = nextList(listPrec);
        }
      }
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

    //copiare la lista de precedentes a las de las listas de tareas hechas
    if(tareaLista==NULL){
      printf("tareaLista fue nulo");
      return NULL;
    }
    Pair *casillaOriginal=searchMap(mapOriginal,(Tarea*) tareaLista->nombre);
    listPrecOriginal=((Tarea *)casillaOriginal->value)->precedencia;
    if(listPrecOriginal!=NULL){ 
      for (char *b = firstList(listPrecOriginal); b != NULL; b = nextList(listPrecOriginal)){
        pushBack(((Tarea *)tareaLista)->precedencia, b);
      }
    }
    //asignare los datos a la lista de realizados
    pushBack(listRealizados,tareaLista); 
    insertMap(mapAux,tareaLista->nombre,tareaLista);
    heap_pop(monMin);
  }
  return listRealizados;
}

void mostrarTareas(List * listOrdenados){
  //a continuacion se mostraran los datos 
  for (Tarea *a = firstList(listOrdenados); a != NULL; a = nextList(listOrdenados)){
    printf("Tarea: %s Prioridad: %d ", ((Tarea*)a)->nombre,((Tarea*)a)->prioridad);
    //recorrere la lista de precedentes de la tarea hecha para mostrar sus respectivos precedentes
    printf("Precedentes: ");
    for (char *b = firstList(((Tarea*)a)->precedencia); b != NULL; b = nextList(((Tarea*)a)->precedencia)){
      printf("(%s) ",b);
    }
    printf("\n");
  }
}

void copiarHashMap(HashMap* mapaOriginal, HashMap* mapaAuxiliar) {
    for (Pair* par = firstMap(mapaOriginal); par != NULL; par = nextMap(mapaOriginal)) {
        Tarea* tareaOriginal = (Tarea*)par->value;

        // creo la variable tareaAuxiliar para el mapa auxiliar
        Tarea* tareaAuxiliar = (Tarea*)malloc(sizeof(Tarea));
        strcpy(tareaAuxiliar->nombre, tareaOriginal->nombre);
        tareaAuxiliar->prioridad = tareaOriginal->prioridad;

        // copio la lista de precedencia (si existe)
        if (tareaOriginal->precedencia != NULL) {
            tareaAuxiliar->precedencia = createList();
            for (char* precedente = firstList(tareaOriginal->precedencia); precedente != NULL; precedente = nextList(tareaOriginal->precedencia)) {
                pushBack(tareaAuxiliar->precedencia, precedente);
            }
        }

        // inserto la tarea copiada en el mapa auxiliar
        insertMap(mapaAuxiliar, tareaAuxiliar->nombre, tareaAuxiliar);
    }
}

void marcListo(List * listOrdenados, HashMap * map, char tareaElim[] ){
  unsigned short comand;
  Pair* casilla;
  List* listPrec;
  Tarea *tarea;
  for(Pair *j=firstMap(map);j!=NULL;j=nextMap(map)){
    tarea=(Tarea*)j->value;
    if(strcmp(tareaElim,tarea->nombre)==0){
      //una vez encontrado veremos diferentes condiciones
      //compruebo el caso en que la tarea tuviera precedencia de ser asi ingreso con distintas condiciones
      if(tarea->precedencia!=NULL){
        
        //compruebo con una advertencia que el usuario este seguro de su desición
        printf("¿Esta seguro que quiere eliminar esta tarea? ingrese si:1 no:0");
        scanf("%hu", &comand);
        while(comand!=0&&comand!=1){
          printf("ingrese comando valido\n");
          scanf("%hu", &comand);
        }
        //si el usuario ingresa si entonces se eliminara la tarea de la lista de tareas ordenadas y lo cu
        if(comand==1){
          popCurrent(listOrdenados);
          eraseMap(map,tareaElim);
          //eliminare a los posibles numero que lo hayan tenido de precedente
          for(Pair *i=firstMap(map);i!=NULL;i=nextMap(map)){ 
            listPrec=((Tarea *)i->value)->precedencia;
            if(listPrec!=NULL){
              for(char *l = firstList(listPrec); l != NULL; l = nextList(listPrec)){
                if(strcmp(l,tareaElim)==0){
                  popCurrent(listPrec);
                }
              }
            }
          }
          //////////////////////////////////////////////////////////////////////
          return;
        }
      }
      //si no tiene precedencia elimino la tarea y lo cuento como realizado instantaneamente
      else{
        popCurrent(listOrdenados);
        eraseMap(map,tareaElim);
        
        //eliminare a los posibles numero que lo hayan tenido de precedente
        for(Pair *i=firstMap(map);i!=NULL;i=nextMap(map)){
          listPrec=((Tarea *)i->value)->precedencia;
          if(listPrec!=NULL){
            for(char *l = firstList(listPrec); l != NULL; l = nextList(listPrec)){
              if(strcmp(l,tareaElim)==0){
                popCurrent(listPrec);
              }
            }
          }
        }
        //////////////////////////////////////////////////////////////////////
        return;
        }
      
    }
  }
  return;  
}


int main()
{
  HashMap *map = createMap(10002); 
  int comand, prioridad;
  List *listOrdenados=createList();
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
      scanf(" %[^\n]s", nombre);
      while (searchMap(map,nombre)!=NULL){
        printf("ingreso un nombre ya ocupado intente denuevo\n");
        scanf(" %[^\n]s", nombre);
      }
      
      
      printf("ingrese la prioridad de su tarea\n");
      scanf("%d",&prioridad);
      
      
      agregarTarea(map,nombre,prioridad);
    }

    if(comand==2){
      printf("ingrese el nombre de la tarea");
      scanf(" %[^\n]s", nombre);
      strcpy(tarea,nombre);

      printf("ingrese el precedente de la tarea y sea conciente de errores logicos");
      scanf(" %[^\n]s", nombre);
      strcpy(preced,nombre);

      estPreced(map,preced,tarea);
      
    }
    
    if(comand==3){
      HashMap *copiaMap = createMap(10002); 
      copiarHashMap(map,copiaMap);
      //lo almaceno aca para usarlo en otras funciones
      listOrdenados=Ordenar(map,copiaMap);
      
      mostrarTareas(listOrdenados);
    }

    if(comand==4){
      HashMap *copiaMap = createMap(10002); 
      copiarHashMap(map,copiaMap);
      //lo almaceno aca para usarlo en otras funciones
      listOrdenados=Ordenar(map,copiaMap);
      
      printf("ingrese el nombre de la tarea a eliminar");
      scanf(" %[^\n]s", nombre);
      strcpy(tarea,nombre);

      marcListo(listOrdenados,map,tarea);
    }
    
    if(comand==0){
      printf("Fin del programa");
      break;
    }
  }
  
  return 0;
}



