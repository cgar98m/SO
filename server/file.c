#include "file.h"

/************************************************************
* @Nombre: readCharUntil
* @Def: Lee de un fd hasta que se encuentra con el delimiter
* @Arg:	In: fd = file descriptor a leer
*		In: delimiter = caracter que delimita la lectura
* @Ret: devuelve el string leido (NULL si da error)
************************************************************/
char * readCharUntil(int fd, char delimiter) {

	char * data;
	char * new_data;
	char aux;
	int bytes = 0;

	//Get memory
	data = (char *) malloc(sizeof(char));
	if(data == NULL) {
		return NULL;
	}
	
	//Read data until delimiter is found
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
			
		}
		
	} while(aux != delimiter);
	
	//Replace delimiter
	data[bytes] = '\0';
	
	//Check if has '\r'
	if(bytes > 0 && data[bytes - 1] == '\r') {
		
		//Replace '\r'
		data[bytes - 1] = '\0';
		
		//Resize memory
		new_data = (char *) realloc(data, bytes);
		if(new_data == NULL) {
			free(data);
			return NULL;
		} else {
			data = new_data;
		}
		
	}

	return data;
	
}

/****************************************************************
* @Nombre: readIntUntil
* @Def: Lee de un fd hasta que se encuentra con el delimiter
* @Arg:	In: fd = file descriptor a leer
*		In: delimiter = caracter que delimita la lectura
* @Ret: devuelve el entero leido (si da error o 0, devuelve -1)
****************************************************************/
int readIntUntil(int fd, char delimiter) {

	char * data;
	int number;
	
	//Get data as char array
	data = readCharUntil(fd, delimiter);
	if(data == NULL) {
		return -1;
	}
	
	//Get int value
	number = atoi(data);
	free(data);
	if(number == 0) {
		return -1;
	}
	
	return number;
	
}

/***************************************************************
* @Nombre: FILE_readConfig
* @Def: Obtiene la configuracion de un fichero de configuracion
* @Arg:	Out: config = configuracion vacia
*		In: file_name = nombre del fichero
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
***************************************************************/
int FILE_readConfig(struct Config * config, char * file_name) {

	int fd;
	
	//Open file
	fd = open(file_name, O_RDONLY);
	if(fd < 0) {
		return -1;
	}
	
	//Get ip
	config->ip = readCharUntil(fd, '\n');
	if(config->ip == NULL) {
		close(fd);
		return -1;
	}
	
	//Get McGruder port
	config->mcgruder_port = readIntUntil(fd, '\n');
	if(config->mcgruder_port == -1) {
		CONFIG_cleanConfig(config);
		close(fd);
		return -1;
	}
	
	//Get McTravish port
	config->mctravish_port = readIntUntil(fd, '\n');
	if(config->mctravish_port == -1) {
		CONFIG_cleanConfig(config);
		close(fd);
		return -1;
	}
	
	//Get param
	config->param = readIntUntil(fd, '\n');
	if(config->param == -1) {
		CONFIG_cleanConfig(config);
		close(fd);
		return -1;
	}
	
	//Close file
	close(fd);
	return 1;

}

/**************************************************
* @Nombre: md5sum
* @Def: Obtiene el md5sum de un file descriptor
* @Arg:	In: fd = configuracion vacia
*		In: file_name = nombre del fichero
* @Ret: devuelve el md5sum (NULL si hay un error)
**************************************************/
char * md5sum(int fd) {
	
	char aux;
	int bytes = 0;
	char * new_data;
	char * data;

	//Get memory
	data = (char *) malloc(sizeof(char));
	if(data == NULL) {
		return NULL;
	}
	
	//Read data until blanckspace
	do {
		
		//Read char
		if(read(fd, &aux, sizeof(char)) != 1) {
			free(data);
			return NULL;
		}
		
		//Check if blanckspace
		if(aux != ' ') {
		
			//Add char
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
		
	} while(aux != ' ');
	
	//Delimite data
	data[bytes] = '\0';
	return data;
	
}

/****************************************************
* @Nombre: FILE_checksum
* @Def: Obtiene el checksum de un fichero
* @Arg:	In: file_name = nombre del fichero
* @Ret: devuelve el checksum (NULL si hay un error)
****************************************************/
char * FILE_checksum(char * file_name) {

	char full_path[MAX_PATH];
	char * checksum;
	
	//Get full path
	strcpy(full_path, FILE_PATH);
	strcat(full_path, file_name);
	
	int fd_pipe[2];

	//Crear pipe
	if(pipe(fd_pipe) != 0) {
		return NULL;
	}

	//Fork to md5sum
	pid_t p_fork = fork();
	if (p_fork > 0) {
	
		//Close write fd (read only)
		close(fd_pipe[1]);
		
		//Get checksum
		checksum = md5sum(fd_pipe[0]);
		close(fd_pipe[0]);
		if(checksum == NULL) {
			wait(&p_fork);
			return NULL;
		}

		//Wait to child
		wait(&p_fork);

	} else {
		
		//Check if the fork generated an error
		if(p_fork == 0) {

			//Close read fd (write only)
			close(fd_pipe[0]);

			//Redirect default wirte fd
			dup2(fd_pipe[1], 1);

			//Execute md5sum
			char * args[] = {"md5sum", full_path, NULL};
			execvp(args[0], args);

			//Only if an error occurs
			close(fd_pipe[1]);
			return NULL;

		} else {	//Error
			close(fd_pipe[0]);
			close(fd_pipe[1]);
			return NULL;
		}
	
	}

	return checksum;

}

/*****************************************************************
* @Nombre: FILE_createFile
* @Def: Crea un fichero a partir de la informacion proporcionada
* @Arg:	In: file_name = nombre del fichero
*		In: data = informacion
*		In: bytes = longitud de la informacion
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
*****************************************************************/
int FILE_createFile(char * file_name, char * data, long bytes) {
	
	int fd;
	char full_path[MAX_PATH];
	
	//Get full path
	strcpy(full_path, FILE_PATH);
	strcat(full_path, file_name);
	
	//Open file
	fd = open(full_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd < 0) {
		return -1;
	}
	
	//Add data
	if(write(fd, data, bytes) < bytes) {
		close(fd);
		return -1;
	}
	
	//Close file
	close(fd);
	return 1;
	
}

/************************************************************
* @Nombre: FILE_updateRegister
* @Def: Anade informacion al registro llamado REGISTER_FILE
* @Arg:	In: info = informacion nueva
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
************************************************************/
int FILE_updateRegister(char * info) {
	
	int fd;
	char full_path[MAX_PATH];
	
	//Get full path
	strcpy(full_path, FILE_PATH);
	strcat(full_path, REGISTER_FILE);
	
	//Open file
	fd = open(full_path, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if(fd < 0) {
		return -1;
	}
	
	//Write to register
	int bytes = strlen(info);
	if(write(fd, info, bytes) < bytes) {
		close(fd);
		return -1;
	}
	
	//Close file
	close(fd);
	return 1;
	
}

/**************************************************************
* @Nombre: FILE_getAstroData
* @Def: Obtiene los datos de las constelaciones de un fichero
* @Arg:	In: file_name = fichero a leer
*		Out: data = informacion de las constelaciones
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
**************************************************************/
int FILE_getAstroData(char * file_name, struct AstroData * data) {

	int fd;
	char full_path[MAX_PATH];
	
	//Get full path
	strcpy(full_path, FILE_PATH);
	strcat(full_path, file_name);
	
	//Open file
	fd = open(full_path, O_RDONLY);
	if(fd < 0) {
		return -1;
	}
	
	//Prepare aux data
	struct AstroData new_data = ASTRODATA_defaultData();
	char * aux;
	double aux_d;
	
	//Get file data
	while(1) {
		
		//Get density
		aux = readCharUntil(fd, ' ');
		if(aux == NULL || aux[0] == '\0') {
			if(aux != NULL) {
				free(aux);
			}
			break;
		}
		free(aux);
		aux = readCharUntil(fd, ' ');
		if(aux == NULL || aux[0] == '\0') {
			if(aux != NULL) {
				free(aux);
			}
			break;
		}
		aux_d = atof(aux);
		free(aux);
		
		//Update density average
		new_data.density_avg = (new_data.density_avg * new_data.total_const + aux_d) / (double) (new_data.total_const + 1);
		
		//Get magnitude
		aux = readCharUntil(fd, '\n');
		if(aux == NULL || aux[0] == '\0') {
			if(aux != NULL) {
				free(aux);
			}
			break;
		}
		aux_d = atof(aux);
		free(aux);
		
		//Check first loop
		if(new_data.total_const == 0) {
			new_data.min_mag = aux_d;
			new_data.max_mag = aux_d;
		} else {
			//Update min data
			if(aux_d < new_data.min_mag) {
				new_data.min_mag = aux_d;
			}
			//Update max data
			if(aux_d > new_data.max_mag) {
				new_data.max_mag = aux_d;
			}
		}
		
		//Increase data
		new_data.total_const++;
		
	}
	
	//Check if data was found
	if(new_data.total_const == 0) {
		close(fd);
		return -1;
	}
	
	//Set data to return
	data->total_const = new_data.total_const;
	data->density_avg = new_data.density_avg;
	data->min_mag = new_data.min_mag;
	data->max_mag = new_data.max_mag;
	close(fd);
	return 1;
	
}