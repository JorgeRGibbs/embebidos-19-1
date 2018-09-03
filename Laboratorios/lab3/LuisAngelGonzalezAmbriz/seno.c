#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define MUESTRAS 512
void genera_seno(float seno[]);
void guarda_datos(float datos[]);
int main()
{

	float seno[MUESTRAS];
	genera_seno(seno);
	guarda_datos(seno);

	return 0;
}


void genera_seno(float seno[])
{
	float f=1000,fs=45000;
	register int n;
	for ( n = 0; n < MUESTRAS; n++)
	{
		seno[n]=sinf(2*M_PI*f*n/fs);
	}
}

void guarda_datos(float datos[])
{

	FILE *ap_arch;
	register int n;
	ap_arch=fopen("seno.dat","w");
	if(!ap_arch)
	{
		perror("Error al abrir Archivo");
		exit(EXIT_FAILURE);

	}
	for(n=0;n<MUESTRAS;n++)
	{

		fprintf(ap_arch, "%d %f \n",n,datos[n]);
	}
	fclose(ap_arch);
	system("gnuplot 'graficar'");

}