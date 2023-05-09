#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <stdbool.h>
#include <ctype.h>
#define BARRA "-------------------------------------------------------"

typedef struct{
  char *nombre;
  int prioridad;
} Tarea;

void mostrarMenu() {
  puts(BARRA);
  printf("                   Organizador de Tareas\n");
  puts(BARRA);
  printf("\nSeleccione una opción:\n\n1. Agregar tarea\n2. Establecer precedencia entre tareas\n3. Mostrar tareas por hacer\n4. Marcar tarea como completada\n0. Salir\n\n");
  puts(BARRA);
}

void agregarTarea(List * tareas, char *nombre, int prior){
  Tarea *nodo=(Tarea *)malloc(sizeof(Tarea));
  strcpy(nodo->nombre,nombre);
  nodo->prioridad=prior;
  pushBack(tareas,nodo);

  //mostrar lista (pendiente)
  
}

int main()
{
  int comand, prioridad;
  char *nombre=malloc(sizeof(char));
  List *tareas=createList();
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
      printf("llega aca");
      agregarTarea(tareas,nombre,prioridad);
      
    }

    if(comand==0){
      printf("Fin del programa");
      break;
    }
  }
  
  return 0;
}



