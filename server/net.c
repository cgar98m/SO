#include "net.h"

/*****************************************************************************
* @Nombre: NET_openServer
* @Def: Abre un socket para escuchar las peticiones de los possibles clientes
* @Arg:	In: ip = IP del servidor
*		In: port = puerto del servidor
*		Out: addr = informacion de la conexion del servidor
* @Ret: devuelve el socket de escucha del servidor (-1 si ha ido mal)
*****************************************************************************/
int NET_openServer(char * ip, int port, struct sockaddr_in * addr) {

	int fd = 0;
	
	//Open socket
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(fd < 0) {
		return -1;
	}
	
	//Prepare struct
	memset(addr, 0, sizeof(*addr));
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	
	//Set ip
	if(inet_aton(ip, &addr->sin_addr) == 0) {
		close(fd);
		return -1;
	}
	
	//Bind socket
	if(bind(fd, (void *) addr, sizeof(*addr)) < 0) {
		close(fd);
		return -1;
	}
	
	//All ok
	return fd;

}

/************************************************************************************
* @Nombre: NET_acceptConnection
* @Def: Escucha a un socket para aceptar solicitudes de conexion
* @Arg:	In: fd = socket a escuchar
*		In: addr = informacion de la conexion
* @Ret: devuelve el file descriptor de la conexion con el cliente (-1 si ha ido mal)
************************************************************************************/
int NET_acceptConnection(int fd, struct sockaddr_in addr) {
	
	//Listen to fd
	if(listen(fd, MAX_CONNECTIONS) != 0) {
		return -1;
	}
	
	//Accept connection
	socklen_t length = sizeof(addr);
	return accept(fd, (void *) &addr, &length);
	
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

	//Send length
	if(write(fd, &pack.length, sizeof(int)) <= 0) {
		return -1;
	}

	//Check if need to send data
	if(pack.length == 0) {
		return 1;
	}

	//Send data
	if(write(fd, pack.data, pack.length) <= 0) {
		return -1;
	}
	
	return 1;

}

/*******************************************************************
* @Nombre: NET_sendDisconnectMsg
* @Def: Envia un paquete al cliente para informar de su desconexion
* @Arg:	In: fd = file descriptor de la conexion
*		In: type = tipus de paquet (1 = OK, -1 = KO)
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
*******************************************************************/
int NET_sendDisconnectMsg(int fd, int type) {
	
	struct NetPack reply;
	
	//Prepare reply
	if(type > 0) {
		reply = NETPACK_makeNetPack(DISCONNECT_T, CONNECT_OK_H, NULL);
	} else {
		reply = NETPACK_makeNetPack(DISCONNECT_T, CONNECT_KO_H, NULL);
	}
	
	if(reply.length == -1) {
		return -1;
	}
	
	//Send reply
	if(sendNetPack(fd, reply) < 0) {
		NETPACK_cleanPack(&reply);
		return -1;
	}
	
	//Clean net pack
	NETPACK_cleanPack(&reply);
	return 1;
	
}

/*****************************************************************
* @Nombre: connectManagement
* @Def: Envia un paquete al cliente para contestar a su conexion
* @Arg:	In: fd = file descriptor de la conexion
*		In: type = tipus de paquet (1 = OK, -1 = KO)
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
*****************************************************************/
int connectManagement(int fd, int type) {
	
	struct NetPack reply;
	
	//Prepare reply
	if(type > 0) {
		reply = NETPACK_makeNetPack(CONNECT_T, CONNECT_OK_H, NULL);
	} else {
		reply = NETPACK_makeNetPack(CONNECT_T, CONNECT_KO_H, NULL);
	}
	
	if(reply.length == -1) {
		return -1;
	}
	
	//Send reply
	if(sendNetPack(fd, reply) < 0) {
		NETPACK_cleanPack(&reply);
		return -1;
	}
	
	//Clean net pack
	NETPACK_cleanPack(&reply);
	return 1;
	
}

/*************************************************************
* @Nombre: readByteUntilFromFd
* @Def: Lee byte a byte de un fd hasta encontrar un delimiter
* @Arg:	In: fd = file descriptor a leer
*		In: delimiter = delimitador del mensage
* @Ret: devuelve los bytes leidos (NULL si hay error)
*************************************************************/
char * readByteUntilFromFd(int fd, char delimiter) {

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
			perror("E2");
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
	
	//Check type
	if(read(fd, &pack.type, 1) <= 0) {
		return pack;
	}
	
	//Get header
	pack.header = readByteUntilFromFd(fd, ']');
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
		pack.length = -2;
		return pack;
	}
	
	//Get data
	if(read(fd, pack.data, pack.length) <= 0) {
		NETPACK_cleanPack(&pack);
	}
	
	return pack;

}

/************************************************************
* @Nombre: NET_establishConnection
* @Def: Escucha a un cliente a la espera de su presentacion
* @Arg:	In: fd = cliente a escuchar
* @Ret: devuelve el nombre del cliente
************************************************************/
char * NET_establishConnection(int fd) {

	struct NetPack pack;
	
	//Get net pack
	NETPACK_defaultPack(&pack);
	pack = getNetPack(fd);
	
	//Check net pack
	if(pack.length < 0) {
		write(1, INTERNAL_ERROR, strlen(INTERNAL_ERROR));
		if(pack.length < -1) {
			connectManagement(fd, -1);
		}
		return NULL;
	}
	
	//Check if is a valid package
	if(pack.type != CONNECT_T) {
		write(1, COMMUNICATION_ERROR, strlen(COMMUNICATION_ERROR));
		NETPACK_cleanPack(&pack);
		connectManagement(fd, -1);
		return NULL;
	}

	//Get client name
	char * telescope = (char *) malloc(pack.length + 1);
	if(telescope == NULL) {
		write(1, INTERNAL_ERROR, strlen(INTERNAL_ERROR));
		NETPACK_cleanPack(&pack);
		connectManagement(fd, -1);
		return NULL;
	}
	strcpy(telescope, pack.data);
	NETPACK_cleanPack(&pack);

	//Send connect reply
	if(connectManagement(fd, 1) < 0) {
		MENU_connectKo(telescope);
		free(telescope);
		return NULL;
	}
	
	//Successful connection
	MENU_connectOk(telescope);
	write(1, WAIT, strlen(WAIT));
	return telescope;

}

/*************************************************************
* @Nombre: sendDataMsg
* @Def: Notifica al cliente sobre el envio de un fichero
* @Arg:	In: fd = file descriptor
*		In: header = cabecera del paquete
* @Ret: devuelve 1 si ha ido bien y -1 si ha habido un error
*************************************************************/
int sendDataMsg(int fd, char * header) {
	
	struct NetPack reply;
	
	//Prepare reply
	reply = NETPACK_makeNetPack(DATA_T, header, NULL);
	if(reply.length == -1) {
		return -1;
	}
	
	//Send reply
	if(sendNetPack(fd, reply) < 0) {
		NETPACK_cleanPack(&reply);
		return -1;
	}

	//Clean net pack
	NETPACK_cleanPack(&reply);
	return 1;
	
}

/***************************************************************************************
* @Nombre: getChecksumData
* @Def: Obtiene la informacion del checksum y comprueba si ha recibido bien el fichero
* @Arg:	In: fd = file descriptor
*		In/Out: s_config = configuracion del envio
*		In: data = informacion del envio
*		In: length = longitud de la informacion
* @Ret: devuelve 1 si ha ido bien y -1 si ha habido un error
***************************************************************************************/
int getChecksumData(int fd, struct SendConfig * s_config, char * data, long length) {

	//Prepare message md5sum
	char * msg_checksum = (char * ) malloc(length + 1);
	if(msg_checksum == NULL) {
		write(1, INTERNAL_ERROR, strlen(INTERNAL_ERROR));
		sendDataMsg(fd, CHECKSUM_KO_H);
		return -1;
	}
	
	//Get message md5sum
	int i;
	for(i = 0; i < length; i++) {
		msg_checksum[i] = data[i];
	}
	msg_checksum[i] = '\0';
	
	//Dump data to file
	if(FILE_createFile(s_config->file, s_config->data, s_config->size) < 0) {
		write(1, INTERNAL_ERROR, strlen(INTERNAL_ERROR));
		free(msg_checksum);
		sendDataMsg(fd, CHECKSUM_KO_H);
		return -1;
	}
	
	//Get file md5sum
	char * checksum = FILE_checksum(s_config->file);
	if(checksum == NULL) {
		write(1, INTERNAL_ERROR, strlen(INTERNAL_ERROR));
		free(msg_checksum);
		sendDataMsg(fd, CHECKSUM_KO_H);
		return -1;
	}
	
	//Compare md5sums
	if(strcmp(checksum, msg_checksum) != 0) {
		write(1, INTERNAL_ERROR, strlen(INTERNAL_ERROR));
		free(checksum);
		free(msg_checksum);
		sendDataMsg(fd, CHECKSUM_KO_H);
		return -1;
	}
	
	//Checksum ok, file received
	MENU_receivedFile(s_config->file);
	free(checksum);
	free(msg_checksum);
	sendDataMsg(fd, CHECKSUM_OK_H);
	write(1, WAIT, strlen(WAIT));
	return 1;

}

/*******************************************************************
* @Nombre: getFileData
* @Def: Añade informacion a la configuracion de envio y pide mas
* @Arg:	In: fd = file descriptor
*		In/Out: s_config = configuracion del envio
*		In: data = informacion del envio
*		In: length = longitud de la informacion
* @Ret: devuelve 1 si ha ido bien y -1 si ha habido un error
*******************************************************************/
int getFileData(int fd, struct SendConfig * s_config, char * data, long length) {

	//Dump data
	for(long i = 0; i < length; i++) {
		s_config->data[s_config->current_size + i] = data[i];
	}
	s_config->current_size += length;
	MENU_showPercent(s_config->file, (float) (s_config->current_size / (double) s_config->size) * 100);

	//Send response
	if(sendDataMsg(fd, FILE_OK_H) < 0) {
		write(1, COMMUNICATION_ERROR, strlen(COMMUNICATION_ERROR));
		return -1;
	}
	
	//All ok
	return 1;

}

/*******************************************************************
* @Nombre: receiveData
* @Def: Recibe la informacion de un fichero enviado por un cliente
* @Arg:	In: fd = file descriptor
*		In/Out: s_config = configuracion del envio
* @Ret: devuelve 1 si ha ido bien y -1 si ha habido un error
*******************************************************************/
int receiveData(int fd, struct SendConfig * s_config) {
	
	struct NetPack pack;
	
	//Prepare reply
	pack = getNetPack(fd);
	if(pack.length < 0 || pack.type != DATA_T) {
		write(1, COMMUNICATION_ERROR, strlen(COMMUNICATION_ERROR));
		if(pack.length < -1) {
			sendDataMsg(fd, FILE_KO_H);
		}
		return -1;
	}
	
	//More data from file
	if(strcmp(pack.header, DEFAULT_HEADER) == 0) {
		
		int response = getFileData(fd, s_config, pack.data, pack.length);
		NETPACK_cleanPack(&pack);

		//Check response
		if(response < 0) {
			return -1;
		} else {
			return receiveData(fd, s_config);
		}
		
	}
	
	//End of data
	if(strcmp(pack.header, ENDFILE_HEADER) == 0) {
		int response = getChecksumData(fd, s_config, pack.data, pack.length);
		NETPACK_cleanPack(&pack);
		return response;
	}
	
	//Other kind of data
	NETPACK_cleanPack(&pack);
	sendDataMsg(fd, FILE_KO_H);
	return -1;
	
}

/*********************************************************************
* @Nombre: getFileName
* @Def: Obtiene la informacion desde una posicion hasta un caracter
* @Arg:	In: date = fecha de creacion del fichero
*		In: type = tipo del fichero
* @Ret: devuelve el nombre del fichero (NULL si hay un error)
*********************************************************************/
char * getFileName(char * date, char type[4]) {

	//Get file name
	char * file_name = (char *) malloc(strlen(date) + 5);
	if(file_name == NULL) {
		return NULL;
	}
	
	//Add info
	file_name[0] = '\0';
	strcpy(file_name, date);
	strcat(file_name, ".");
	strcat(file_name, type);
	return file_name;

}

/************************************************************************
* @Nombre: readByteFromUntil
* @Def: Obtiene la informacion desde una posicion hasta un caracter
* @Arg:	In: buff = configuracion del envio
*		In: index = indice de comienzo
*		In: delimiter = caracter que delimita el fin de la informacion
* @Ret: devuelve la informacion recortada (NULL si hay un error)
************************************************************************/
char * readByteFromUntil(char * buff, int index, char delimiter) {

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
		aux = buff[index];
		index++;
		
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

/******************************************************************
* @Nombre: readByteFromTo
* @Def: Obtiene la informacion en un rango establecido
* @Arg:	In: buff = configuracion del envio
*		In: start = comienzo
*		In: length = longitud de la informacion
* @Ret: devuelve la informacion recortada (NULL si hay un error)
******************************************************************/
char * readByteFromTo(char * buff, int start, int length) {
	
	char * data;

	//Get memory
	data = (char *) malloc(sizeof(char) * (length - start + 1));
	if(data == NULL) {
		return NULL;
	}
	
	//Get range of data
	int j = 0;
	for(int i = start; i < length; i++) {
		data[j] = buff[i];
		j++;
	}
	data[j] = '\0';
	return data;
	
}

/***************************************************************************************
* @Nombre: getSendConfig
* @Def: Obtiene la configuracion de envio de un fichero a partir de cierta informacion
* @Arg:	In/Out: s_config = configuracion del envio
*		In: data = informacion
*		In: length = longitud de la informacion
* @Ret: devuelve 1 si ha ido bien, -1 si ha ido mal
***************************************************************************************/
int getSendConfig(struct SendConfig ** s_config, char * data, int length) {
	
	//Prepare send config
	*s_config = (struct SendConfig *) malloc(sizeof(struct SendConfig));
	if(*s_config == NULL) {
		return -1;
	}
	SENDCONFIG_defaultConfig(*s_config);
	
	//Get file type
	for(int i = 0; i < 3; i++) {
		(*s_config)->type[i] = data[i];
	}
	(*s_config)->type[3] = '\0';

	//Get file length
	char * aux = readByteFromUntil(data, 4, '&');
	(*s_config)->size = atol(aux);
	free(aux);
	if((*s_config)->size == 0) {
		SENDCONFIG_cleanConfig(*s_config);
		return -1;
	}
	
	//Prepare data
	(*s_config)->data = (char *) malloc((*s_config)->size);
	if((*s_config)->data == NULL) {
		SENDCONFIG_cleanConfig(*s_config);
		return -1;
	}
	
	//Get file creation date
	aux = readByteFromTo(data, length - 15, length);
	if(aux == NULL) {
		SENDCONFIG_cleanConfig(*s_config);
		return -1;
	}
	
	//Get file name
	(*s_config)->file = getFileName(aux, (*s_config)->type);
	if((*s_config)->file == NULL) {
		SENDCONFIG_cleanConfig(*s_config);
		free(aux);
		return -1;
	}
	free(aux);
	
	//All ok
	return 1;
	
}

/**********************************************************
* @Nombre: prepareRegister
* @Def: Anade una nueva entrada al registro especificado
* @Arg:	In/Out: reg = registro
*		In/Out: total = numero de filas del registro
*		In: size = informacion del registro
* @Ret: devuelve 1 si ha ido bien, -1 si ha ido mal
**********************************************************/
int prepareRegister(char *** reg, int * total, long size) {

	//Prepare time params
	time_t t;
	struct tm * time_info;

	//Get time
	time(&t);
	time_info = localtime(&t);

	//Get register row
	char buff[MAX_ROW];
	sprintf(buff, REGISTER_ROW, time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, size);
	
	//Prepare register row to register
	char * register_row = (char *) malloc(strlen(buff) + 1);
	if(register_row == NULL) {
		return -1;
	}
	strcpy(register_row, buff);
	
	//Get register memory
	char ** new_register;
	if(*total == 0) {
		new_register = (char **) malloc(sizeof(char *));
	} else {
		new_register = (char **) realloc(*reg, sizeof(char *) * (*total + 1));
	}
	
	//Check memory request
	if(new_register == NULL) {
		free(register_row);
		return -1;
	}
	*reg = new_register;
	
	//Attach register row
	new_register[*total] = register_row;
	(*total)++;
	return 1;
	
}

/********************************************************************
* @Nombre: dataManagement
* @Def: Procesa la recepcion de un fichero
* @Arg:	In: fd = file descriptor
*		In: pack = paquete a tratar
*		In: telescope = nombre del cliente
*		In/Out: imgs_register = registro de las imagenes recibidas
*		In/Out: total_imgs = total de imagenes recibidas
*		In: imgs_s = semaforo de las imagenes
*		In/Out: info_register = registro de la informacion recibida
*		In/Out: total_info = total de ficheros txt recibidos
*		In: info_s = semaforo de los ficheros de texto
*		In: q_id = id de la cola de mensages
* @Ret: devuelve 1 si ha ido bien y -1 si ha habido un error
********************************************************************/
int dataManagement(int fd, struct NetPack pack, char * telescope, char *** imgs_register, int * total_imgs, semaphore imgs_s, char *** info_register, int * total_info, semaphore info_s, int q_id) {

	//Check pack header
	if(strcmp(pack.header, METADATA_HEADER) != 0) {
		write(1, COMMUNICATION_ERROR, strlen(COMMUNICATION_ERROR));
		sendDataMsg(fd, FILE_KO_H);
		return -1;
	}
	
	//Get send configuration
	struct SendConfig * s_config;
	if(getSendConfig(&s_config, pack.data, pack.length) < 0) {
		write(1, INTERNAL_ERROR, strlen(INTERNAL_ERROR));
		sendDataMsg(fd, FILE_KO_H);
		return -1;
	}

	//Comunicate file send started and send response
	MENU_receivingFile(telescope);
	
	//Send response
	if(sendDataMsg(fd, FILE_OK_H) < 0) {
		SENDCONFIG_cleanConfig(s_config);
		free(s_config);
		return -1;
	}
	
	//Receive data
	int response = receiveData(fd, s_config);
	if(response > 0) {	//Update register params
		if(strcmp(s_config->type, "txt") == 0) {
			SEM_wait(&info_s);
			prepareRegister(info_register, total_info, s_config->size);
			SEM_signal(&info_s);
			struct NewDataMsg msg = newMsg(1, "txt", s_config->size, s_config->file);
			msgsnd(q_id, (struct msgbuf *) &msg, sizeof(msg) - sizeof(long), IPC_NOWAIT);
		}
		if(strcmp(s_config->type, "jpg") == 0) {
			SEM_wait(&imgs_s);
			prepareRegister(imgs_register, total_imgs, s_config->size);
			SEM_signal(&imgs_s);
			struct NewDataMsg msg = newMsg(1, "jpg", s_config->size, s_config->file);
			msgsnd(q_id, (struct msgbuf *) &msg, sizeof(msg) - sizeof(long), IPC_NOWAIT);
		}
	}
	free(s_config);
	return response;

}

/*********************************************************************************************
* @Nombre: NET_listenToClient
* @Def: Escucha a un cliente y realiza las acciones pertinentes
* @Arg:	In: fd = cliente a escuchar
*		In: telescope = nombre del tescopio
*		In/Out: imgs_register = registro de las imagenes recibidas
*		In/Out: total_imgs = total de imagenes recibidas
*		In: imgs_s = semaforo de las imagenes
*		In/Out: info_register = registro de la informacion recibida
*		In/Out: total_info = total de ficheros txt recibidos
*		In: info_s = semaforo de los ficheros de texto
*		In: q_id = id de la cola de mensages
* @Ret: devuelve 1 si ha ido bien, -1 si ha ido mal y -2 si se desea desconectar el cliente
*********************************************************************************************/
int NET_listenToClient(int fd, char * telescope, char *** imgs_register, int * total_imgs, semaphore imgs_s, char *** info_register, int * total_info, semaphore info_s, int q_id) {
	
	struct NetPack pack;
	
	//Get net pack
	NETPACK_defaultPack(&pack);
	pack = getNetPack(fd);
	
	//Check net pack
	if(pack.length < 0) {
		write(1, INTERNAL_ERROR, strlen(INTERNAL_ERROR));
		if(pack.length < -1) {	
			NET_sendDisconnectMsg(fd, -1);
		}
		return -1;
	}
	
	//Recognize net pack type
	int response;
	switch(pack.type) {
			
		case DISCONNECT_T:	//Disconnect request
			if(NET_sendDisconnectMsg(fd, 1) < 0) {
				MENU_disconnectKo(telescope);
			} else {
				MENU_disconnectOk(telescope);
			}
			NETPACK_cleanPack(&pack);
			return -2;

		case DATA_T:	//Send file request
			response = dataManagement(fd, pack, telescope, imgs_register, total_imgs, imgs_s, info_register, total_info, info_s, q_id);
			NETPACK_cleanPack(&pack);
			return response;
			
		default:		//Undefined request
			write(1, COMMUNICATION_ERROR, strlen(COMMUNICATION_ERROR));
			NETPACK_cleanPack(&pack);
			connectManagement(fd, -1);
			return -1;
			
	}
	
}