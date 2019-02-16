#ifndef _FILE_H
#define _FILE_H

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "config.h"

//Path buffer size
#define MAX_PATH 128

//File endings
#define TXT_ENDING "txt"
#define JPG_ENDING "jpg"

#define TXT_FILE 0
#define JPG_FILE 1

/*****************************************************
* @Nombre: FILE_readConfig
* @Def: Obtiene la configuracion de un fichero
* @Arg:	In: file_name = nombre del fichero
*		Out: config = configuracion a devolver
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
*****************************************************/
int FILE_readConfig(char * file_name, struct Config * config);

/*************************************************************************
* @Nombre: FILE_getDirectoryFiles
* @Def: Obtiene los ficheros del directorio FILE_PATH
* @Arg:	Out: txt = referencia del puntero a los nombres de ficheros .txt
*		Out: jpg = referencia del puntero a los nombres de ficheros .jpg
*		Out: total = array con el total de ficheros de ambos tipos
*		In: path = capeta donde se encuentran los ficheros
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
*************************************************************************/
int FILE_getDirectoryFiles(char *** txt, char *** jpg, int total[2], char * path);

/****************************************************************
* @Nombre: FILE_readFile
* @Def: Obtiene el contenido de un fichero a nivel de byte
* @Arg:	In: file_name = nombre del fichero
*		Out: bytes = numero de bytes de la informacion obtenida
*		In: path = carpeta donde se encuentra el fichero
*		In: turn_off = flag que indica si se cierra el programa
* @Ret: devuelve el contenido del fichero (NULL si ha ido mal)
****************************************************************/
char * FILE_readFile(char * file_name, long * bytes, char * path, int * turn_off);

/**********************************************************
* @Nombre: FILE_checksum
* @Def: Obten el checksum de un fichero
* @Arg:	In: file_name = nombre del fichero
*		In: path = carpeta donde se encuentra el fichero
* @Ret: devuelve el checksum del fichero
**********************************************************/
char * FILE_checksum(char * file_name, char * path);

/**********************************************************
* @Nombre: FILE_deleteFile
* @Def: Elimina un fichero del sistema de ficheros
* @Arg:	In: file_name = nombre del fichero
*		In: path = carpeta donde se encuentra el fichero
* @Ret: devuelve 0 si ha ido bien y -1 si ha ido mal
**********************************************************/
int FILE_deleteFile(char * file_name, char * path);

#endif