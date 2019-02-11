#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/msg.h>

#include "config.h"
#include "file.h"
#include "menu.h"
#include "net.h"
#include "flist.h"
#include "msg.h"

//Message queue data
#define KEY_FILE "makefile"
#define KEY 1

//Server
int server_fd;

//Registers
char ** imgs_register;
int total_imgs;
char ** info_register;
int total_info;

//Semaphores
semaphore thread_s;
semaphore imgs_s;
semaphore info_s;

//Message queue id
int q_id;

//Server mode
int turn_off;

/*******************************************
* @Nombre: closeProgram
* @Def: Prepara el servidor para su cierre
* @Arg:	---
* @Ret: ---
*******************************************/
void closeProgram() {
	turn_off = 1;
	if(server_fd != -1) {
		close(server_fd);
		server_fd = -1;
	}
	signal(SIGINT, closeProgram);
}

/**************************************************
* @Nombre: freeResources
* @Def: Libera los recursos globales del programa
* @Arg:	---
* @Ret: ---
**************************************************/
void freeResources() {
	
	//Free imgs resgister
	for(int i = 0; i < total_imgs; i++) {
		FILE_updateRegister(imgs_register[i]);
		free(imgs_register[i]);
	}
	if(imgs_register != NULL) {
		free(imgs_register);
	}
	
	//Free info resgister
	for(int i = 0; i < total_info; i++) {
		free(info_register[i]);
	}
	if(info_register != NULL) {
		free(info_register);
	}
	
	//Destroy semaphores
	SEM_destructor(&imgs_s);
	SEM_destructor(&info_s);
	SEM_destructor(&thread_s);
	
	//Destroy message queue
	msgctl(q_id, IPC_RMID, (struct msqid_ds *) NULL);
	
}

/**********************************************
* @Nombre: initSemaphores
* @Def: Inicializa los semaforos del servidor
* @Arg:	---
* @Ret: ---
**********************************************/
void initSemaphores() {
	SEM_constructor(&thread_s);
	SEM_init(&thread_s, 0);	//Synchro
	SEM_constructor(&imgs_s);
	SEM_init(&imgs_s, 1);	//Mutex
	SEM_constructor(&info_s);
	SEM_init(&info_s, 1);	//Mutex
}

/***********************************************************************************
* @Nombre: clientManagement
* @Def: Maneja la conexion con un cliente
* @Arg:	In: client = file descriptor de la conexion con el cliente
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal (establecimiento de conexion)
***********************************************************************************/
void * clientManagement(void * client) {
	
	//Get socket
	int client_fd = *((int *) client);
	SEM_signal(&thread_s);
	
	//Start connection
	char * telescope = NET_establishConnection(client_fd);
	if(telescope == NULL) {
		close(client_fd);
		return NULL;
	}
	
	//Listen to the client
	int response;
	do {
		response = NET_listenToClient(client_fd, telescope, &imgs_register, &total_imgs, imgs_s, &info_register, &total_info, info_s, q_id, &turn_off);
	} while(response > 0);
	
	//Disconnect client
	if(response == -1) {
		MENU_disconnectOk(telescope);
	}
	free(telescope);
	
	//Close connection
	close(client_fd);
	
	//Wait for new interactions
	write(1, WAIT, strlen(WAIT));
	return NULL;
	
}

/**************************************
* @Nombre: lionel
* @Def: Inicializa el servidor Lionel
* @Arg:	---
* @Ret: ---
**************************************/
void lionel(struct FList * t_list, struct Config config) {

	pthread_t t_id;
	int client_fd;

	//Prepare server service
	struct sockaddr_in addr;
	write(1, START_SERVER, strlen(START_SERVER));
	server_fd = NET_openServer(config.ip, config.mcgruder_port, &addr);
	if(server_fd < 0) {
		write(1, SERVER_NOT_OPEN, strlen(SERVER_NOT_OPEN));
		turn_off = 1;
	}
	
	//Wait for new connections loop
	write(1, WAIT, strlen(WAIT));
	while(turn_off < 0) {
		
		//Accept connections
		client_fd = NET_acceptConnection(server_fd, addr);
		
		//Check if connection is established
		if(client_fd < 0) {
			write(1, CONNECTION_ERROR, strlen(CONNECTION_ERROR));
			write(1, WAIT, strlen(WAIT));
		} else {
			
			//Create dedicated server
			if(pthread_create(&t_id, NULL, clientManagement, (void *) &client_fd) != 0) {
				write(1, INTERNAL_ERROR, strlen(INTERNAL_ERROR));
			} else {
				//Add thread id to the list
				FLIST_add(t_list, t_id);
				//Wait to dedicated server response
				SEM_wait(&thread_s);
			}
			
		}
		
	}
	
	//Wait for dedicated servers closing
	write(1, CLOSE_PROGRAM, strlen(CLOSE_PROGRAM));
	while(FLIST_empty(*t_list) < 0) {
		FLIST_getFirst(*t_list, &t_id);
		FLIST_remove(t_list, t_id);
		pthread_join(t_id, NULL);
	}

}

/**************************************
* @Nombre: paquita
* @Def: Inicializa el proceso Paquita
* @Arg:	---
* @Ret: ---
**************************************/
void paquita() {
	
	//Data --> F5: shared memory with semaphores
	long received_imgs = 0;
	long imgs_size = 0;
	long received_txts = 0;
	double const_avg = 0.0f;
	struct AstroData data = ASTRODATA_defaultData();
	
	//Listen to lionel messages
	struct NewDataMsg msg = MSG_defaultMsg();
	while(turn_off < 0) {
		
		//Prepare message
		MSG_cleanMsg(&msg);
		//Get message (blocking read)
		msgrcv(q_id, (struct msgbuf *) &msg, sizeof(struct NewDataMsg) - sizeof(long), 1, 0);
		if(msg.id == -1) {
			break;
		}
		
		//Process message
		if(strcmp(msg.type, "jpg") == 0) {
			received_imgs++;
			imgs_size += msg.bytes;
		} else {
			if(FILE_getAstroData(msg.file_name, &data) > 0) {
				const_avg = (const_avg * received_txts + data.total_const) / (double) (received_txts + 1);
				received_txts++;
			}
		}
		
	}
	
	//Show data
	char buff[1024];
	int bytes = sprintf(buff, "IMGS: %ld\nSIZE: %ld\nTXTS: %ld\nAVG CONSTELACIONS: %.2f\nCONST TOTALS: %ld\nAVG DENSITAT: %.3f\nMAX: %.2f\nMIN: %.2f\n",
	received_imgs, imgs_size, received_txts, const_avg, data.total_const, data.density_avg, data.max_mag, data.min_mag);
	write(1, buff, bytes);
	
}

/********************************************************
* @Nombre: main
* @Def: Inicializa el servidor Lionel y Paquita
* @Arg:	In: argc = numero de parametros introducidos
*		In: argv = puntero a los parametros introducidos
* @Ret: devuelve 0 si ha ido bien y 1 si ha ido mal
********************************************************/
int main(int argc, char ** argv) {

	//Rewrite SIGINT
	turn_off = -1;
	server_fd = -1;
	signal(SIGINT, closeProgram);
	
	//Check arguments
	if(argc != 2) {
		MENU_argError(argc < 2);
		exit(EXIT_FAILURE);
	}
	
	//Get file configuration
	struct Config config;
	if(FILE_readConfig(&config, argv[1]) < 0) {
		write(1, CONFIG_ERROR, strlen(CONFIG_ERROR));
		exit(EXIT_FAILURE);
	}
	
	//Create thread list for lionel
	struct FList t_list;
	if(FLIST_create(&t_list) < 0) {
		write(1, LIST_ERROR, strlen(LIST_ERROR));
		CONFIG_cleanConfig(&config);
		exit(EXIT_FAILURE);
	}
	
	//Prepare message queue for Paquita
	q_id = msgget(ftok(KEY_FILE, KEY), 0600 | IPC_CREAT);
	if(q_id == -1) {
		write(1, MSGQ_ERROR, strlen(MSGQ_ERROR));
		CONFIG_cleanConfig(&config);
		FLIST_destroy(&t_list);
		exit(EXIT_FAILURE);
	}
	
	//Init registers
	imgs_register = NULL;
	total_imgs = 0;
	info_register = NULL;
	total_info = 0;
	
	//Prepare semaphores
	initSemaphores();
	
	//Create Paquita and start Lionel service
	int id = fork();
	if(id < 0) {
		freeResources();
		CONFIG_cleanConfig(&config);
		FLIST_destroy(&t_list);
	} else {
		if(id > 0) {
			
			//Lionel server
			lionel(&t_list, config);
			
			//Free data
			CONFIG_cleanConfig(&config);
			FLIST_destroy(&t_list);
			freeResources();
			
			//Wait for paquita
			int status;
			waitpid(id, &status, 0);
			
		} else {
			//Paquita process
			paquita();
			CONFIG_cleanConfig(&config);
			FLIST_destroy(&t_list);
		}
	}

	//End of program
	exit(EXIT_SUCCESS);
	
}