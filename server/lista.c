#include "lista.h"

Lista LISTA_crea(){
	
	Lista l;			//Variable usada para inicializar la lista
	
	l.pri = (Nodo *)malloc(sizeof(Nodo));
	
	if (l.pri == NULL){
		printf("\nError al inicializar la mano");
	} else {
		
		l.ant = l.pri;
		
		//Se añade el elemento fantasma con valores distintivos
		l.pri->e = -1;
		l.pri->next = NULL;
		
	}

	return l;
	
}

Lista LISTA_inserta(Lista l, int elemento){

	Nodo *aux;			//Auxiliar que anadira un elemento a la lista

	aux = (Nodo *)malloc(sizeof(Nodo));
	
	if (aux != NULL) {
		aux->e = elemento;
		aux->next = l.ant->next;
		l.ant->next = aux;
		l.ant = aux;
	} else {
		printf("\nError al insertar ficha");
	}

	return l;
	
}

int LISTA_consulta(Lista l) {

	int elemento;		//Sirve para mostrar al usuario el elemento que hay en el PDI

	//Comprobamos que el PDI no este vacio
	if (l.ant->next != NULL) {
		elemento = l.ant->next->e;
	}

	return elemento;

}

Lista LISTA_irInicio(Lista l) {
	
	l.ant = l.pri;
	
	return l;

}

int LISTA_fin(Lista l) {

	return (l.ant->next == NULL);

}

Lista LISTA_avanza(Lista l) {

	//Comprobamos si se puede avanzar
	if (l.ant->next != NULL) {
		l.ant = l.ant->next;
	}

	return l;

}

Lista LISTA_borra(Lista l) {

	Nodo *aux;			//Auxiliar usada para borrar el PDI

	//Comprobamos si el PDI esta vacio
	if (l.ant->next == NULL) {
		printf("Error al borrar. La lista esta vacia");
	} else {
		aux = l.ant->next;
		l.ant->next = aux->next;
		free(aux);
	}

	return l;

}

int LISTA_vacia(Lista l) {

	return (l.pri->next == NULL);

}

void LISTA_destruir(Lista l) {

	Nodo *aux;			//Auxiliar usada para destruir cada elemento de la lista

	//Navegamos por la lista de inicio a fin
	while (l.pri != NULL) {
		aux = l.pri;
		l.pri = l.pri->next;
		free(aux);
	}
	
	l.ant = NULL;

}

Lista LISTA_irFin(Lista l) {

	//Avanzamos hasta el penultimo elemento de la lista para asignar el PDI al siguiente elemento
	while (l.ant->next->next != NULL) {
		l = LISTA_avanza(l);
	}

	return l;

}

Lista LISTA_irFinal(Lista l) {

	//Avanzamos hasta el ultimo elemento de la lista para añadir el PDI fuera de la lista
	while (!LISTA_fin(l)) {
		l = LISTA_avanza(l);
	}

	return l;

}