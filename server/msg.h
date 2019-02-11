#ifndef _MSG_H
#define _MSG_H

#include <string.h>

//Max file type
#define MAX_TYPE 4

//Max file name
#define MAX_FILE 20

//NewDataMsg
struct NewDataMsg {
	long id;
	char type[MAX_TYPE];
	long bytes;
	char file_name[MAX_FILE];
};

/********************************************************************
* @Nombre: MSG_defaultMsg
* @Def: Crea un mensaje sin contenido (util para recibir un mensaje)
* @Arg:	---
* @Ret: Mensaje vacio
********************************************************************/
struct NewDataMsg MSG_defaultMsg();

/********************************************************************
* @Nombre: MSG_newMsg
* @Def: Crea un mensaje con los datos especificados
* @Arg:	In: id = buzon donde se enviara el mensaje
*		In: type = tipo del fichero recibido
*		In: bytes = tamano del fichero
*		In: file_name = nombre del fichero
* @Ret: Mensaje listo para enviar
********************************************************************/
struct NewDataMsg MSG_newMsg(long id, char type[MAX_TYPE], long bytes, char file_name[MAX_FILE]);

/*******************************************
* @Nombre: MSG_cleanMsg
* @Def: Limpia la estructura de un mensaje
* @Arg:	In/Out: Mensaje a limpiar
* @Ret: ---
*******************************************/
void MSG_cleanMsg(struct NewDataMsg * msg);

#endif