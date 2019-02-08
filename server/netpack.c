#include "netpack.h"
#include <stdio.h>
#include <unistd.h>

/*******************************************************
* @Nombre: NETPACK_defaultPack
* @Def: Establece los valores por defecto de un paquete
* @Arg:	In/Out: pack = paquete a establecer
* @Ret: ---
*******************************************************/
void NETPACK_defaultPack(struct NetPack * pack) {
	pack->type = 0x00;
	pack->header = NULL;
	pack->length = -1;
	pack->data = NULL;
}

/***************************************************************************
* @Nombre: NETPACK_cleanPack
* @Def: Libera la memoria de un paquete y establece los valores por defecto
* @Arg:	In/Out: pack = paquete a limpiar
* @Ret: ---
***************************************************************************/
void NETPACK_cleanPack(struct NetPack * pack) {
	
	pack->type = 0x00;
	
	if(pack->header != NULL) {
		free(pack->header);
		pack->header = NULL;
	}
	
	pack->length = -1;
	
	if(pack->data != NULL) {
		free(pack->data);
		pack->data = NULL;
	}
	
}

/***************************************************
* @Nombre: NETPACK_makeNetPack
* @Def: Crea un paquete a partir de los parametros
* @Arg:	In: type = tipo del paquete
*		In: header = cabecera del paquete
*		In: data = datos del paquete
* @Ret: devuelve el paquete creado
***************************************************/
struct NetPack NETPACK_makeNetPack(char type, char * header, char * data) {

	struct NetPack pack;
	
	//Set default config
	NETPACK_defaultPack(&pack);
	
	//Set type
	pack.type = type;
	
	//Set header
	pack.header = (char *) malloc(strlen(header));
	if(pack.header == NULL) {
		NETPACK_cleanPack(&pack);
		return pack;
	}
	strcpy(pack.header, header);
	
	//Check data
	if(data == NULL) {
		pack.length = 0;
		return pack;
	}
	
	//Set data
	pack.data = (char *) malloc(strlen(data));
	if(pack.data == NULL) {
		NETPACK_cleanPack(&pack);
		return pack;
	}
	strcpy(pack.data, data);
	
	//Set length
	pack.length = strlen(data) + 1;
	return pack;
	
}