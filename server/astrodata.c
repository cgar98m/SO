#include "astrodata.h"

void cleanData(struct AstroData * data) {
	data->total_const = 0;
	data->density_avg = 0.0f;
	data->min_mag = 0.0f;
	data->max_mag = 0.0f;
}

struct AstroData defaultData() {
	struct AstroData data;
	data.total_const = 0;
	data.density_avg = 0.0f;
	data.min_mag = 0.0f;
	data.max_mag = 0.0f;
	return data;
}