#include "msg.h"

/********************************************************************
* @Nombre: MSG_defaultMsg
* @Def: Crea un mensaje sin contenido (util para recibir un mensaje)
* @Arg:	---
* @Ret: Mensaje vacio
********************************************************************/
struct NewDataMsg MSG_defaultMsg() {
	struct NewDataMsg msg;
	msg.id = -1;
	msg.type = -1;
	msg.kbytes = 0.0f;
	msg.astrodata = ASTRODATA_defaultData();
	return msg;
}

/********************************************************************
* @Nombre: MSG_newMsg
* @Def: Crea un mensaje con los datos especificados
* @Arg:	In: id = buzon donde se enviara el mensaje
*		In: type = tipo del fichero recibido
*		In: kbytes = tamano del fichero
*		In: astrodata = informacion del fichero de texto
* @Ret: Mensaje listo para enviar
********************************************************************/
struct NewDataMsg MSG_newMsg(long id, int type, double kbytes, struct AstroData astrodata) {
	struct NewDataMsg msg;
	msg.id = id;
	msg.type = type;
	msg.kbytes = kbytes;
	msg.astrodata = astrodata;
	return msg;
}

/*******************************************
* @Nombre: MSG_cleanMsg
* @Def: Limpia la estructura de un mensaje
* @Arg:	In/Out: Mensaje a limpiar
* @Ret: ---
*******************************************/
void MSG_cleanMsg(struct NewDataMsg * msg) {
	msg->id = -1;
	msg->type = -1;
	msg->kbytes = 0.0f;
	ASTRODATA_cleanData(&(msg->astrodata));
}