#ifndef FILE_H_
#define FILE_H_

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "config.h"
#include "sendconfig.h"
#include "astrodata.h"

//Path buff size
#define MAX_PATH 128

//Files path
#define FILE_PATH "server_files/"

//Register files
#define IMG_REGISTER "Kalkun.txt"
#define TXT_REGISTER "Kalkun_v2.txt"

/***************************************************************
* @Nombre: FILE_readConfig
* @Def: Obtiene la configuracion de un fichero de configuracion
* @Arg:	Out: config = configuracion vacia
*		In: file_name = nombre del fichero
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
***************************************************************/
int FILE_readConfig(struct Config * config, char * file_name);

/****************************************************
* @Nombre: FILE_checksum
* @Def: Obtiene el checksum de un fichero
* @Arg:	In: file_name = nombre del fichero
* @Ret: devuelve el checksum (NULL si hay un error)
****************************************************/
char * FILE_checksum(char * file_name);

/*****************************************************************
* @Nombre: FILE_createFile
* @Def: Crea un fichero a partir de la informacion proporcionada
* @Arg:	In: file_name = nombre del fichero
*		In: data = informacion
*		In: bytes = longitud de la informacion
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
*****************************************************************/
int FILE_createFile(char * file_name, char * data, long bytes);

/************************************************************
* @Nombre: FILE_updateRegister
* @Def: Anade informacion al registro especificado
* @Arg:	In: info = informacion nueva
*		In: file = fichero a actualizar
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
************************************************************/
int FILE_updateRegister(char * info, char * file);

/**************************************************************
* @Nombre: FILE_getAstroData
* @Def: Obtiene los datos de las constelaciones de un fichero
* @Arg:	In: file_name = fichero a leer
*		Out: data = informacion de las constelaciones
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
**************************************************************/
int FILE_getAstroData(char * file_name, struct AstroData * data);

#endif