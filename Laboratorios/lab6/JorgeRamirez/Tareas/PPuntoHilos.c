//PRODUCTO PUNTO CON HILOS Y PARALELISMO ALTERNADO
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
pthread_mutex_t bloqueo;

int tambloque = N/NUM_HILOS;
int *A,*B,*AB;
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

	B = reservaMemoria();
	llenarArreglo ( B );
	imprimirArreglo( B );

	AB = reservaMemoria();

	for(nh =0; nh<NUM_HILOS; nh++){
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL,funHilo,(void*)&nhs[nh]);
		printf("Hilos creados\n");
	}

	for(nh =0; nh<NUM_HILOS; nh++){

		pthread_join(tids[nh], (void **)&res);
		printf("Hilo %d terminado\n", *res);
	}
	
	imprimirArreglo(AB);


	pthread_mutex_destroy(&bloqueo);
	free(A);
	free(B);
	free(AB);

	return 0;
}
//parellismo alternado
//2,6,14, .... 
//1,5,9 ....

//Forma Alternada

void * funHilo(void *arg){
	register int i=0;
	
	int nh = *(int*)arg;
	int suma = 0 ;
	//int *A;
	//while((--i));
	for (i = nh ; i < N ; i+= NUM_HILOS)
	{
		AB[i]=A[i]*B[i];
		suma += AB[i];
	}
	pthread_mutex_lock(&bloqueo);
	prom += suma;
	//printf("suma: %d\n",prom);
	if (nh == 3)
	{
		printf("El producto punto es: %d\n", prom);
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


