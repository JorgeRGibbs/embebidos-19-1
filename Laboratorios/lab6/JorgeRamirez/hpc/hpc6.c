#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N 64
#define NUM_HILOS 4
//Paralelismo por bloques

void * funHilo(void *arg);
int * reservaMemoria();
void llenarArreglo(int * datos);
void imprimirArreglo(int *datos);
pthread_mutex_t bloqueo;

int tambloque = N/NUM_HILOS;
int *A;
int prom;

int main(){
	pthread_t tids[NUM_HILOS];
	int nhs[NUM_HILOS] , *res;
	register int nh;
	printf("probando hilos \n");
	pthread_mutex_init(&bloqueo,NULL);
	A = reservaMemoria();
	llenarArreglo ( A );
	imprimirArreglo( A );



	for(nh =0; nh<NUM_HILOS; nh++){
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL,funHilo,(void*)&nhs[nh]);
		printf("Hilos creados\n");
	}

	for(nh =0; nh<NUM_HILOS; nh++){

		pthread_join(tids[nh], (void **)&res);
		printf("Hilo %d terminado\n", *res);
	}
	
	//imprimirArreglo(A);


	pthread_mutex_destroy(&bloqueo);
	free(A);

	return 0;
}
//parellismo alternado
//2,6,14, .... 
//1,5,9 ....

//Alternada

void * funHilo(void *arg){
	register int i=0;
	
	int nh = *(int*)arg;
	int suma_parcial = 0 ;
	//int *A;
	//while((--i));
	for (i = nh ; i < N ; i+= NUM_HILOS)
	{
		suma_parcial += A[i];
	}
	pthread_mutex_lock(&bloqueo);
	prom += suma_parcial;
	//printf("suma: %d\n",prom);
	if (nh == 3)
	{
		printf("El promedio es: %d\n", prom>>6);
	}
	pthread_mutex_unlock(&bloqueo);
	
	pthread_exit(arg);
}

/*
void * funHilo(void *arg){
	register int i=0;
	//int *A;
	int nh = *(int*)arg;
	for (i = nh ; i < N ; i+= NUM_HILOS)
	{
		pthread_mutex_lock(&bloqueo);
		prom += A[i];
		pthread_mutex_unlock(&bloqueo);
	}
	pthread_exit(arg);
}
*/

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
	//srand(getpid());
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
