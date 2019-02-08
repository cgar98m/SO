#include <unistd.h>
#include <signal.h>

#include "menu.h"
#include "file.h"
#include "config.h"
#include "net.h"

//Globals
struct Config config;
int server_fd;
int turn_off;

/************************************************************
* @Nombre: getDate
* @Def: Obtiene la fecha del nombre de un fichero jpg o txt
* @Arg:	In: file_name = nombre del fichero
* @Ret: fecha del fichero (si da error, NULL)
************************************************************/
char * getDate(char * file_name) {

	char * date;
	int i = 0;
	
	//Get memory
	date = (char *) malloc(strlen(file_name) - 3);
	if(date == NULL) {
		return NULL;
	}
	
	//Copy until '.' is found
	while(file_name[i] != '.') {
		date[i] = file_name[i];
		i++;
	}
	
	//End of line
	date[i] = '\0';
	return date;

}

/*****************************************************************************
* @Nombre: sendFile
* @Def: Procesa y envia un fichero al servidor
* @Arg:	In: file_name = nombre del fichero
*		In: type = tipo del fichero (txt o jpg)
*		In: path = carpeta donde se encuentra el fichero
* @Ret: devuelve 1 si ha ido bien, -1 si no se ha podido enviar el fichero,
*		-2 si falla el procesamiento de datos y -3 si se cierra el programa
*****************************************************************************/
int sendFile(char * file_name, char type[4], char * path) {
	
	char * data;
	long bytes;
	char * date;
	
	//Check if want to close program
	if(turn_off != -1) {
		return -3;
	}
	
	//Get file data
	MENU_fileFound(file_name);
	data = FILE_readFile(file_name, &bytes, path);
	
	//Check if data is ok
	if(data == NULL) {
		MENU_fileReadError(file_name);
		return -2;
	}
	
	//Get file date
	date = getDate(file_name);
	if(date == NULL) {
		free(data);
		return -2;
	}
	
	//Send file to server
	MENU_sendingFile(file_name);
	int response = NET_sendData(server_fd, data, bytes, date, type, path, &turn_off);

	//Free data
	free(data);
	free(date);
	return response;
	
}

/****************************************************************************
* @Nombre: freeFiles
* @Def: Se encarga de liberar de memoria los nombres de fichero y su puntero
* @Arg:	In/Out: files = puntero a los nombres de ficheros
*		In: min = indice de nombre del fichero por el cual empezar
*		In: total = numero total de nombres de fichero
* @Ret: ---
****************************************************************************/
void freeFiles(char ** files, int min, int total) {
	
	//Free from min to total
	for(int i = min; i < total; i++) {
		free(files[i]);
	}
	
	//Free pointer
	free(files);
	
}

/****************************************************
* @Nombre: disconnect
* @Def: Notifica al servidor de nuestra desconexion
* @Arg:	---
* @Ret: ---
****************************************************/
void disconnect() {
	
	//Notify server from disconnection
	if(NET_disconnectRequest(server_fd, config.telescope) > 0) {
		NET_disconnectReply(server_fd);
	}
	
	//Close connection
	close(server_fd);

}

/**********************************************************************************
* @Nombre: sendAllFiles
* @Def: Procesa y envia todos los ficheros txt y jpg al servidor
* @Arg:	In/Out: txt = ficheros txt
*		In/Out: jpg = ficheros jpg
*		In: total = numero de ficheros txt y jpg
*		In: path = carpeta donde se encuentran los ficheros
* @Ret: ---
**********************************************************************************/
void sendAllFiles(char ** txt, char ** jpg, int total[2], char * path) {
	
	int response;
	
	//Send txts
	for(int i = 0; i < total[0]; i++) {
		
		//Send file
		response = sendFile(txt[i], TXT_ENDING, path);
		if(response > 0) {
			
			//File sent message
			write(1, FILE_SENT, strlen(FILE_SENT));
			
			//Delete file
			if(FILE_deleteFile(txt[i], path) != 0) {
				MENU_deleteFileError(txt[i]);
			}

		} else {
		
			//Check error
			if(response == -1) {	//Send error
				MENU_fileSentError(txt[i]);
			} else {
				if(response == -3) {	//Closing program
					MENU_disconnect(config.telescope);
				} else {	//File processing error
					MENU_fileProcessError(txt[i]);
				}
			}
			
			//Close files
			freeFiles(txt, i, total[0]);
			freeFiles(jpg, 0, total[1]);
			
			//Check if needs to close program
			if(response == -1 || response == -3) {
				disconnect();
				CONFIG_cleanConfig(&config);
				exit(EXIT_FAILURE);
			}
		
		}
		
		//Free file
		free(txt[i]);
		
	}
	
	//Free txt
	if(txt != NULL) {
		free(txt);
	}
	
	//Send jpgs
	for(int i = 0; i < total[1]; i++) {
		
		//Send file
		response = sendFile(jpg[i], JPG_ENDING, path);
		if(response > 0) {
			
			//File sent message
			write(1, FILE_SENT, strlen(FILE_SENT));
			
			//Delete file
			if(FILE_deleteFile(jpg[i], path) != 0) {
				MENU_deleteFileError(jpg[i]);
			}

		} else {
		
			//Check error
			if(response == -1) {	//Send error: close client
				MENU_fileSentError(jpg[i]);
			} else {	//File processing error
				if(response == -3) {	//Closing program
					MENU_disconnect(config.telescope);
				} else {	//File processing error
					MENU_fileProcessError(jpg[i]);
				}
			}
			
			//Close files
			freeFiles(jpg, i, total[1]);
			
			//Check if needs to close program
			if(response == -1 || response == -3) {
				disconnect();
				CONFIG_cleanConfig(&config);
				exit(EXIT_FAILURE);
			}
		
		}
		
		//Free file
		free(jpg[i]);
		
	}
	
	//Free jpg
	if(jpg != NULL) {
		free(jpg);
	}

}

/****************************************************************
* @Nombre: lookForFiles
* @Def: Comprueba si hay ficheros nuevos para enviar al servidor
* @Arg:	---
* @Ret: ---
****************************************************************/
void lookForFiles() {

	char ** txt;
	char ** jpg;
	int total[2];
	
	//Check if want to close program
	if(turn_off != -1) {
		return;
	}
	
	//Read directory files
	write(1, TESTING_FILES, strlen(TESTING_FILES));
	if(FILE_getDirectoryFiles(&txt, &jpg, total, config.files_dir) < 0) {
		MENU_disconnect(config.telescope);
		disconnect();
		CONFIG_cleanConfig(&config);
		exit(EXIT_FAILURE);
	}
	
	//Check if any files exist
	if(total[0] == 0 && total[1] == 0) {
		write(1, NO_FILES, strlen(NO_FILES));	
		freeFiles(txt, 0, total[0]);
		freeFiles(jpg, 0, total[1]);
	} else {	//Send files
		sendAllFiles(txt, jpg, total, config.files_dir);
	}
	
	//Rewrite SIGALRM
	signal(SIGALRM, lookForFiles);

}

/*********************************************
* @Nombre: closeProgram
* @Def: Prepara el cliente para ser cerrado
* @Arg:	---
* @Ret: ---
*********************************************/
void closeProgram() {
	turn_off = 1;
	signal(SIGINT, closeProgram);
}

/********************************************************
* @Nombre: main
* @Def: Inicializa el cliente McGruder
* @Arg:	In: argc = numero de parametros introducidos
*		In: argv = puntero a los parametros introducidos
* @Ret: devuelve 0 si ha ido bien y 1 si ha ido mal
********************************************************/
int main(int argc, char ** argv) {

	//Turn off flag
	turn_off = -1;

	//Set default config
	CONFIG_defaultConfig(&config);
	server_fd = -1;

	//Check arguments
	if(argc != 2) {
		MENU_argError(argc < 2);
		write(1, CLOSE_PROGRAM, strlen(CLOSE_PROGRAM));
		exit(EXIT_FAILURE);
	}
	
	//Get file configuration
	if(FILE_readConfig(argv[1], &config) < 0) {
		write(1, CONFIG_ERROR, strlen(CONFIG_ERROR));
		write(1, CLOSE_PROGRAM, strlen(CLOSE_PROGRAM));
		exit(EXIT_FAILURE);
	}
	
	//Start telescope service
	MENU_startTelescope(config.telescope);
	write(1, CONNECT, strlen(CONNECT));
	if(NET_establishConnection(config.address, config.port, &server_fd) < 0) {
		write(1, CONNECT_KO, strlen(CONNECT_KO));
		CONFIG_cleanConfig(&config);
		exit(EXIT_FAILURE);
	}
	
	//Notify server from telescope
	if(NET_connectRequest(server_fd, config.telescope) < 0) {
		close(server_fd);
		CONFIG_cleanConfig(&config);
		exit(EXIT_FAILURE);
	}
	
	//Wait for server notification
	if(NET_connectReply(server_fd) < 0) {
		close(server_fd);
		CONFIG_cleanConfig(&config);
		exit(EXIT_FAILURE);
	}
	write(1, CONNECT_OK, strlen(CONNECT_OK));
	
	//Rewrite SIGINT
	signal(SIGINT, closeProgram);
	
	//Look for files loop
	signal(SIGALRM, lookForFiles);
	while(turn_off == -1) {
		write(1, WAIT, strlen(WAIT));
		alarm(config.revision_time);
		pause();
	}
	
	//Close connection
	MENU_disconnect(config.telescope);
	disconnect();
	
	//Clear config to exit
	CONFIG_cleanConfig(&config);
	exit(EXIT_SUCCESS);

}