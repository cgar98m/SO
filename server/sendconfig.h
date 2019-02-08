#ifndef _SENDCONFIG_H
#define _SENDCONFIG_H

#include <stdlib.h>

//SendConfig
struct SendConfig {
	char type[4];
	long size;
	long current_size;
	char * data;
	char * file;
};

/*************************************************************
* @Nombre: SENDCONFIG_defaultConfig
* @Def: Establece los valores por defecto de la configuracion
* @Arg:	In/Out: Configuracion a establecer
* @Ret: ---
*************************************************************/
void SENDCONFIG_defaultConfig(struct SendConfig * config);

/************************************************************************************
* @Nombre: SENDCONFIG_cleanConfig
* @Def: Libera la memoria usada por la configuracion y establece valores por defecto
* @Arg:	In:/Out: Configuracion a limpiar
* @Ret: ---
************************************************************************************/
void SENDCONFIG_cleanConfig(struct SendConfig * config);

#endif