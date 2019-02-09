#ifndef _ASTRODATA_H
#define _ASTRODATA_H

struct AstroData {
	long total_const;
	double density_avg;
	double min_mag;
	double max_mag;	
};

void cleanData(struct AstroData * data);

struct AstroData defaultData();

#endif