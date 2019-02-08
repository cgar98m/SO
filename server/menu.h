#ifndef MENU_H_
#define MENU_H_

#include <unistd.h>
#include <string.h>
#include <stdio.h>

//Buff size
#define MAX_BUFF 128

//Clear console line
#define CLEAR "\33[2K\r"

//MENU_argError params
#define ARG_MANY 0
#define ARG_FEW 1

//Argument error messages
#define ARG_ERROR_MANY "Error! Too many arguments!\n"
#define ARG_ERROR_FEW "Error! Too few arguments!\n"

//Close program messages
#define CLOSE_PROGRAM "\nClosing Lionel...\n"
#define CLOSE_CLIENTS "Closing connected clients...\n"
#define DUMP_START "Received files register started...\n"

//Config error messages
#define CONFIG_ERROR "Error! Configuration not load!\n"

//Server service messages
#define START_SERVER "Starting Lionel.\n"
#define SERVER_NOT_OPEN "Error! Could not start Lionel!\n"
#define WAIT "Waiting...\n"

//Client connection messages
#define CONNECTION_ERROR "Error! Could not connect to the client!\n"
#define COMMUNICATION_ERROR "\nError! Wrong package received!\n"
#define CONNECT_KO "Error! Could not connect properly to %s!\n"
#define CONNECT_OK "Connection Lionel-%s ready.\n"
#define DISCONNECT_KO "Error! Could not disconnect properly from %s!\n"
#define DISCONNECT_FROM "Disconnecting from %s.\n"

//Server processing error messages
#define INTERNAL_ERROR "Error! Communication process failed!\n"

//File receiving messages
#define PERCENT_MSG "%s -> %.2f%%"
#define RECEIVING_FILE "Receiving data from %s...\n"
#define RECEIVED_FILE "\nFile %s received.\n"

/*****************************************************
* @Nombre: MENU_argError
* @Def: Notifica si hay exceso o falta de parametros
* @Arg:	In: argError = 1 si hay exceso, 0 si faltan
* @Ret: ---
*****************************************************/
void MENU_argError(int arg_error);

/****************************************************
* @Nombre: MENU_connectKo
* @Def: Notifica la conexion erronea con un cliente
* @Arg:	In: telescope = nombre del cliente
* @Ret: ---
****************************************************/
void MENU_connectKo(char * telescope);

/****************************************************
* @Nombre: MENU_connectOk
* @Def: Notifica la conexion correcta con un cliente
* @Arg:	In: telescope = nombre del cliente
* @Ret: ---
****************************************************/
void MENU_connectOk(char * telescope);

/*********************************************
* @Nombre: MENU_disconnectOk
* @Def: Notifica la desconexion de un cliente
* @Arg:	In: telescope = nombre del cliente
* @Ret: ---
*********************************************/
void MENU_disconnectOk(char * telescope);

/******************************************************
* @Nombre: MENU_disconnectKo
* @Def: Notifica la desconexion erronea con un cliente
* @Arg:	In: telescope = nombre del cliente
* @Ret: ---
******************************************************/
void MENU_disconnectKo(char * telescope);

/***********************************************************
* @Nombre: MENU_receivingFile
* @Def: Notifica el comienzo de la recepcion de un fichero
* @Arg:	In: telescope = nombre del cliente
* @Ret: ---
***********************************************************/
void MENU_receivingFile(char * telescope);

/**************************************************************
* @Nombre: MENU_showPercent
* @Def: Muestra el porcentage de fichero que ha sido recibido
* @Arg:	In: telescope = nombre del cliente
* @Ret: ---
**************************************************************/
void MENU_showPercent(char * file_name, float percent);

/*****************************************************
* @Nombre: MENU_receivedFile
* @Def: Notifica la correcta recepcion de un fichero
* @Arg:	In: file_name = nombre del fichero
* @Ret: ---
*****************************************************/
void MENU_receivedFile(char * file_name);

#endif