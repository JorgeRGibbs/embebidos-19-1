#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N 32
#define NUM_HILOS 4
//Paralelismo por bloques

void * funHilo(void *arg);
int * reservaMemoria();
void llenarArreglo(int * datos);
void imprimirArreglo(int *datos);


int *datos;
//pthread_mutex_t bloqueo;
int contador;


int * reservaMemoria(){
	int *mem;
	mem = (int*)malloc(sizeof(int)*N);
	if(!mem){

		perror("error al hacer la memoria");
		exit(EXIT_FAILURE);
	}
	return mem;
}


void llenarArreglo(int * datos){
	register int i;
	srand(getpid());
	for(i=0;i<N;i++){
		datos[i] = rand()%255;
	}
}

void imprimirArreglo(int * datos){
	register int i;
	for(i = 0; i<N;i++){
		if(!(i%16)){
			printf("\n");
		}
		printf("%5d",datos[i]);
		printf(",");
	}
	printf("\n");
}


int *menorArreglo (int *datos)
{
	register int i, menor;
	
	menor  = datos[0];
	
	for (int i = 0; i < NUM_HILOS; i++)
	{
		if (datos [i] < menor)
		{
			menor = datos[i];
		}
	}
	return menor;
}

int *mayorArreglo (int *datos)
{
	register int i, mayor;
	mayor  = datos[0];
	for (int i = 0; i < NUM_HILOS; i++)
	{
		if (datos [i] > mayor)
		{
			mayor = datos[i];
		}
	}
}

int promedioArreglo(int *datos)
{
	register int i;
	int prom,aux,sum;
	for (i = 0; i < N ; i++)
	{
		aux = datos[i];
//		printf("aux: %d\n",datos[i] );
		sum= sum + aux;
		fflush(stdout);
	}

	prom = sum/N;
	return prom;
}

int ordenaArreglo(int *datos)
{	
	register int i;
	int d,t;
	for (i = 1 ; i <= N - 1; i++) {
    	d = i;
    	while ( d > 0 && datos[d-1] > datos[d]) {
    		t          = datos[d];
      		datos[d]   = datos[d-1];
      		datos[d-1] = t;
 			d--;
    	}
  	}
 	/*printf("Arreglo ordenado\n");
  	for (i = 0; i <= N - 1; i++) {
    	printf("Dato %d : %d\n", i,datos[i]);
  }*/
  return *datos;
 
}

void * funHilo(void *arg){
	register int i=0;
	int nh = *(int*)arg;

	if (nh == 0)
	{
		int menor = menorArreglo(nh);
		printf("\nMenor: %d",menor);
		exit(0);
	}
	if (nh == 1)
	{
		int mayor = (int*)mayorArreglo(nh);
		printf("\n Mayor: %d",mayor);
		exit(0);
	}
	if (nh == 2)
	{
		int prom = (int*)promedioArreglo(nh);
		printf("\n Promedio: %d", prom);
		exit(0);		
	}
	if (nh == 3)
	{
		int *ord = (int*)ordenaArreglo(datos);
		printf("Arreglo:\n");
		imprimirArreglo(datos);
		printf("Arreglo ordenado: \n");
		imprimirArreglo(ord);
	}

	pthread_exit(arg);
}
int main(){
	pthread_t tids[NUM_HILOS];
	int nhs[NUM_HILOS] , *res;
	register int nh;
	printf("probando hilos \n");

	datos = reservaMemoria();

/*
	datos = memoria();
	llenarArreglo(datos);
	imprimir(datos);
*/
	for(nh =0; nh<NUM_HILOS; nh++){
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL,funHilo,(void*)&nhs[nh]);
	}
	for(nh =0; nh<NUM_HILOS; nh++){

		pthread_join(tids[nh], (void **)&res);
		printf("Hilo %d terminado\n", *res);
	}


	free ( datos );

	return 0;
}