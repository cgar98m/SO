#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdlib.h>

//Config
struct Config {
	char * telescope;
	int revision_time;
	char * address;
	int port;
	char * files_dir;
};

/*************************************************************
* @Nombre: CONFIG_defaultConfig
* @Def: Establece los valores por defecto de la configuracion
* @Arg:	In/Out: Configuracion a establecer
* @Ret: ---
*************************************************************/
void CONFIG_defaultConfig(struct Config * config);

/************************************************************************************
* @Nombre: CONFIG_cleanConfig
* @Def: Libera la memoria usada por la configuracion y establece valores por defecto
* @Arg:	In:/Out: Configuracion a limpiar
* @Ret: ---
************************************************************************************/
void CONFIG_cleanConfig(struct Config * config);

#endif