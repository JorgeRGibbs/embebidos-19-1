#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N 8192
//#define NUM_HILOS 4

//Version secuencial 

int *A,*B,*C;
int *reservaMemoria();
void llenarArreglo(int *datos);
void imprimirArreglo (int *datos);
void producto(void);

int main(int argc, char const *argv[])
{
	A = reservaMemoria();
	llenarArreglo ( A );
	imprimirArreglo( A );

	B = reservaMemoria();
	llenarArreglo ( B );
	imprimirArreglo( B );

	C = reservaMemoria();
	producto();
	imprimirArreglo( C );

	free ( A );
	free ( B );
	free ( C );
	return 0;
}

void  producto(void){
	register int i;
	for (int i = 0; i < N; i++)
	{
		C[i] = A[i] * B[i];
	}
}

void llenarArreglo(int *datos)
{
	srand(time(NULL));
	register int i;
	for (i = 0; i < N; i++)
	{
		datos[i] = rand() % 255;
	}
}

void imprimirArreglo(int * datos){
	//int mayor,menor;
	register int i;
	//printf("El número mayor es: %d\n",mayor);
	//printf("El número menor es: %d\n",menor);
	//printf("El promedio es: %d \n",prom );
	//printf("Arreglo ordenado: \n");
	for (i = 0; i < N ; i++)
	{
		printf("Dato %d : %d\n",i,datos[i] );
	}

	free(datos);
	printf("Memoria liberada\n");
}

int * reservaMemoria()
{
	int *mem;
	mem = (int *) malloc(N * sizeof(int ));
	//printf("Memoria reservada en %p \n", &mem);
	if (!mem)
	{
		perror("Error al asignar memoria");
		exit(EXIT_FAILURE);
	}
	return mem;
}


