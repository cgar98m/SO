/****************************************************
*
* Proposito: Crear una lista con punto de interes (PDI) que funcione como tablero, fichas restantes y de la mano de los jugadores
* Autores: Christian Garcia Mascarell y Javier Maestro Garcia
* Fecha de creacion: 30/03/17
* Fecha de ultima modificacion: 28/05/17
*
****************************************************/

#ifndef _LISTA_H_
#define _LISTA_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct N {			//Elemento donde se guardan las fichas
	int e;
	struct N *next;
} Nodo;

typedef struct {			//Tipo que maneja la lista
	Nodo *pri;
	Nodo *ant;
} Lista;

/****************************************************
*
* Finalidad: Crea una lista
* Parametros: No recibe ningun parametro
* Retorno: Retorna una lista nueva
*
****************************************************/
Lista LISTA_crea();

/****************************************************
*
* Finalidad: Insertar un elemento en la lista
* Parametros:	in: l = lista donde se anadira el elemento
*				in: elemento = elemento que se anade a la lista
* Retorno: Retorna la lista con el nuevo elemento, el cual es el PDI
*
****************************************************/
Lista LISTA_inserta(Lista l, int elemento);

/****************************************************
*
* Finalidad: Consultar el PDI
* Parametros:	in: l = lista en que se consulta el PDI
* Retorno: Retorna el elemento del PDI
*
****************************************************/
int LISTA_consulta(Lista l);

/****************************************************
*
* Finalidad: Mover el PDI al inicio de la lista
* Parametros:	in: l = lista en que se mueve el PDI
* Retorno: Retorna la lista con el PDI al inicio de la lista
*
****************************************************/
Lista LISTA_irInicio(Lista l);

/****************************************************
*
* Finalidad: Comprobar si el PDI esta en el final de la lista (el PDI esta despues del ultimo elemento de la lista)
* Parametros:	in: l = lista en que se comprueba el PDI
* Retorno: Retorna un 1 si esta al final de la lista y un 0 si no lo esta
*
****************************************************/
int LISTA_fin(Lista l);

/****************************************************
*
* Finalidad: Avanzar el PDI una posicion en la lista
* Parametros:	in: l = lista en que se desplaza el PDI
* Retorno: Retorna la lista con el PDI desplazado una posicion
*
****************************************************/
Lista LISTA_avanza(Lista l);

/****************************************************
*
* Finalidad: Borrar el PDI de la lista (el PDI se resitua en el siguiente elemento de la lista)
* Parametros:	in: l = lista en que se borra el PDI
* Retorno: Retorna la lista sin el PDI anterior
*
****************************************************/
Lista LISTA_borra(Lista l);

/****************************************************
*
* Finalidad: Comprobar si la lista esta vacia
* Parametros:	in: l = lista a comprobar
* Retorno: Retorna un 1 si la lista esta vacia y un 0 si no lo esta
*
****************************************************/
int LISTA_vacia(Lista l);

/****************************************************
*
* Finalidad: Destruir la lista
* Parametros:	in: l = lista a destruir
* Retorno: No retorna ningun parametro
*
****************************************************/
void LISTA_destruir(Lista l);

/****************************************************
*
* Finalidad: Ir al final de la lista (el PDI esta en ultimo elemento de la lista)
* Parametros:	in: l = lista en que se mueve el PDI
* Retorno: Retorna la lista con el PDI al final de la lista
*
****************************************************/
Lista LISTA_irFin(Lista l);

/****************************************************
*
* Finalidad: Ir al final de la lista (el PDI esta despues del ultimo elemento de la lista)
* Parametros:	in: l = lista en que se mueve el PDI
* Retorno: Retorna la lista con el PDI al final de la lista
*
****************************************************/
Lista LISTA_irFinal(Lista l);

#endif