#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "archivos.h"
#include "procesamiento.h"
#include "defs.h"


float Hamming(float seno[], float y[],int finBloque)
{
	float w[MUESTRAS];
	register int i;
	//double x  = ((2*PI*n)/(N -1));
	w[i] = (0.54 - (0.46*cosf(((2*M_PI*i)/(MUESTRAS-1)))));
	y[i] = seno[i] * w[i];
	return *y;
}

