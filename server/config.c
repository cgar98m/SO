#include "config.h"

/*************************************************************
* @Nombre: CONFIG_defaultConfig
* @Def: Establece los valores por defecto de la configuracion
* @Arg:	In/Out: Configuracion a establecer
* @Ret: ---
*************************************************************/
void CONFIG_defaultConfig(struct Config * config) {
	config->ip = NULL;
	config->mcgruder_port = -1;
	config->mctravish_port = -1;
	config->param = -1;
}

/************************************************************************************
* @Nombre: CONFIG_cleanConfig
* @Def: Libera la memoria usada por la configuracion y establece valores por defecto
* @Arg:	In:/Out: Configuracion a limpiar
* @Ret: ---
************************************************************************************/
void CONFIG_cleanConfig(struct Config * config) {
	
	//Clean ip
	if(config->ip != NULL) {
		free(config->ip);
		config->ip = NULL;
	}
	
	//Clean client port
	config->mcgruder_port = -1;
	
	//Clean server port
	config->mctravish_port = -1;
	
	//Celan param
	config->param = -1;

}