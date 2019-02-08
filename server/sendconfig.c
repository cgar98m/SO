#include "sendconfig.h"

/*************************************************************
* @Nombre: SENDCONFIG_defaultConfig
* @Def: Establece los valores por defecto de la configuracion
* @Arg:	In/Out: Configuracion a establecer
* @Ret: ---
*************************************************************/
void SENDCONFIG_defaultConfig(struct SendConfig * config) {
	config->type[0] = '\0';
	config->size = -1;
	config->current_size = 0;
	config->data = NULL;
	config->file = NULL;
}

/************************************************************************************
* @Nombre: SENDCONFIG_cleanConfig
* @Def: Libera la memoria usada por la configuracion y establece valores por defecto
* @Arg:	In:/Out: Configuracion a limpiar
* @Ret: ---
************************************************************************************/
void SENDCONFIG_cleanConfig(struct SendConfig * config) {
	
	config->type[0] = '\0';
	config->size = -1;
	config->current_size = 0;
	
	if(config->data != NULL) {
		free(config->data);
		config->data = NULL;
	}
	
	if(config->file != NULL) {
		free(config->file);
		config->file = NULL;
	}
	
}