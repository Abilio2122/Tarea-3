#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

typedef struct Node Node;

struct Node {
    void * data;
    Node * next;
    Node * prev;
};

struct List {
    Node * head;
    Node * tail;
    Node * current;
};

typedef List List;

Node * createNode(void * data) {
    Node * new = (Node *)malloc(sizeof(Node));
    assert(new != NULL);
    new->data = data;
    new->prev = NULL;
    new->next = NULL;
    return new;
}

List * createList() {
  List* list=(List*) malloc(sizeof(List));
  list->head=NULL;
  list->tail=NULL;
  list->current=NULL;
  return list;
}

void * firstList(List * list) {
  if (list->head==NULL){
    return NULL; 
  }
  list->current=list->head;
  return list->current->data;
}

void * nextList(List * list) {
  if(list->current==NULL)
  {
    return NULL;
  }
  if(list->current->next==NULL){
    return NULL;
  }
  list->current=list->current->next;
  return list->current->data;
  
}

void * lastList(List * list) {
  while(list->current->next){
    list->current=list->current->next;
  }
  list->current=list->tail;
  return list->tail->data;
}

void * prevList(List * list) {
  if(list->current==NULL){
    return NULL;
  }  
  if(list->current->prev!=NULL){ 
    list->current=list->current->prev;
    return list->current->data;
  }
  return NULL;
}

void pushFront(List * list, void * data) {
  Node * n = createNode(data);
  n->next=list->head;
  if(list->head){
    list->head->prev=n;
    list->head=n;
    n->prev=NULL;
  }
  else{
    list->head=n;
    list->tail=n;
  }
  
}

void pushBack(List * list, void * data) {
    list->current = list->tail;
    pushCurrent(list,data);
}

void pushCurrent(List * list, void * data) {
  Node * n = createNode(data);
  n->prev=list->current;
  list->current->next=n;
  if(list->current==list->tail)
    list->tail=n;
}

void * popFront(List * list) {
    list->current = list->head;
    return popCurrent(list);
}

void * popBack(List * list) {
    list->current = list->tail;
    return popCurrent(list);
}

void * popCurrent(List * list) {
  void * aux=list->current->data;
  //si el current esta al final de la lista
  if(list->current==list->tail){
    list->tail=list->current->prev;
    list->tail->next=NULL;
    list->current->prev=NULL;
  }
  //si el current no tiene extremos nulos
  else if(list->current->prev!=NULL&&list->current->next!=NULL){ 
    list->current->prev->next=list->current->next;
    list->current->next->prev=list->current->prev;
  }
  //si el current esta al principio de la lista
  if(list->current==list->head){
    list->head=list->head->next;
    list->head->prev=NULL;

  }
  //retorno dato eliminado
  list->current=list->current->next;
  return aux;
}

void cleanList(List * list) {
    while (list->head != NULL) {
        popFront(list);
    }
}

