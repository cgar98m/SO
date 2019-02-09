#ifndef _NET_H
#define _NET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/msg.h>

#include "menu.h"
#include "netpack.h"
#include "sendconfig.h"
#include "file.h"
#include "semaphore.h"
#include "msg.h"

//Max connections
#define MAX_CONNECTIONS 10

//Register format
#define REGISTER_ROW "%.4d-%.2d-%.2d %.2d:%.2d %ld bytes\n"

//Register max buffer
#define MAX_ROW 128

//NetPackage
struct NetPackage {
	char type;
	char * header;
	long length;
	char * data;
};

/*****************************************************************************
* @Nombre: NET_openServer
* @Def: Abre un socket para escuchar las peticiones de los possibles clientes
* @Arg:	In: ip = IP del servidor
*		In: port = puerto del servidor
*		Out: addr = informacion de la conexion del servidor
* @Ret: devuelve el socket de escucha del servidor (-1 si ha ido mal)
*****************************************************************************/
int NET_openServer(char * ip, int port, struct sockaddr_in * addr);

/************************************************************************************
* @Nombre: NET_acceptConnection
* @Def: Escucha a un socket para aceptar solicitudes de conexion
* @Arg:	In: fd = socket a escuchar
*		In: addr = informacion de la conexion
* @Ret: devuelve el file descriptor de la conexion con el cliente (-1 si ha ido mal)
************************************************************************************/
int NET_acceptConnection(int fd, struct sockaddr_in addr);

/*******************************************************************
* @Nombre: NET_sendDisconnectMsg
* @Def: Envia un paquete al cliente para informar de su desconexion
* @Arg:	In: fd = file descriptor de la conexion
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
*******************************************************************/
int NET_sendDisconnectMsg(int fd, int type);

/************************************************************
* @Nombre: NET_establishConnection
* @Def: Escucha a un cliente a la espera de su presentacion
* @Arg:	In: fd = cliente a escuchar
* @Ret: devuelve el nombre del cliente
************************************************************/
char * NET_establishConnection(int fd);

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
int NET_listenToClient(int fd, char * telescope, char *** imgs_register, int * total_imgs, semaphore imgs_s, char *** info_register, int * total_info, semaphore info_s, int q_id);

#endif