#include "config.h"

/*************************************************************
* @Nombre: CONFIG_defaultConfig
* @Def: Establece los valores por defecto de la configuracion
* @Arg:	In/Out: Configuracion a establecer
* @Ret: ---
*************************************************************/
void CONFIG_defaultConfig(struct Config * config) {
	config->telescope = NULL;
	config->revision_time = -1;
	config->address = NULL;
	config->port = -1;
	config->files_dir = NULL;
}

/************************************************************************************
* @Nombre: CONFIG_cleanConfig
* @Def: Libera la memoria usada por la configuracion y establece valores por defecto
* @Arg:	In:/Out: Configuracion a limpiar
* @Ret: ---
************************************************************************************/
void CONFIG_cleanConfig(struct Config * config) {
	
	//Clean telescope name
	if(config->telescope != NULL) {
		free(config->telescope);
		config->telescope = NULL;
	}
	
	//Clean revision time
	config->revision_time = -1;
	
	//Clean address
	if(config->address != NULL) {
		free(config->address);
		config->address = NULL;
	}
	
	//Clean port
	config->port = -1;

	//Clean files directory
	if(config->files_dir != NULL) {
		free(config->files_dir);
		config->files_dir = NULL;
	}
	
}