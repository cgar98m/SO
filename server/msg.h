#ifndef _MSG_H
#define _MSG_H

#include "astrodata.h"

//Max file type
#define MAX_TYPE 4

//File types
#define JPG_TYPE 0
#define TXT_TYPE 1

//NewDataMsg
struct NewDataMsg {
	long id;
	int type;
	double kbytes;
	struct AstroData astrodata;
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
*		In: kbytes = tamano del fichero
*		In: astrodata = informacion del fichero de texto
* @Ret: Mensaje listo para enviar
********************************************************************/
struct NewDataMsg MSG_newMsg(long id, int type, double kbytes, struct AstroData astrodata);

/*******************************************
* @Nombre: MSG_cleanMsg
* @Def: Limpia la estructura de un mensaje
* @Arg:	In/Out: Mensaje a limpiar
* @Ret: ---
*******************************************/
void MSG_cleanMsg(struct NewDataMsg * msg);

#endif