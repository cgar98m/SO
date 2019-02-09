#include "astrodata.h"

/***********************************************************
* @Nombre: ASTRODATA_defaultData
* @Def: Devuelve los valores por defecto de la informacion
* @Arg:	---
* @Ret: Informacion base
***********************************************************/
struct AstroData ASTRODATA_defaultData() {
	struct AstroData data;
	data.total_const = 0;
	data.density_avg = 0.0f;
	data.min_mag = 0.0f;
	data.max_mag = 0.0f;
	return data;
}

/*************************************************
* @Nombre: ASTRODATA_cleanData
* @Def: Limpia una estructura del tipo AstroData
* @Arg:	In:/Out: Informacion a limpiar
* @Ret: ---
*************************************************/
void ASTRODATA_cleanData(struct AstroData * data) {
	data->total_const = 0;
	data->density_avg = 0.0f;
	data->min_mag = 0.0f;
	data->max_mag = 0.0f;
}