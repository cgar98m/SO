#ifndef _NETPACK_H
#define _NETPACK_H

#include <stdlib.h>
#include <string.h>

//Connection types
#define CONNECT_T 0x01
#define DISCONNECT_T 0x02

//Send data types
#define DATA_T 0x03
#define NO_DATA_T 0x04

//Connection headers
#define CONNECT_OK_H "[CONOK]"

//Default header
#define DEFAULT_HEADER "[]"

//File sending headers
#define METADATA_HEADER "[METADATA]"
#define ENDFILE_HEADER "[ENDFILE]"
#define FILE_OK_H "[FILEOK]"
#define FILE_KO_H "[FILEKO]"
#define CHECKSUM_OK_H "[CHECKOK]"
#define CHECKSUM_KO_H "[CHECKKO]"

//NetPack
struct NetPack {
	char type;
	char * header;
	int length;
	char * data;
};

/*******************************************************
* @Nombre: NETPACK_defaultPack
* @Def: Establece los valores por defecto de un paquete
* @Arg:	In/Out: pack = paquete a establecer
* @Ret: ---
*******************************************************/
void NETPACK_defaultPack(struct NetPack * pack);

/***************************************************************************
* @Nombre: NETPACK_cleanPack
* @Def: Libera la memoria de un paquete y establece los valores por defecto
* @Arg:	In/Out: pack = paquete a limpiar
* @Ret: ---
***************************************************************************/
void NETPACK_cleanPack(struct NetPack * pack);

/***************************************************
* @Nombre: NETPACK_makeNetPack
* @Def: Crea un paquete a partir de los parametros
* @Arg:	In: type = tipo del paquete
*		In: header = cabecera del paquete
*		In: data = datos del paquete
*		In: size = tamaño de los datos en bytes
* @Ret: devuelve el paquete creado
***************************************************/
struct NetPack NETPACK_makeNetPack(char type, char * header, char * data, int size);

#endif