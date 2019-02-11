#ifndef _FLIST_H
#define _FLIST_H

#include <stdlib.h>
#include <pthread.h>

//Node
struct Node {
	pthread_t e;
	struct Node * next;
};

//FList
struct FList {
	struct Node * head;
};

/*****************************************************
* @Nombre: FLIST_create
* @Def: Crea una nueva lista de enteros
* @Arg:	Out: list = puntero donde crear la lista
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
*****************************************************/
int FLIST_create(struct FList * list);

/***********************************************************************
* @Nombre: FLIST_add
* @Def: Anade un entero a la lista siempre que no exista ya
* @Arg:	In/Out: list = puntero a la lista
*		In: e = elemento a anadir
* @Ret: devuelve 1 si ha ido bien, -1 si ha ido mal y -2 si ya existia
***********************************************************************/
int FLIST_add(struct FList * list, pthread_t e);

/***********************************************************************
* @Nombre: FLIST_remove
* @Def: Elimina un entero de la lista
* @Arg:	In/Out: list = puntero a la lista
*		In: e = elemento a eliminar
* @Ret: devuelve 1 si ha ido bien, -1 si ha ido mal y -2 si no existia
***********************************************************************/
int FLIST_remove(struct FList * list, pthread_t e);

/***************************************************
* @Nombre: FLIST_getFirst
* @Def: Obten el primer entero de la lista
* @Arg:	In: list = puntero a la lista
*		Out: e = numero a devolver
* @Ret: devuelve 1 si ha ido bien, -1 si ha ido mal
***************************************************/
int FLIST_getFirst(struct FList list, pthread_t * e);

/************************************************************
* @Nombre: FLIST_empty
* @Def: Comprueba si la lista esta vacia
* @Arg:	In: list = lista
* @Ret: devuelve 1 si la lista esta vacia, sino devuelve -1
************************************************************/
int FLIST_empty(struct FList list);

/*****************************************************
* @Nombre: FLIST_destroy
* @Def: Elimina una lista
* @Arg:	In/Out: list = puntero a la lista
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
*****************************************************/
int FLIST_destroy(struct FList * list);

#endif