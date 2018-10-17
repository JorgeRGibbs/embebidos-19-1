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


int *A,*B,*C;
//pthread_mutex_t bloqueo;
int contador;

int main(){
	pthread_t tids[NUM_HILOS];
	int nhs[NUM_HILOS] , *res;
	register int nh;
	printf("probando hilos \n");

	A = reservaMemoria();
	llenarArreglo ( A );
	imprimirArreglo( A );

	B = reservaMemoria();
	llenarArreglo ( B );
	imprimirArreglo( B );

	C = reservaMemoria();
	//imprimirArreglo( C );

	free ( A );
	free ( B );
	free ( C );
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
	imprimirArreglo( C );


	free(A);
	free(B);
	free(C);

	return 0;
}
//parellismo alternado
//2,6,14, .... 
//1,5,9 ....

void * funHilo(void *arg){
	register int i=0;
	int nh = *(int*)arg;

	for (i = nh ; i < N ; i+= NUM_HILOS)
	{
		C[i] = A[i] * B[i];
	}

	pthread_exit(arg);
}

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
		if(!(i%8)){
			printf("\n");
		}
		printf("%5d",datos[i]);
		printf(",");
	}
	printf("\n");
}
/*
void * Mayor(void * arg){
	register int i;
	static int mayor;
	int *datos =(int *)arg;
	mayor = datos[0];
	for(i =1; i<N;i++){
		if(datos[i]>mayor){
			mayor = datos[i];
		}
	}
	pthread_exit(&mayor);
}
void * Menor(void * arg){
	register int i;
	static int menor;
	int *datos =(int *)arg;
	menor = datos[0];
	for(i =1; i<N;i++){
		if(datos[i]<menor){
			menor = datos[i];
		}
	}
	pthread_exit(&menor);
}

}*/