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
	msg.type[0] = '\0';
	msg.bytes = 0;
	msg.file_name[0] = '\0';
	return msg;
}

/********************************************************************
* @Nombre: MSG_newMsg
* @Def: Crea un mensaje con los datos especificados
* @Arg:	In: id = buzon donde se enviara el mensaje
*		In: type = tipo del fichero recibido
*		In: bytes = tamano del fichero
*		In: file_name = nombre del fichero
* @Ret: Mensaje listo para enviar
********************************************************************/
struct NewDataMsg MSG_newMsg(long id, char type[MAX_TYPE], long bytes, char file_name[MAX_FILE]) {
	struct NewDataMsg msg;
	msg.id = id;
	strcpy(msg.type, type);
	msg.bytes = bytes;
	strcpy(msg.file_name, file_name);
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
	msg->type[0] = '\0';
	msg->bytes = 0;
	msg->file_name[0] = '\0';
}