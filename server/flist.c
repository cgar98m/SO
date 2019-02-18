#include "flist.h"

/*****************************************************
* @Nombre: FLIST_create
* @Def: Crea una nueva lista de ids
* @Arg:	Out: list = puntero donde crear la lista
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
*****************************************************/
int FLIST_create(struct FList * list) {

	//Check pointer
	if(list == NULL) {
		return -1;
	}

	//Set up list
	list->head = NULL;
	return 1;
	
}

/***********************************************************************
* @Nombre: FLIST_add
* @Def: Anade un id a la lista siempre que no exista ya
* @Arg:	In/Out: list = puntero a la lista
*		In: e = elemento a anadir
* @Ret: devuelve 1 si ha ido bien, -1 si ha ido mal y -2 si ya existia
***********************************************************************/
int FLIST_add(struct FList * list, pthread_t e) {
	
	//Check pointer
	if(list == NULL) {
		return -1;
	}
	
	//Check if it's empty
	struct Node * node = list->head;
	if(node == NULL) {
		
		//Create node
		node = (struct Node *) malloc(sizeof(struct Node));
		if(node == NULL) {
			return -1;
		}
		
		//Set up node and linking
		node->e = e;
		node->next = NULL;
		list->head = node;
		return 1;
		
	}
	
	//Check if has more elements
	do {
		
		//Check element
		if(node->e == e) {
			return -2;
		}
		
		//Go ahead if possible
		if(node->next != NULL) {
			node = node->next;
		}
		
	} while(node->next != NULL);
	
	//Create node to add element
	struct Node * aux = (struct Node *) malloc(sizeof(struct Node));
	if(aux == NULL) {
		return -1;
	}
	
	//Add element and linking
	aux->e = e;
	aux->next = NULL;
	node->next = aux;
	return 1;
	
}

/***********************************************************************
* @Nombre: FLIST_remove
* @Def: Elimina un id de la lista
* @Arg:	In/Out: list = puntero a la lista
*		In: e = elemento a eliminar
* @Ret: devuelve 1 si ha ido bien, -1 si ha ido mal y -2 si no existia
***********************************************************************/
int FLIST_remove(struct FList * list, pthread_t e) {

	//Check pointer
	if(list == NULL) {
		return -1;
	}
	
	//Check if it's empty
	struct Node * node = list->head;
	if(node == NULL) {
		return -2;
	}
	
	//Check first element
	if(node->e == e) {
		list->head = node->next;		
		free(node);
		return 1;
	}
	
	//Check if has more elements
	while(node->next != NULL) {
		
		//Check element
		if(node->next->e == e) {
			//Delete element
			struct Node * aux = node->next;
			node->next = aux->next;
			free(aux);
			return 1;
		}
		
		//Go ahead
		node = node->next;
	
	}
	
	//Element not found
	return -2;

}

/***************************************************
* @Nombre: FLIST_getFirst
* @Def: Obten el primer id de la lista
* @Arg:	In: list = puntero a la lista
*		Out: e = numero a devolver
* @Ret: devuelve 1 si ha ido bien, -1 si ha ido mal
***************************************************/
int FLIST_getFirst(struct FList list, pthread_t * e) {
	
	//Check empty list
	if(list.head == NULL) {
		return -1;
	}
	
	//Get element
	if(e != NULL) {
		*e = list.head->e;
	}
	return 1;

}

/************************************************************
* @Nombre: FLIST_empty
* @Def: Comprueba si la lista esta vacia
* @Arg:	In: list = puntero a la lista
* @Ret: devuelve 1 si la lista esta vacia, sino devuelve -1
************************************************************/
int FLIST_empty(struct FList list) {
	
	//Check head
	if(list.head == NULL) {
		return 1;
	}
	
	return -1;
	
}

/*****************************************************
* @Nombre: FLIST_destroy
* @Def: Elimina una lista
* @Arg:	In/Out: list = puntero a la lista
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
*****************************************************/
int FLIST_destroy(struct FList * list) {

	//Check pointer
	if(list == NULL) {
		return -1;
	}
	
	//Remove elements until list is empty
	while(FLIST_empty(*list) < 0) {
		FLIST_remove(list, FLIST_getFirst(*list, NULL));
	}
	
	return 1;
	
}