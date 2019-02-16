#include "file.h"

/*************************************************************
* @Nombre: readCharUntil
* @Def: Lee de un fd hasta que se encuentra con el delimiter
* @Arg:	In: fd = file descriptor a leer
*		In: delimiter = caracter que delimita la lectura
* @Ret: devuelve el string leido (NULL si da error)
*************************************************************/
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
	if(data[bytes - 1] == '\r') {
		
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

/*****************************************************************
* @Nombre: readIntUntil
* @Def: Lee de un fd hasta que se encuentra con el delimiter
* @Arg:	In: fd = file descriptor a leer
*		In: delimiter = caracter que delimita la lectura
* @Ret: devuelve el entero leido (si da error o 0, devuelve -1)
*****************************************************************/
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

/*****************************************************
* @Nombre: FILE_readConfig
* @Def: Obtiene la configuracion de un fichero
* @Arg:	In: file_name = nombre del fichero
*		Out: config = configuracion a devolver
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
*****************************************************/
int FILE_readConfig(char * file_name, struct Config * config) {

	//Open file
	int fd = open(file_name, O_RDONLY);
	if(fd < 0) {
		return -1;
	}
	
	//Get telescope name
	config->telescope = readCharUntil(fd, '\n');
	if(config->telescope == NULL) {
		close(fd);
		return -1;
	}
	
	//Get revision time
	config->revision_time = readIntUntil(fd, '\n');
	if(config->revision_time <= 0) {
		CONFIG_cleanConfig(config);
		close(fd);
		return -1;
	}
	
	//Get address
	config->address = readCharUntil(fd, '\n');
	if(config->address == NULL) {
		CONFIG_cleanConfig(config);
		close(fd);
		return -1;
	}
	
	//Get port
	config->port = readIntUntil(fd, '\n');
	if(config->port == -1) {
		CONFIG_cleanConfig(config);
		close(fd);
		return -1;
	}
	
	//Get files directory
	config->files_dir = readCharUntil(fd, '\n');
	if(config->files_dir == NULL) {
		CONFIG_cleanConfig(config);
		close(fd);
		return -1;
	}
	
	//Close file
	close(fd);
	return 1;

}

/************************************************************
* @Nombre: chooseFile
* @Def: Identifica los directorios '.' y '..'
* @Arg:	In: arg = archivo/directorio a comprobar
* @Ret: devuelve 0 en caso de ser '.' o '..' y 1 si no lo es
*************************************************************/
static int chooseFile(const struct dirent *arg) {
	if(strcmp(arg->d_name, ".") == 0 || strcmp(arg->d_name, "..") == 0) {
		return 0;
	}
	return 1;
}

/************************************************************
* @Nombre: getFileEnding
* @Def: Obten la terminacion de 3 caracteres de un fichero
* @Arg:	In: file_name = nombre del fichero
*		Out: ending = variable donde guardar la terminacion
* @Ret: ---
*************************************************************/
void getFileEnding(char * file_name, char ending[4]) {
	int length = strlen(file_name);
	ending[0] = file_name[length - 3];
	ending[1] = file_name[length - 2];
	ending[2] = file_name[length - 1];
	ending[3] = '\0';
}

/****************************************************************
* @Nombre: toLowerCase
* @Def: Transforma una string a valores alfabeticos en minuscula
* @Arg:	In: string = cadena de caracteres a tratar
* @Ret: ---
****************************************************************/
void toLowerCase(char * string) {
	
	for(int i = 0; i < (int) strlen(string); i++) {
		
		//Check end of string
		if(string[i] == '\0') {
			break;
		}
		
		//Check if is an alphabetic value
		if(string[i] >= 'A' && string[i] <= 'Z') {
			string[i] = string[i] - 'A' + 'a';
		}
		
	}
	
}

/**************************************************
* @Nombre: addFile
* @Def: Anade el nombre de un fichero a un puntero
* @Arg:	In/Out: pointer = puntero a tratar
*		In: file_name = nombre del fichero
*		In: total = ficheros actuales
* @Ret: ---
**************************************************/
int addFile(char *** pointer, char * file_name, int total) {

	char ** new_pointer;
	
	//Get memory for a new file
	if(*pointer == NULL) {
		new_pointer = (char **) malloc(sizeof(char *));
	} else {
		new_pointer = (char **) realloc(*pointer, sizeof(char *) * (total + 1));
	}
	
	//Check memory
	if(new_pointer == NULL) {
		return -1;	
	} else {
		*pointer = new_pointer;
	}
	
	//Get memory for file name
	(*pointer)[total] = (char *) malloc(strlen(file_name) + 1);
	if((*pointer)[total] == NULL) {
		return -1;
	}
	
	//Copy file name
	strcpy((*pointer)[total], file_name);
	return 1;
	
}

/************************************************************************************
* @Nombre: freeFileData
* @Def: Libera la memoria usada por un puntero de un conjunto de nombres de fichero
* @Arg:	In/Out: pointer = puntero a tratar
*		In: total = numero de ficheros por liberar
* @Ret: ---
************************************************************************************/
void freeFileData(char *** pointer, int total) {
	
	//Check if has data
	if(*pointer == NULL) {
		return;
	}
	
	//Free file names
	for(int i = 0; i < total; i++) {
		if(*pointer[i] != NULL) {
			free(*pointer[i]);
		}
	}
	
	//Free file pointer
	free(*pointer);
	*pointer = NULL;
	
}

/*************************************************************************
* @Nombre: FILE_getDirectoryFiles
* @Def: Obtiene los ficheros del directorio FILE_PATH
* @Arg:	Out: txt = referencia del puntero a los nombres de ficheros .txt
*		Out: jpg = referencia del puntero a los nombres de ficheros .jpg
*		Out: total = array con el total de ficheros de ambos tipos
* @Ret: devuelve 1 si ha ido bien y -1 si ha ido mal
*************************************************************************/
int FILE_getDirectoryFiles(char *** txt, char *** jpg, int total[2], char * path) {
	
	//Auxiliars
	struct dirent ** files = NULL;
	char ending[4];
	int total_files;
	
	//Init values
	*jpg = NULL;
	*txt = NULL;
	total[0] = 0;
	total[1] = 0;
	
	//Get directory files
	total_files = scandir(path, &files, chooseFile, alphasort);
	for(int i = 0; i < total_files; i++) {
		
		//Get ending
		getFileEnding(files[i]->d_name, ending);
		toLowerCase(ending);
		
		//Check file type
		if(strcmp(ending, TXT_ENDING) == 0) {
			//Add txt file
			if(addFile(txt, files[i]->d_name, total[0]) < 0) {
				//Free memory
				freeFileData(jpg, total[0]);
				freeFileData(txt, total[1]);
				for(int j = i; j < total_files; j++) {
					free(files[j]);
				}
				free(files);
				return -1;
			} else {
				total[0]++;
				free(files[i]);
			}
		} else {
			//Add jpg file
			if(addFile(jpg, files[i]->d_name, total[1]) < 0) {
				//Free memory
				freeFileData(jpg, total[0]);
				freeFileData(txt, total[1]);
				for(int j = i; j < total_files; j++) {
					free(files[j]);
				}
				free(files);
				return -1;
			} else {
				total[1]++;
				free(files[i]);
			}
		}
		
	}
	
	//Free files data
	if(files != NULL) {
		free(files);
	}
	
	//All ok
	return 1;
	
}

/****************************************************************
* @Nombre: FILE_readFile
* @Def: Obtiene el contenido de un fichero a nivel de byte
* @Arg:	In: file_name = nombre del fichero
*		Out: bytes = numero de bytes de la informacion obtenida
*		In: path = carpeta donde se encuentra el fichero
*		In: turn_off = flag que indica si se cierra el programa
* @Ret: devuelve el contenido del fichero (NULL si ha ido mal)
****************************************************************/
char * FILE_readFile(char * file_name, long * bytes, char * path, int * turn_off) {
	
	int fd;
	char full_path[MAX_PATH];
	
	//Get full path
	strcpy(full_path, path);
	strcat(full_path, "/");
	strcat(full_path, file_name);
	
	//Open file
	fd = open(full_path, O_RDONLY);
	if(fd < 0) {
		return NULL;
	}
	
	//Read file content
	*bytes = 0;
	char * buffer = (char *) malloc(1);
	while(1) {
		
		//Check buffer memory
		if(buffer == NULL) {
			close(fd);
			return NULL;
		}
		
		//Check program end
		if(*turn_off > 0) {
			close(fd);
			free(buffer);
			return NULL;
		}
		
		//Read until read error
		if(read(fd, &buffer[*bytes], 1) <= 0) {
			buffer[*bytes] = '\0';
			break;
		}
		
		//Increase memory
		(*bytes)++;
		buffer = (char *) realloc(buffer, *bytes + 1);
		
	}

	//Close file
	close(fd);
	return buffer;
	
}

/*******************************************************
* @Nombre: md5sum
* @Def: Obten el md5sum escrito en un file descriptor
* @Arg:	In: fd = file descriptor
* @Ret: devuelve el md5sum del file descriptor
*******************************************************/
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

/**********************************************************
* @Nombre: FILE_checksum
* @Def: Obten el checksum de un fichero
* @Arg:	In: file_name = nombre del fichero
*		In: path = carpeta donde se encuentra el fichero
* @Ret: devuelve el checksum del fichero
**********************************************************/
char * FILE_checksum(char * file_name, char * path) {

	char full_path[MAX_PATH];
	char * checksum;
	
	//Get full path
	strcpy(full_path, path);
	strcat(full_path, "/");
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
		if (p_fork == 0) {

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

/**********************************************************
* @Nombre: FILE_deleteFile
* @Def: Elimina un fichero del sistema de ficheros
* @Arg:	In: file_name = nombre del fichero
*		In: path = carpeta donde se encuentra el fichero
* @Ret: devuelve 0 si ha ido bien y -1 si ha ido mal
**********************************************************/
int FILE_deleteFile(char * file_name, char * path) {
	
	char full_path[MAX_PATH];
	
	//Get full path
	strcpy(full_path, path);
	strcat(full_path, "/");
	strcat(full_path, file_name);
	
	return unlink(full_path);
	
}