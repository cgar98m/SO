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

#define KEY_FILE "makefile"
#define KEY 1

//Config
struct Config config;

//Server fd
int server_fd;

//Client list
struct FList client_list;

//Registers
char ** imgs_register;
int total_imgs;
char ** info_register;
int total_info;

//Semaphores
semaphore client_s;
semaphore thread_s;
semaphore imgs_s;
semaphore info_s;

//Message queue
int q_id;

/***********************************************************************************
* @Nombre: clientManagement
* @Def: Maneja la conexion con un cliente
* @Arg:	In: client = file descriptor de la conexion con el cliente
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal (establecimiento de conexion)
***********************************************************************************/
void * clientManagement(void * client) {
	
	int client_fd = *((int *) client);
	
	//Notify fd was caught
	SEM_signal(&thread_s);
	
	//Start connection
	char * telescope = NET_establishConnection(client_fd);
	if(telescope == NULL) {
		close(client_fd);
		SEM_wait(&client_s);
		FLIST_remove(&client_list, client_fd);
		SEM_signal(&client_s);
		return (void *) -1;
	}
	
	//Listen to the client
	int response;
	do {
		response = NET_listenToClient(client_fd, telescope, &imgs_register, &total_imgs, imgs_s, &info_register, &total_info, info_s, q_id);
	} while(response > 0);
	
	//Disconnect client
	if(response == -1) {
		MENU_disconnectOk(telescope);
	}
	free(telescope);
	
	//Close connection
	close(client_fd);
	SEM_wait(&client_s);
	FLIST_remove(&client_list, client_fd);
	SEM_signal(&client_s);
	
	//Wait for new interactions
	write(1, WAIT, strlen(WAIT));
	return (void *) 1;
	
}

/**************************************************************************************
* @Nombre: closeProgram
* @Def: Informa a los clientes que el servidor se esta cerrando y termina el programa
* @Arg:	---
* @Ret: ---
**************************************************************************************/
void closeProgram() {
	
	int client_fd;
	
	//Closing program message
	write(1, CLOSE_PROGRAM, strlen(CLOSE_PROGRAM));
	
	//Close server and clients
	if(server_fd >= 0) {
		
		//Close clients
		write(1, CLOSE_CLIENTS, strlen(CLOSE_CLIENTS));
		while(FLIST_empty(client_list) < 0) {
			FLIST_getFirst(client_list, &client_fd);
			NET_sendDisconnectMsg(client_fd, -1);
			close(client_fd);
			FLIST_remove(&client_list, client_fd);
		}
		
		//Close server
		close(server_fd);
		
	}

	//Notify dump to register
	if(total_imgs > 0) {
		write(1, DUMP_START, strlen(DUMP_START));
	}
	
	//Dump imgs register
	for(int i = 0; i < total_imgs; i++) {
		FILE_updateRegister(imgs_register[i]);
		free(imgs_register[i]);
	}
	free(imgs_register);
	
	//Free info register
	for(int i = 0; i < total_info; i++) {
		free(info_register[i]);
	}
	free(info_register);
	
	//Clean config
	CONFIG_cleanConfig(&config);
	
	//Destroy semaphores
	SEM_destructor(&client_s);
	SEM_destructor(&thread_s);
	SEM_destructor(&imgs_s);
	SEM_destructor(&info_s);
	
	//Destroy message queue
	msgctl(q_id, IPC_RMID, (struct msqid_ds *) NULL);
	
	//Exit program
	exit(EXIT_SUCCESS);
	
}

/**************************************
* @Nombre: lionel
* @Def: Inicializa el servidor Lionel
* @Arg:	---
* @Ret: ---
**************************************/
void lionel() {

	//Prepare server service
	struct sockaddr_in addr;
	write(1, START_SERVER, strlen(START_SERVER));
	server_fd = NET_openServer(config.ip, config.mcgruder_port, &addr);
	if(server_fd < 0) {
		write(1, SERVER_NOT_OPEN, strlen(SERVER_NOT_OPEN));
		raise(SIGINT);	//Close program
		pause();		//Wait for SIGINT
	}
	
	//Wait for new connections loop
	pthread_t t_id;
	int client_fd;
	write(1, WAIT, strlen(WAIT));
	while(1) {
		
		//Accept connections
		client_fd = NET_acceptConnection(server_fd, addr);
		
		//Check if connection is established
		if(client_fd < 0) {
			write(1, CONNECTION_ERROR, strlen(CONNECTION_ERROR));
			write(1, WAIT, strlen(WAIT));
		} else {
			
			//Add client to the client list
			SEM_wait(&client_s);
			FLIST_add(&client_list, client_fd);
			SEM_signal(&client_s);
			
			//Create dedicated server
			if(pthread_create(&t_id, NULL, clientManagement, (void *) &client_fd) != 0) {
				write(1, INTERNAL_ERROR, strlen(INTERNAL_ERROR));
			} else {
				//Wait to reassign client_fd
				SEM_wait(&thread_s);
			}
			
		}
		
	}

}

/**************************************
* @Nombre: lionel
* @Def: Inicializa el proceso Lionel
* @Arg:	---
* @Ret: ---
**************************************/
void paquita() {
	
	char buff[1024];
	int bytes;
	
	//Data --> F5: shared memory with semaphores
	long received_imgs = 0;
	long imgs_size = 0;
	long received_txts = 0;
	double const_avg = 0.0f;
	struct AstroData data = defaultData();
	
	//Listen to lionel messages
	struct NewDataMsg msg = defaultMsg();
	while(1) {
		
		//Prepare message
		cleanMsg(&msg);
		//Get message (blocking read)
		msgrcv(q_id, (struct msgbuf *) &msg, sizeof(msg) - sizeof(long), 1, 0);
		if(msg.id == -1) {
			continue;
		}
		
		//Process message
		if(strcmp(msg.type, "jpg") == 0) {
			received_imgs++;
			imgs_size += msg.bytes;
			bytes = sprintf(buff, "IMGS: %ld\nSIZE: %ld\nTXTS: %ld\nAVG CONSTELACIONS: %.2f\nCONST TOTALS: %ld\nAVG DENSITAT: %.3f\nMAX: %.2f\nMIN: %.2f\n",
			received_imgs, imgs_size, received_txts, const_avg, data.total_const, data.density_avg, data.max_mag, data.min_mag);
			write(1, buff, bytes);
		} else {
			if(FILE_getAstroData(msg.file_name, &data) > 0) {
				const_avg = (const_avg * received_txts + data.total_const) / (double) (received_txts + 1);
				received_txts++;
				bytes = sprintf(buff, "IMGS: %ld\nSIZE: %ld\nTXTS: %ld\nAVG CONSTELACIONS: %.2f\nCONST TOTALS: %ld\nAVG DENSITAT: %.3f\nMAX: %.2f\nMIN: %.2f\n",
				received_imgs, imgs_size, received_txts, const_avg, data.total_const, data.density_avg, data.max_mag, data.min_mag);
				write(1, buff, bytes);
			}
		}
		
	}
	
}

/********************************************************
* @Nombre: main
* @Def: Inicializa el servidor Lionel y Paquita
* @Arg:	In: argc = numero de parametros introducidos
*		In: argv = puntero a los parametros introducidos
* @Ret: devuelve 0 si ha ido bien y 1 si ha ido mal
********************************************************/
int main(int argc, char ** argv) {

	//Set default config
	CONFIG_defaultConfig(&config);
	server_fd = -1;
	FLIST_create(&client_list);
	imgs_register = NULL;
	total_imgs = 0;
	info_register = NULL;
	total_info = 0;
	q_id = -1;
	
	//Prepare semaphores
	SEM_constructor(&thread_s);
	SEM_init(&thread_s, 0);	//Synchro
	SEM_constructor(&client_s);
	SEM_init(&client_s, 1);	//Mutex
	SEM_constructor(&imgs_s);
	SEM_init(&imgs_s, 1);	//Mutex
	SEM_constructor(&info_s);
	SEM_init(&info_s, 1);	//Mutex
	
	//Rewrite SIGINT
	signal(SIGINT, closeProgram);
	
	//Check arguments
	if(argc != 2) {
		MENU_argError(argc < 2);
		raise(SIGINT);	//Close program
		pause();		//Wait for SIGINT
	}

	//Get file configuration
	if(FILE_readConfig(&config, argv[1]) < 0) {
		write(1, CONFIG_ERROR, strlen(CONFIG_ERROR));
		raise(SIGINT);	//Close program
		pause();		//Wait for SIGINT
	}
	
	//Prepare message queue for Paquita
	q_id = msgget(ftok(KEY_FILE, KEY), 0600 | IPC_CREAT);
	if(q_id == -1) {
		write(1, MSGQ_ERROR, strlen(MSGQ_ERROR));
		raise(SIGINT);	//Close program
		pause();		//Wait for SIGINT
	}
	
	//Create Paquita and start Lionel service
	int id = fork();
	if(id < 0) {
		raise(SIGINT);	//Close program
		pause();		//Wait for SIGINT
	} else {
		if(id > 0) {
			lionel();
		} else {
			paquita();
		}
	}

}