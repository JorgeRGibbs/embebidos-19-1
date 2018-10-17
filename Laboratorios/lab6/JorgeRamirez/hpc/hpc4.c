#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N 32
#define NUM_HILOS 4
//Paralelismo por bloques y procesos

void * funHilo(void *arg);
int * reservaMemoria();
void llenarArreglo(int * datos);
void imprimirArreglo(int *datos);
void procesoPadre(int pipefd[]);

int *A,*B,*C;
//pthread_mutex_t bloqueo;
int contador;

int main(){
	pid_t pid;
	register int np;
	int pipefd[2]={1,0}, pipe_status;
	printf("probando procesos...\n");

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

	procesoPadre( pipefd );
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

void procesoPadre(int pipefd[])
{
	int np,npc,tamBloque = N / NUM_PROC;
	pid_t,pid_hijo;

	printf("Proceso padre ejecutado con pid %d\n", getpid() );

	close( pipefd[1]);

	for (int i = 0; i < NUM_PROC; i++)
	{
		pid_hijo = wait(&npc);
		npc = npc >> 8;
		iniBloque = npc * tamBloque;
		read (pipefd[0], C+iniBloque , sizeof(int)* tamBloque);
		printf("proceso hijo %d terminado con pid %d\n", npc, pid_hijo );
	}
	close(pipefd[0]);
}

void procesoHijo (int np, int pipefd[])
{
	int tamBloque = N / NUM_PROC;
	int iniBloque = np * tamBloque;
	int finBloque = iniBloque + tamBloque;

	printf("Proceso hijo %d ejecutado con pid %d\n", np, getpid() );

	for (int i = 0; i < finBloque; i++)
	{
		C[i] = A[i]*B[i];
	}
	write( pipefd[1] , C+iniBloque, );
	exit(np);
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