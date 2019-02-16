#include "net.h"

/********************************************************
* @Nombre: NET_establishConnection
* @Def: Establece la conexion con el servidor
* @Arg:	In: ip = direccion ip del servidor
*		In: port = puerto del servidor
*		Out: server_fd = file descriptor de la conexion
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
********************************************************/
int NET_establishConnection(char * ip, int port, int * server_fd) {
	
	struct sockaddr_in addr;
	
	//Open socket
	*server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(*server_fd < 0) {
		return -1;
	}
	
	//Prepare struct
	memset(&addr, '0', sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	
	//Set ip
	if(inet_aton(ip, &addr.sin_addr) == 0) {
		close(*server_fd);
		return -1;
	}
	
	//Establish connection
	if(connect(*server_fd, (void *) &addr, sizeof(addr)) < 0) {
		close(*server_fd);
		return -1;
	}
	
	//All ok
	return 1;
	
}

/*************************************************************
* @Nombre: readByteUntil
* @Def: Lee byte a byte de un fd hasta encontrar un delimiter
* @Arg:	In: fd = file descriptor a leer
*		In: delimiter = delimitador del mensage
* @Ret: devuelve los bytes leidos (NULL si hay error)
*************************************************************/
char * readByteUntil(int fd, char delimiter) {

	char * data;
	char * new_data;
	char aux;
	int bytes = 0;

	//Get memory
	data = (char *) malloc(sizeof(char));
	if(data == NULL) {
		return NULL;
	}
	
	//Read data
	do {
		
		//Read char
		if(read(fd, &aux, sizeof(char)) != 1) {
			break;
		}
		
		//Check value
		if(aux != delimiter) {
			
			//Assign value
			data[bytes] = aux;
			bytes++;
			
			//Get memory
			new_data = (char *) realloc(data, bytes + 1);
			if(new_data == NULL) {
				free(data);
				return NULL;
			} else {
				data = new_data;
			}
			
		} else {
			if(delimiter != '\0') {
				
				//Assign value
				data[bytes] = aux;
				bytes++;
				
				//Get memory
				new_data = (char *) realloc(data, bytes + 1);
				if(new_data == NULL) {
					free(data);
					return NULL;
				} else {
					data = new_data;
				}
				
			}
		}
		
	} while(aux != delimiter);
	
	//Replace delimiter
	data[bytes] = '\0';
	return data;
	
}

/***********************************************
* @Nombre: getNetPack
* @Def: Recibe un paquete del servidor
* @Arg:	In: fd = file descriptor de la conexion
* @Ret: devuelve el paquete recibido
************************************************/
struct NetPack getNetPack(int fd) {
	
	struct NetPack pack;
	
	//Prepare struct
	NETPACK_defaultPack(&pack);
	
	//Get type
	if(read(fd, &pack.type, 1) <= 0) {
		return pack;
	}
	
	//Get header
	pack.header = readByteUntil(fd, ']');
	if(pack.header == NULL) {
		NETPACK_cleanPack(&pack);
		return pack;
	}
	
	//Get length
	if(read(fd, &pack.length, sizeof(int)) <= 0) {
		NETPACK_cleanPack(&pack);
		return pack;
	}
	
	//Check length
	if(pack.length == 0) {
		return pack;
	}
	
	//Init data
	pack.data = (char *) malloc(pack.length);
	if(pack.data == NULL) {
		NETPACK_cleanPack(&pack);
		return pack;
	}
	
	//Get data
	if(read(fd, pack.data, pack.length) <= 0) {
		NETPACK_cleanPack(&pack);
	}

	//All ok
	return pack;

}

/****************************************************
* @Nombre: sendNetPack
* @Def: Envia un paquete al servidor
* @Arg:	In: fd = file descriptor de la conexion
*		In: pack = paquete a enviar
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
****************************************************/
int sendNetPack(int fd, struct NetPack pack) {

	//Send type
	if(write(fd, &pack.type, 1) <= 0) {
		return -1;
	}

	//Send header
	if(write(fd, pack.header, strlen(pack.header)) <= 0) {
		return -1;
	}

	//Send size
	if(write(fd, &pack.length, sizeof(int)) <= 0) {
		return -1;
	}

	//Check if has data to send
	if(pack.length == 0) {
		return 1;
	}

	//Send data
	if(write(fd, pack.data, pack.length) <= 0) {
		return -1;
	}

	return 1;

}

/******************************************************
* @Nombre: NET_connectRequest
* @Def: Notifica al servidor del cliente conectado
* @Arg:	In: server_fd = file descriptor de la conexion
*		In: telescope = nombre del telescopio cliente
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
******************************************************/
int NET_connectRequest(int server_fd, char * telescope) {

	struct NetPack pack;

	//Make net pack
	pack = NETPACK_makeNetPack(CONNECT_T, DEFAULT_HEADER, telescope, strlen(telescope));
	if(pack.length == -1) {
		return -1;
	}
	
	//Send net pack
	if(sendNetPack(server_fd, pack) < 0) {
		NETPACK_cleanPack(&pack);
		return -1;
	}
	
	//Clean net pack
	NETPACK_cleanPack(&pack);
	return 1;

}

/******************************************************
* @Nombre: NET_connectReply
* @Def: Espera la respuesta de conexion del servidor
* @Arg:	In: server_fd = file descriptor de la conexion
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
******************************************************/
int NET_connectReply(int server_fd) {
	
	struct NetPack pack;
	
	//Get net pack
	pack = getNetPack(server_fd);
	if(pack.length != -1 && pack.type == CONNECT_T && strcmp(pack.header, CONNECT_OK_H) == 0) {
		NETPACK_cleanPack(&pack);
		return 1;
	}
	
	NETPACK_cleanPack(&pack);
	return -1;
	
}

/**********************************************************
* @Nombre: NET_disconnectRequest
* @Def: Notifica al servidor de la desconexion del cliente
* @Arg:	In: server_fd = file descriptor de la conexion
*		In: telescope = nombre del cliente
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
**********************************************************/
int NET_disconnectRequest(int server_fd, char * telescope) {

	struct NetPack pack;

	//Set up package
	pack = NETPACK_makeNetPack(DISCONNECT_T, DEFAULT_HEADER, telescope, strlen(telescope));
	if(pack.length == -1) {
		return -1;
	}
	
	//Send package
	if(sendNetPack(server_fd, pack) < 0) {
		NETPACK_cleanPack(&pack);
		return -1;
	}
	
	NETPACK_cleanPack(&pack);
	return 1;

}

/********************************************************
* @Nombre: NET_disconnectReply
* @Def: Espera la respuesta de desconexion del servidor
* @Arg:	In: server_fd = file descriptor de la conexion
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
********************************************************/
int NET_disconnectReply(int server_fd) {
	
	struct NetPack pack;
	
	//Prepare struct
	pack = getNetPack(server_fd);
	if(pack.length != -1 && pack.type == DISCONNECT_T && strcmp(pack.header, CONNECT_OK_H) == 0) {
		NETPACK_cleanPack(&pack);
		return 1;
	}
	
	NETPACK_cleanPack(&pack);
	return -1;
	
}

/************************************************************************
* @Nombre: sendDataFragment
* @Def: Envia un fragmento de la informacion de un fichero
* @Arg:	In: data = informacion a enviar
*		In: start = primer byte a enviar
*		In: size = total de bytes a enviar
*		In: server_fd = file descriptor de la conexion
* @Ret: devuelve 1 si ha ido bien, -1 si falla el envio al servidor o
*		el servidor se ha desconectado y -2 si falla el procesamiento
*		de datos
************************************************************************/
int sendDataFragment(char * data, long start, long size, int server_fd) {
	
	//Get data fragment
	int send_size = size < DATA_LENGTH ? size : DATA_LENGTH;
	char * send_data = (char *) malloc(send_size);
	if(send_data == NULL) {
		return -2;
	}
	
	long i;
	for(i = 0; i < send_size; i++) {
		send_data[i] = data[i + start];
	}
	
	//Set up metadata package
	struct NetPack pack = NETPACK_makeNetPack(DATA_T, DEFAULT_HEADER, send_data, send_size);
	if(pack.length == -1) {
		return -2;
	}
	free(send_data);
	
	//Send package
	if(sendNetPack(server_fd, pack) < 0) {
		NETPACK_cleanPack(&pack);
		return -1;
	}
	
	NETPACK_cleanPack(&pack);
	return 1;

}

/********************************************************************************************
* @Nombre: sendDataEnd
* @Def: Envia un mensage al servidor informando del fin de la transmision de la informacion
* @Arg:	In: file_name = nombre del fichero enviado
*		In: path = carpeta donde se encuentra el fichero
*		In: server_fd = file descriptor de la conexion
* @Ret: devuelve 1 si ha ido bien, -1 si falla el envio al servidor o el servidor se ha
*		desconectado y -2 si falla el procesamiento de datos
********************************************************************************************/
int sendDataEnd(char * file_name, char * path, int server_fd) {
	
	//Get checksum
	char * checksum = FILE_checksum(file_name, path);
	if(checksum == NULL) {
		free(file_name);
		return -2;
	}
	free(file_name);
	
	//Set up metadata package
	struct NetPack pack = NETPACK_makeNetPack(DATA_T, ENDFILE_HEADER, checksum, strlen(checksum));
	free(checksum);
	if(pack.length == -1) {
		return -2;
	}
	
	//Send package
	if(sendNetPack(server_fd, pack) < 0) {
		NETPACK_cleanPack(&pack);
		return -1;
	}
	
	NETPACK_cleanPack(&pack);
	return 1;

}

/*******************************************************************************
* @Nombre: dataRespondManagement
* @Def: Procesa la respuesta del servidor en el envio de un fichero
* @Arg:	In: header = cabecera de la respuesta del servidor
*		In: current_bytes = bytes ya enviados
*		In: bytes = total de bytes de la informacion
*		In: data = informacion del fichero
*		In: file_name = nombre del fichero
*		In: server_fd = file descriptor de la conexion con el servidor
* @Ret: devuelve 1 si ha ido bien, 2 si se ha enviado toda la información,
*		-1 si falla la respuesta del servidor o el servidor se ha desconectado,
*		-2 si falla el procesamiento de datos y -3 si se quiere desconectar el
*		cliente
*******************************************************************************/
int dataRespondManagement(char * header, long * current_bytes, long bytes, char * data, char * file_name, int server_fd, char * path) {

	//Check file ok
	int response;
	if(strcmp(header, FILE_OK_H) == 0) {
		//Check if needs more data to send
		if(*current_bytes < bytes) {
			response = sendDataFragment(data, *current_bytes, bytes - *current_bytes, server_fd);
			if(response < 0) {
				return response;
			}
			(*current_bytes) += DATA_LENGTH;
		} else {
			response = sendDataEnd(file_name, path, server_fd);
			if(response < 0) {
				return response;
			}
		}
	}

	//Check checksum ok
	if(strcmp(header, CHECKSUM_OK_H) == 0) {
		//Check if it's a valid message
		if(*current_bytes < bytes) {
			return -1;
		} else {
			return 2;
		}
	}
	
	//Check file and checksum ko
	if(strcmp(header, FILE_KO_H) == 0 || strcmp(header, CHECKSUM_KO_H) == 0) {
		return -1;
	}
	
	//All ok
	return 1;

}

/***************************************************
* @Nombre: getMetadata
* @Def: Obtiene la metadata de un fichero
* @Arg:	In: type = tipo del fichero
*		In: bytes = tamaño del fichero
*		In: date = fecha de creacion del fichero
* @Ret: devuelve la metadata del fichero
***************************************************/
char * getMetadata(char type[3], long bytes, char * date) {
	
	char * metadata = (char *) malloc(MAX_BUFF);
	if(metadata == NULL) {
		return NULL;
	}
	
	int real_bytes = sprintf(metadata, METADATA, type, bytes, date);
	metadata = (char *) realloc(metadata, real_bytes + 1);
	metadata[real_bytes] = '\0';
	return metadata;

}

/************************************************
* @Nombre: getFileName
* @Def: Obtiene el nombre de un fichero
* @Arg:	In: basename = nombre base del fichero
*		In: type = tipo del fichero (txt o jpg)
* @Ret: devuelve el nombre del fichero
************************************************/
char * getFileName(char * basename, char type[4]) {

	//Get file name
	char * file_name = (char *) malloc(strlen(basename) + 5);
	if(file_name == NULL) {
		return NULL;
	}
	strcpy(file_name, basename);
	strcat(file_name, ".");
	strcat(file_name, type);
	return file_name;

}

/*********************************************************************************
* @Nombre: NET_sendData
* @Def: Envia al servidor la informacion precisada
* @Arg:	In: server_fd = file descriptor de la conexion
*		In: data = contenido del fichero
*		In: bytes = total de bytes de la informacion
*		In: date = fecha del fichero
*		In: type = tipo del fichero (txt o jpg)
*		In: path = carpeta donde se encuentra el fichero
*		In: turn_off = puntero al flag que indica si se quiere cerrar el cliente
* @Ret: devuelve 1 si ha ido bien, -1 si no se ha podido enviar el fichero o
*		el servidor se ha desconectado, -2 si falla el procesamiento de datos
*		y -3 si se quiere cerrar el cliente
*********************************************************************************/
int NET_sendData(int server_fd, char * data, long bytes, char * date, char type[4], char * path, int * turn_off) {
	
	struct NetPack pack;
	long current_bytes = 0;
	int respond;
	
	//Get metadata
	char * metadata = getMetadata(type, bytes, date);
	if(metadata == NULL) {
		return -2;
	}
	
	//Set up metadata package
	pack = NETPACK_makeNetPack(DATA_T, METADATA_HEADER, metadata, strlen(metadata));
	if(pack.length == -1) {
		free(metadata);
		return -2;
	}
	free(metadata);
	
	//Send package
	if(sendNetPack(server_fd, pack) < 0) {
		NETPACK_cleanPack(&pack);
		return -1;
	}
	
	//Get file name
	char * file_name = getFileName(date, type);
	if(file_name == NULL) {
		NETPACK_cleanPack(&pack);
		return -2;
	}
	
	//Server listening
	while(1) {
		
		//Get net pack
		NETPACK_cleanPack(&pack);
		pack = getNetPack(server_fd);
		if(pack.length == -1) {
			free(file_name);
			return -1;
		}
		
		//Check message type
		switch(pack.type) {
			
			case DISCONNECT_T:	//Server disconnected
				NETPACK_cleanPack(&pack);
				free(file_name);
				return -1;
				
			case DATA_T:	//Server data respond
				
				//Process response
				if(*turn_off == -1) {
					respond = dataRespondManagement(pack.header, &current_bytes, bytes, data, file_name, server_fd, path);
				} else {
					respond = -3;
				}
				
				//Check error
				if(respond < 0) {
					NETPACK_cleanPack(&pack);
					free(file_name);
					return respond;
				}
				
				//Check end of sending data
				if(respond == 2) {
					NETPACK_cleanPack(&pack);
					return 1;
				}
				
				break;
				
			default:	//Undefined respond
				NETPACK_cleanPack(&pack);
				free(file_name);
				return -1;
				
		}
		
	}

}

int NET_sendNoData(int server_fd) {

	struct NetPack pack;

	//Set up package
	pack = NETPACK_makeNetPack(NO_DATA_T, DEFAULT_HEADER, NULL, 0);
	if(pack.length == -1) {
		return -1;
	}
	
	//Send package
	if(sendNetPack(server_fd, pack) < 0) {
		NETPACK_cleanPack(&pack);
		return -1;
	}
	
	NETPACK_cleanPack(&pack);
	return 1;

}