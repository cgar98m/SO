#ifndef _NET_H
#define _NET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>

#include "netpack.h"
#include "file.h"

//Buff size
#define MAX_BUFF 128

//Metadata structure
#define METADATA "%s&%ld&%s"

//Max data to send in a socket
#define DATA_LENGTH 4096

/********************************************************
* @Nombre: NET_establishConnection
* @Def: Establece la conexion con el servidor
* @Arg:	In: ip = direccion ip del servidor
*		In: port = puerto del servidor
*		Out: server_fd = file descriptor de la conexion
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
********************************************************/
int NET_establishConnection(char * ip, int port, int * server_fd);

/******************************************************
* @Nombre: NET_connectRequest
* @Def: Notifica al servidor del cliente conectado
* @Arg:	In: server_fd = file descriptor de la conexion
*		In: telescope = nombre del telescopio cliente
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
******************************************************/
int NET_connectRequest(int server_fd, char * telescope);

/******************************************************
* @Nombre: NET_connectReply
* @Def: Espera la respuesta de conexion del servidor
* @Arg:	In: server_fd = file descriptor de la conexion
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
******************************************************/
int NET_connectReply(int server_fd);

/**********************************************************
* @Nombre: NET_disconnectRequest
* @Def: Notifica al servidor de la desconexion del cliente
* @Arg:	In: server_fd = file descriptor de la conexion
*		In: telescope = nombre del cliente
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
**********************************************************/
int NET_disconnectRequest(int server_fd, char * telescope);

/********************************************************
* @Nombre: NET_disconnectReply
* @Def: Espera la respuesta de desconexion del servidor
* @Arg:	In: server_fd = file descriptor de la conexion
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
********************************************************/
int NET_disconnectReply(int server_fd);

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
int NET_sendData(int server_fd, char * data, long bytes, char * date, char type[4], char * path, int * turn_off);

int NET_sendNoData(int server_fd);

#endif