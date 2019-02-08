#include "menu.h"

/******************************************************************
* @Nombre: MENU_argError
* @Def: Notifica si hay exceso o falta de parametros
* @Arg:	In: argError = ARG_MANY si hay exceso, ARG_FEW si faltan
* @Ret: ---
******************************************************************/
void MENU_argError(int arg_code) {
	switch(arg_code) {
		case ARG_MANY:
			write(1, ARG_ERROR_MANY, strlen(ARG_ERROR_MANY));
			break;
		case ARG_FEW:
			write(1, ARG_ERROR_FEW, strlen(ARG_ERROR_FEW));
			break;
	}
}

/*************************************************
* @Nombre: MENU_startTelescope
* @Def: Notifica la inicializacion del telescopio
* @Arg:	In: telescope = nombre del telescopio
* @Ret: ---
*************************************************/
void MENU_startTelescope(char * telescope) {
	char msg[MSG_SIZE];
	int bytes = sprintf(msg, START_TELESCOPE, telescope);
	write(1, msg, bytes);
}

/*************************************************
* @Nombre: MENU_disconnect
* @Def: Notifica la desconexion del telescopio
* @Arg:	In: telescope = nombre del telescopio
* @Ret: ---
*************************************************/
void MENU_disconnect(char * telescope) {
	char msg[MSG_SIZE];
	int bytes = sprintf(msg, DISCONNECT, telescope);
	write(1, msg, bytes);
}

/********************************************
* @Nombre: MENU_fileFound
* @Def: Notifica la existencia de un fichero
* @Arg:	In: file_name = nombre del fichero
* @Ret: ---
********************************************/
void MENU_fileFound(char * file_name) {
	char msg[MSG_SIZE];
	int bytes = sprintf(msg, FILE_FOUND, file_name);
	write(1, msg, bytes);
}

/**************************************************
* @Nombre: MENU_fileReadError
* @Def: Notifica un error de lectura de un fichero
* @Arg:	In: file_name = nombre del fichero
* @Ret: ---
**************************************************/
void MENU_fileReadError(char * file_name) {
	char msg[MSG_SIZE];
	int bytes = sprintf(msg, FILE_READ_ERROR, file_name);
	write(1, msg, bytes);
}

/******************************************
* @Nombre: MENU_sendingFile
* @Def: Notifica el envio de un fichero
* @Arg:	In: file_name = nombre del fichero
* @Ret: ---
******************************************/
void MENU_sendingFile(char * file_name) {
	char msg[MSG_SIZE];
	int bytes = sprintf(msg, SENDING_FILE, file_name);
	write(1, msg, bytes);
}

/************************************************
* @Nombre: MENU_deleteFileError
* @Def: Notifica la no eliminacion de un fichero
* @Arg:	In: file_name = nombre del fichero
* @Ret: ---
************************************************/
void MENU_deleteFileError(char * file_name) {
	char msg[MSG_SIZE];
	int bytes = sprintf(msg, DELETE_FILE_ERROR, file_name);
	write(1, msg, bytes);
}

/*************************************************
* @Nombre: MENU_fileSentError
* @Def: Notifica un error de envio de un fichero
* @Arg:	In: file_name = nombre del fichero
* @Ret: ---
*************************************************/
void MENU_fileSentError(char * file_name) {
	char msg[MSG_SIZE];
	int bytes = sprintf(msg, FILE_SENT_ERROR, file_name);
	write(1, msg, bytes);
}

/*********************************************************
* @Nombre: MENU_fileProcessError
* @Def: Notifica un error de procesamiento de un fichero
* @Arg:	In: file_name = nombre del fichero
* @Ret: ---
*********************************************************/
void MENU_fileProcessError(char * file_name) {
	char msg[MSG_SIZE];
	int bytes = sprintf(msg, FILE_PROCESS_ERROR, file_name);
	write(1, msg, bytes);
}