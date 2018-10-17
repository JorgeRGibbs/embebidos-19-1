#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "archivos.h"
#include "procesamiento.h"
#include "hamming.h"
#include "defs.h"
#include "procesos.h"

void genera_seno(float seno[]);
void guarda_datos(float y[]);
void procesos(float seno[]);


float *y;
float *w;

int main(){
	float seno[MUESTRAS];
	genera_seno(seno);
	procesos(seno);
	guarda_datos(w);
	return 0;
}

