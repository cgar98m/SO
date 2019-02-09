#ifndef _ASTRODATA_H
#define _ASTRODATA_H

//AstroData
struct AstroData {
	long total_const;
	double density_avg;
	double min_mag;
	double max_mag;	
};

/***********************************************************
* @Nombre: ASTRODATA_defaultData
* @Def: Devuelve los valores por defecto de la informacion
* @Arg:	---
* @Ret: Informacion base
***********************************************************/
struct AstroData ASTRODATA_defaultData();

/*************************************************
* @Nombre: ASTRODATA_cleanData
* @Def: Limpia una estructura del tipo AstroData
* @Arg:	In:/Out: Informacion a limpiar
* @Ret: ---
*************************************************/
void ASTRODATA_cleanData(struct AstroData * data);

#endif