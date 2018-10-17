#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

void guarda_datos(float w[]){
	FILE *ap_arch;
	register int n;

	ap_arch = fopen("seno2.dat", "w");
	if(!ap_arch){
		perror("error al abrir el archivo");
	}
	for(n= 0; n<MUESTRAS; n++){
		fprintf(ap_arch, "%f \n", w[n]);
	}
	fclose(ap_arch);
}