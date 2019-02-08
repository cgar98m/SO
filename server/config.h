#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdlib.h>

//Config
struct Config {
	char * ip;
	int mcgruder_port;
	int mctravish_port;
	int param;
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