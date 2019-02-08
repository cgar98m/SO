#include "menu.h"

/*****************************************************
* @Nombre: MENU_argError
* @Def: Notifica si hay exceso o falta de parametros
* @Arg:	In: argError = 1 si hay exceso, 0 si faltan
* @Ret: ---
*****************************************************/
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

/****************************************************
* @Nombre: MENU_connectKo
* @Def: Notifica la conexion erronea con un cliente
* @Arg:	In: telescope = nombre del cliente
* @Ret: ---
****************************************************/
void MENU_connectKo(char * telescope) {
	char buff[MAX_BUFF];
	int bytes = sprintf(buff, CONNECT_KO, telescope);
	write(1, buff, bytes);
}

/****************************************************
* @Nombre: MENU_connectOk
* @Def: Notifica la conexion correcta con un cliente
* @Arg:	In: telescope = nombre del cliente
* @Ret: ---
****************************************************/
void MENU_connectOk(char * telescope) {
	char buff[MAX_BUFF];
	int bytes = sprintf(buff, CONNECT_OK, telescope);
	write(1, buff, bytes);
}

/******************************************************
* @Nombre: MENU_disconnectKo
* @Def: Notifica la desconexion erronea con un cliente
* @Arg:	In: telescope = nombre del cliente
* @Ret: ---
******************************************************/
void MENU_disconnectKo(char * telescope) {
	char buff[MAX_BUFF];
	int bytes = sprintf(buff, DISCONNECT_KO, telescope);
	write(1, buff, bytes);
}

/*********************************************
* @Nombre: MENU_disconnectOk
* @Def: Notifica la desconexion de un cliente
* @Arg:	In: telescope = nombre del cliente
* @Ret: ---
*********************************************/
void MENU_disconnectOk(char * telescope) {
	char buff[MAX_BUFF];
	int bytes = sprintf(buff, DISCONNECT_FROM, telescope);
	write(1, buff, bytes);
}

/***********************************************************
* @Nombre: MENU_receivingFile
* @Def: Notifica el comienzo de la recepcion de un fichero
* @Arg:	In: telescope = nombre del cliente
* @Ret: ---
***********************************************************/
void MENU_receivingFile(char * telescope) {
	char buff[MAX_BUFF];
	int bytes = sprintf(buff, RECEIVING_FILE, telescope);
	write(1, buff, bytes);
}

/**************************************************************
* @Nombre: MENU_showPercent
* @Def: Muestra el porcentage de fichero que ha sido recibido
* @Arg:	In: telescope = nombre del cliente
* @Ret: ---
**************************************************************/
void MENU_showPercent(char * file_name, float percent) {
	
	//Clean line
	char buff[MAX_BUFF];
	int bytes = sprintf(buff, CLEAR);
	write(1, buff, bytes);
	
	//Write %
	bytes = sprintf(buff, PERCENT_MSG, file_name, percent);
	write(1, buff, bytes);
	
}

/*****************************************************
* @Nombre: MENU_receivedFile
* @Def: Notifica la correcta recepcion de un fichero
* @Arg:	In: file_name = nombre del fichero
* @Ret: ---
*****************************************************/
void MENU_receivedFile(char * file_name) {
	char buff[MAX_BUFF];
	int bytes = sprintf(buff, RECEIVED_FILE, file_name);
	write(1, buff, bytes);
}