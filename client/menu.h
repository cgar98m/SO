#ifndef _MENU_H
#define _MENU_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>

//Message buff size
#define MSG_SIZE 128

//MENU_argError params
#define ARG_MANY 0
#define ARG_FEW 1

//Argument messages
#define ARG_ERROR_MANY "Error! Too many arguments!\n"
#define ARG_ERROR_FEW "Error! Too few arguments!\n"

//Close messages
#define CLOSE_PROGRAM "Closing program...\n"

//Configuration messages
#define CONFIG_ERROR "Error! Configuration not load!\n"

//Client started message
#define START_TELESCOPE "Starting %s.\n"

//Connection messages
#define CONNECT "Connecting to Lionel...\n"
#define CONNECT_KO "Could not connect to Lionel...\n"
#define CONNECT_OK "Connection ready.\n"

//Processing messages
#define WAIT "Waiting...\n"

//Disconnection messages
#define DISCONNECT "\nDisconnecting %s.\n"

//File finder messages
#define TESTING_FILES "Testing files...\n"
#define NO_FILES "No files found.\n"
#define FILE_FOUND "File: %s\n"

//File processing messages
#define FILE_READ_ERROR "Error! Could not read %s!\n"
#define DELETE_FILE_ERROR "Error! Could not delete %s\n"
#define FILE_PROCESS_ERROR "Error! Could not process %s\n"

//File sending messages
#define SENDING_FILE "Sending %s...\n"
#define FILE_SENT "File sent.\n"
#define FILE_SENT_ERROR "Error! Could not send %s\n"

/******************************************************************
* @Nombre: MENU_argError
* @Def: Notifica si hay exceso o falta de parametros
* @Arg:	In: argError = ARG_MANY si hay exceso, ARG_FEW si faltan
* @Ret: ---
******************************************************************/
void MENU_argError(int arg_error);

/*************************************************
* @Nombre: MENU_startTelescope
* @Def: Notifica la inicializacion del telescopio
* @Arg:	In: telescope = nombre del telescopio
* @Ret: ---
*************************************************/
void MENU_startTelescope(char * telescope);

/*************************************************
* @Nombre: MENU_disconnect
* @Def: Notifica la desconexion del telescopio
* @Arg:	In: telescope = nombre del telescopio
* @Ret: ---
*************************************************/
void MENU_disconnect(char * telescope);

/********************************************
* @Nombre: MENU_fileFound
* @Def: Notifica la existencia de un fichero
* @Arg:	In: file_name = nombre del fichero
* @Ret: ---
********************************************/
void MENU_fileFound(char * file_name);

/**************************************************
* @Nombre: MENU_fileReadError
* @Def: Notifica un error de lectura de un fichero
* @Arg:	In: file_name = nombre del fichero
* @Ret: ---
**************************************************/
void MENU_fileReadError(char * file_name);

/******************************************
* @Nombre: MENU_sendingFile
* @Def: Notifica el envio de un fichero
* @Arg:	In: file_name = nombre del fichero
* @Ret: ---
******************************************/
void MENU_sendingFile(char * file_name);

/************************************************
* @Nombre: MENU_deleteFileError
* @Def: Notifica la no eliminacion de un fichero
* @Arg:	In: file_name = nombre del fichero
* @Ret: ---
************************************************/
void MENU_deleteFileError(char * file_name);

/*************************************************
* @Nombre: MENU_fileSentError
* @Def: Notifica un error de envio de un fichero
* @Arg:	In: file_name = nombre del fichero
* @Ret: ---
*************************************************/
void MENU_fileSentError(char * file_name);

/*********************************************************
* @Nombre: MENU_fileProcessError
* @Def: Notifica un error de procesamiento de un fichero
* @Arg:	In: file_name = nombre del fichero
* @Ret: ---
*********************************************************/
void MENU_fileProcessError(char * file_name);

#endif