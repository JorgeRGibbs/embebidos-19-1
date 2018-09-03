#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

void guarda_datos( float seno[] )
{
	FILE *ap;
	register int n;

	ap = fopen("seno.dat","w");
	if( !ap )
	{
		perror("Error al abrir el archivo");
		exit(EXIT_FAILURE);
	}
	for( n = 0; n < MUESTRAS; n++ )
	{
		fprintf(ap, "%f\n", seno[n] );
	}
	fclose( ap );
}




